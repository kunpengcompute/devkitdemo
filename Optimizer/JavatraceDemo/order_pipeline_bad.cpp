/*
 * order_pipeline_bad.cpp — 故意制造三个可被 perf / async-profiler 量化的问题：
 *   1) calcPrice   纯计算热点（CPU-bound，几百~几千轮哈希 / xorshift）
 *   2) checkStock  全局 pthread_mutex，把重计算 + 递减库存 + 多次小 write 全部放锁内
 *   3) asyncPersist 每 64 笔订单 ~1.5ms usleep，形成可进入 p99 的孤立长尾
 *
 * 三个 JNI 入口与 NativeBridge.java 严格对应（Java 侧校验在 Java 侧做，不走 JNI）：
 *   Java_NativeBridge_calcPrice    ([BIL)J
 *   Java_NativeBridge_checkStock   ([BIL)J
 *   Java_NativeBridge_asyncPersist ([BILJ)J
 *
 * 编译：
 *   g++ -g -O0 -fPIC -fno-inline -fno-omit-frame-pointer \
 *       -fno-optimize-sibling-calls -pthread -shared \
 *       -I$JAVA_HOME/include -I$JAVA_HOME/include/linux \
 *       -o liborder_pipeline.so order_pipline_bad.cpp
 */

#include <jni.h>
#include <pthread.h>
#include <atomic>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern "C" {

/* ================= 全局状态 ================= */

/* 共享库存，初值 10 亿（原子变量） */
static std::atomic<long long> g_stock(1000000000LL);

/* checkStock 全局互斥锁 */
static pthread_mutex_t g_stock_mutex = PTHREAD_MUTEX_INITIALIZER;

/* /dev/null 的文件句柄（延迟打开） */
static FILE* g_devnull = NULL;

/* 每 64 笔订单触发一次 usleep 的计数器 */
static std::atomic<int> g_persist_counter(0);

/* ================= 内部辅助函数（均加 noinline） ================= */

__attribute__((noinline))
static uint64_t price_transform(const uint8_t* data, size_t size, int rounds) {
    /* xorshift + 混合哈希循环 */
    uint64_t state = 0x9E3779B97F4A7C15ULL;
    for (size_t i = 0; i < size; i++) {
        state ^= (uint64_t)data[i];
        state *= 0xD1B54A32D192ED03ULL;
    }
    for (int r = 0; r < rounds; r++) {
        state ^= state >> 12;
        state ^= state << 25;
        state ^= state >> 27;
        state *= 0x2545F4914F6CDD1DULL;
        uint64_t h = 14695981039346656037ULL;
        h ^= state;
        h *= 1099511628211ULL;
        state ^= h;
    }
    return state;
}

__attribute__((noinline))
static uint64_t stock_heavy_compute(const uint8_t* data, size_t size) {
    /* 故意把计算也放在锁内：200 轮 */
    return price_transform(data, size, 200);
}

__attribute__((noinline))
static void small_writes_to_devnull(const uint8_t* data, size_t size, int count) {
    if (g_devnull == NULL) g_devnull = fopen("/dev/null", "wb");
    if (g_devnull == NULL) return;
    for (int i = 0; i < count; i++) {
        fwrite(data + (i * 8) % size, 1, 8, g_devnull);
    }
    fflush(g_devnull);
}

__attribute__((noinline))
static void persist_tail_latency_tick(void) {
    usleep(1500); /* 1.5ms 人为长尾 */
}

/* ================= 3 个 JNI 入口 ================= */

/*
 * Class:     NativeBridge
 * Method:    calcPrice
 * Signature: ([BIL)J
 *
 * BAD：800 轮重计算，CPU 热点
 */
JNIEXPORT jlong JNICALL Java_NativeBridge_calcPrice(
        JNIEnv* env, jclass /*cls*/, jbyteArray jdata, jint jsize, jlong jorderId) {
    jbyte* data = (jbyte*)env->GetPrimitiveArrayCritical(jdata, NULL);
    if (data == NULL) return 0;
    uint64_t heavy = price_transform((const uint8_t*)data, (size_t)jsize, 800);
    uint64_t order_mix = (uint64_t)jorderId * 0x9E3779B97F4A7C15ULL;
    env->ReleasePrimitiveArrayCritical(jdata, data, JNI_ABORT);
    return (jlong)(heavy ^ order_mix);
}

/*
 * Class:     NativeBridge
 * Method:    checkStock
 * Signature: ([BIL)J
 *
 * BAD：把 重计算 + 递减库存 + 多次小 write 全部放进 mutex 临界区
 */
JNIEXPORT jlong JNICALL Java_NativeBridge_checkStock(
        JNIEnv* env, jclass /*cls*/, jbyteArray jdata, jint jsize, jlong jorderId) {
    jbyte* data = (jbyte*)env->GetPrimitiveArrayCritical(jdata, NULL);
    if (data == NULL) return -1;

    pthread_mutex_lock(&g_stock_mutex);

    /* --- 锁内：重计算（本应放在锁外） --- */
    uint64_t price_piece = stock_heavy_compute((const uint8_t*)data, (size_t)jsize);

    /* --- 锁内：递减共享库存 --- */
    long long cur = g_stock.load();
    if (cur < 1) {
        pthread_mutex_unlock(&g_stock_mutex);
        env->ReleasePrimitiveArrayCritical(jdata, data, JNI_ABORT);
        return -1;
    }
    g_stock.fetch_sub(1);

    /* --- 锁内：多次小 write（本应放在锁外） --- */
    small_writes_to_devnull((const uint8_t*)data, (size_t)jsize, 16);

    pthread_mutex_unlock(&g_stock_mutex);

    env->ReleasePrimitiveArrayCritical(jdata, data, JNI_ABORT);
    (void)price_piece;
    (void)jorderId;
    return (jlong)g_stock.load();
}

/*
 * Class:     NativeBridge
 * Method:    asyncPersist
 * Signature: ([BILJ)J
 *
 * BAD：每 64 笔订单 usleep 1.5ms，制造 p99 长尾
 */
JNIEXPORT jlong JNICALL Java_NativeBridge_asyncPersist(
        JNIEnv* env, jclass /*cls*/, jbyteArray jdata, jint jsize,
        jlong jorderId, jlong jprice) {
    jbyte* data = (jbyte*)env->GetPrimitiveArrayCritical(jdata, NULL);
    if (data == NULL) return 0;

    /* usleep：每 64 笔一次 */
    if (g_persist_counter.fetch_add(1) % 64 == 63) {
        persist_tail_latency_tick();
    }

    if (g_devnull == NULL) g_devnull = fopen("/dev/null", "wb");
    if (g_devnull) {
        fwrite(data, 1, 32, g_devnull);
        fflush(g_devnull);
    }

    env->ReleasePrimitiveArrayCritical(jdata, data, JNI_ABORT);
    (void)jorderId;
    (void)jprice;
    return 32;
}

} /* extern "C" */
