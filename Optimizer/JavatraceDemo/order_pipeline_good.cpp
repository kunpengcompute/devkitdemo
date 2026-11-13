/*
 * order_pipeline_good.cpp — 优化对照版：
 *   - checkStock 把重计算移到锁外，锁内只做 库存递减 + 最小化 IO
 *   - asyncPersist 去掉 usleep，只保留正常写
 *   - calcPrice 业务量不变
 *
 * 三个 JNI 入口与 NativeBridge.java 严格对应，Java 侧代码无需改动：
 *   Java_NativeBridge_calcPrice    ([BIL)J
 *   Java_NativeBridge_checkStock   ([BIL)J
 *   Java_NativeBridge_asyncPersist ([BILJ)J
 *
 * 编译：
 *   g++ -g -O0 -fPIC -fno-inline -fno-omit-frame-pointer \
 *       -fno-optimize-sibling-calls -pthread -shared \
 *       -I$JAVA_HOME/include -I$JAVA_HOME/include/linux \
 *       -o liborder_pipeline.so order_pipline_good.cpp
 */

#include <jni.h>
#include <pthread.h>
#include <atomic>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern "C" {

/* ================= 全局状态（与 bad 版保持一致） ================= */

static std::atomic<long long> g_stock(1000000000LL);
static pthread_mutex_t g_stock_mutex = PTHREAD_MUTEX_INITIALIZER;
static FILE* g_devnull = NULL;
static std::atomic<int> g_persist_counter(0);

/* ================= 内部辅助函数（均加 noinline） ================= */

__attribute__((noinline))
static uint64_t price_transform(const uint8_t* data, size_t size, int rounds) {
    /* 与 bad 版完全相同的算法，保证 calcPrice 业务量一致 */
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
static uint64_t stock_heavy_compute_outside_lock(const uint8_t* data, size_t size) {
    /* 锁外计算：同样 200 轮，但不持锁 */
    return price_transform(data, size, 200);
}

__attribute__((noinline))
static void small_write_outside_lock(const uint8_t* data, size_t size) {
    /* 也移到锁外：合并成一次 write */
    if (g_devnull == NULL) g_devnull = fopen("/dev/null", "wb");
    if (g_devnull == NULL) return;
    fwrite(data, 1, 32, g_devnull);
    fflush(g_devnull);
}

/* ================= 3 个 JNI 入口 ================= */

/*
 * Class:     NativeBridge
 * Method:    calcPrice
 * Signature: ([BIL)J
 *
 * 与 bad 版完全一致（纯计算热点，不算锁竞争问题）
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
 * GOOD：重计算 + IO 全部移到锁外，锁内只保留库存递减
 */
JNIEXPORT jlong JNICALL Java_NativeBridge_checkStock(
        JNIEnv* env, jclass /*cls*/, jbyteArray jdata, jint jsize, jlong jorderId) {
    jbyte* data = (jbyte*)env->GetPrimitiveArrayCritical(jdata, NULL);
    if (data == NULL) return -1;

    /* --- 锁外：重计算 --- */
    uint64_t price_piece = stock_heavy_compute_outside_lock((const uint8_t*)data, (size_t)jsize);

    /* --- 锁内：只做库存递减 --- */
    pthread_mutex_lock(&g_stock_mutex);
    long long cur = g_stock.load();
    if (cur < 1) {
        pthread_mutex_unlock(&g_stock_mutex);
        env->ReleasePrimitiveArrayCritical(jdata, data, JNI_ABORT);
        return -1;
    }
    g_stock.fetch_sub(1);
    pthread_mutex_unlock(&g_stock_mutex);

    /* --- 锁外：写 IO --- */
    small_write_outside_lock((const uint8_t*)data, (size_t)jsize);

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
 * GOOD：去掉 usleep，只保留正常写
 */
JNIEXPORT jlong JNICALL Java_NativeBridge_asyncPersist(
        JNIEnv* env, jclass /*cls*/, jbyteArray jdata, jint jsize,
        jlong jorderId, jlong jprice) {
    jbyte* data = (jbyte*)env->GetPrimitiveArrayCritical(jdata, NULL);
    if (data == NULL) return 0;

    /* 只保留计数器（供统计），不再 usleep */
    g_persist_counter.fetch_add(1);

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
