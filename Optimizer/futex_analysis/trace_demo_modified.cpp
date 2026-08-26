#include <jni.h>

#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace {

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;
int g_outputFd = -1;
constexpr size_t OUTPUT_SIZE = 128;

struct ProcessContext {
    const uint8_t* input = nullptr;
    size_t inputSize = 0;
    int computeRounds = 0;
    int writeCount = 0;
    uint64_t value = 0;
    char output[OUTPUT_SIZE] {};
};

}

__attribute__((noinline))
uint64_t CpuTransform(const uint8_t* data, size_t size, int rounds)
{
    uint64_t result = 1469598103934665603ULL;

    for (size_t i = 0; i < size; ++i) {
        result ^= data[i];
        result *= 1099511628211ULL;
    }

    for (int i = 0; i < rounds; ++i) {
        result ^= result << 13;
        result ^= result >> 7;
        result ^= result << 17;
        result += static_cast<uint64_t>(i) * 31ULL;
        result ^= data[static_cast<size_t>(i) % size];
    }

    return result;
}

__attribute__((noinline))
void BuildOutput(uint64_t value, char* output, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        output[i] = static_cast<char>((value + i) & 0xff);
    }
}

__attribute__((noinline))
void FlushSmallWrites(const char* data, size_t size, int writeCount)
{
    for (int i = 0; i < writeCount; ++i) {
        ssize_t ret = write(g_outputFd, data, size);
        if (ret < 0) {
            std::fprintf(stderr, "write failed: %s\n", std::strerror(errno));
            return;
        }
    }
}

__attribute__((noinline))
void NativeIoStage(ProcessContext& ctx)
{
    FlushSmallWrites(ctx.output, sizeof(ctx.output), ctx.writeCount);
}

__attribute__((noinline))
uint64_t Process(const uint8_t* input, size_t inputSize, int computeRounds, int writeCount)
{
    ProcessContext ctx;
    ctx.input = input;
    ctx.inputSize = inputSize;
    ctx.computeRounds = computeRounds;
    ctx.writeCount = writeCount;

    // Step 1: CPU-intensive work runs without the global mutex.
    ctx.value = CpuTransform(ctx.input, ctx.inputSize, ctx.computeRounds);

    // Step 2: Build thread-local output without locking.
    BuildOutput(ctx.value, ctx.output, sizeof(ctx.output));

    // Step 3: Only protect the shared IO section.
    pthread_mutex_lock(&g_mutex);
    NativeIoStage(ctx);
    pthread_mutex_unlock(&g_mutex);

    return ctx.value;
}

extern "C"
JNIEXPORT jlong JNICALL Java_NativeBridge_process(JNIEnv* env, jclass, jbyteArray data,
                          jint computeRounds, jint writeCount)
{
    const jsize size = env->GetArrayLength(data);
    jbyte* bytes = env->GetByteArrayElements(data, nullptr);

    if (bytes == nullptr) {
        return 0;
    }

    uint64_t result = Process(
            reinterpret_cast<uint8_t*>(bytes),
            static_cast<size_t>(size),
            static_cast<int>(computeRounds),
            static_cast<int>(writeCount));

    env->ReleaseByteArrayElements(data, bytes, JNI_ABORT);
    return static_cast<jlong>(result);
}

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM*, void*)
{
    g_outputFd = open("/dev/null", O_WRONLY);
    if (g_outputFd < 0) {
        std::fprintf(stderr, "open /dev/null failed: %s\n", std::strerror(errno));
        return JNI_ERR;
    }

    return JNI_VERSION_1_8;
}

extern "C"
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM*, void*)
{
    if (g_outputFd >= 0) {
        close(g_outputFd);
        g_outputFd = -1;
    }
}