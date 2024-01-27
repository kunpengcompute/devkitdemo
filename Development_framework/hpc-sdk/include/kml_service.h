/*******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2022. All rights reserved.
 * Description: Part of KML library
 ******************************************************************************/
#ifndef LIBKSERVICE_KML_SERVICE_H
#define LIBKSERVICE_KML_SERVICE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kml_export.h"
#include "kml_attributes.h"

#ifdef kml_complex_float

#elif defined(__cplusplus)

#include <complex>
#define kml_complex_float std::complex<float>

#elif __STDC_VERSION__ >= 199901L && !defined(__STDC_NO_COMPLEX__)

#define kml_complex_float float _Complex

#else

typedef struct kml_complex_float {
    float real;
    float imag;
} kml_complex_float;

#define kml_complex_float kml_complex_float

#endif

#ifdef kml_complex_double

#elif defined(__cplusplus)

#include <complex>
#define kml_complex_double std::complex<double>

#elif __STDC_VERSION__ >= 199901L && !defined(__STDC_NO_COMPLEX__)

#define kml_complex_double double _Complex

#else

typedef struct kml_complex_double {
    double real;
    double imag;
} kml_complex_double;

#define kml_complex_double kml_complex_double

#endif

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus

template<class T>
struct KmlIsFloatingPoint : std::integral_constant<bool,
#ifdef __aarch64__
                                std::is_same<__fp16, typename std::remove_cv<T>::type>::value ||
#endif
                                    std::is_same<float, typename std::remove_cv<T>::type>::value ||
                                    std::is_same<double, typename std::remove_cv<T>::type>::value ||
                                    std::is_same<long double, typename std::remove_cv<T>::type>::value> {
};

template<class T, class U>
constexpr T KmlMin(T a, U b)
{
    static_assert(std::is_same<T, U>::value, "KmlMin doesn't allow implicit casts on arguments.");
    if constexpr (std::is_pointer_v<T>) {
        if (a == nullptr) {
            return a;
        }
        if (b == nullptr) {
            return b;
        }
        if constexpr (KmlIsFloatingPoint<T>::value) {
            if (std::isnan(*a)) {
                return a;
            }
        }
        return (*a < *b) ? a : b;
    } else {
        if constexpr (KmlIsFloatingPoint<T>::value) {
            if (std::isnan(a)) {
                return a;
            }
        }
        return (a < b) ? a : b;
    }
}

template<class T, class U>
constexpr T KmlMax(T a, U b)
{
    static_assert(std::is_same<T, U>::value, "KmlMax doesn't allow implicit casts on arguments.");
    if constexpr (KmlIsFloatingPoint<T>::value) {
        if (std::isnan(a)) {
            return a;
        }
    }
    return (a > b) ? a : b;
}

extern "C" {
#endif

#define KML_DEFAULT_ALLOCATE_ALIGN 128

#ifndef L1_CACHELINE_SIZE
#define L1_CACHELINE_SIZE 64
#endif

#ifndef L2_CACHELINE_SIZE
#define L2_CACHELINE_SIZE 64
#endif

#ifndef L3_CACHELINE_SIZE
#define L3_CACHELINE_SIZE 128
#endif

#ifndef KML_WAIL
#define KML_WAIL(why, ...)                                                   \
    do {                                                                     \
        KmlCannotContinue("%s:%i: " why, __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#endif

#ifndef KML_WAIL_NO_MEMORY
#define KML_WAIL_NO_MEMORY KML_WAIL("OUT OF MEMORY")
#endif

#define KML_VERSION_PART_LEN 100
struct KMLVersion {
    char componentName[KML_VERSION_PART_LEN];
    char componentVersion[KML_VERSION_PART_LEN];
    char softwareName[KML_VERSION_PART_LEN];
    char softwareVersion[KML_VERSION_PART_LEN];
    char productName[KML_VERSION_PART_LEN];
    char productVersion[KML_VERSION_PART_LEN];
    char componentAppendInfo[KML_VERSION_PART_LEN];
};
typedef struct KMLVersion KSERVICEVersion;

extern KML_EXPORT int KSERVICEGetVersion(KSERVICEVersion *ver);

enum KmlSetting {
    KML_VERBOSE,
    KML_NUM_THREADS,
    KML_THREAD_USING_POLICY,
    KML_CFMA_ACCELERATION,
    KML_CFMA_DRIVER_LOADED,
    KML_SKIP_ZEROS,
    KML_GEMM_TO_GEMM3M,
    KML_BITWISE_COMPATIBILITY,
    KML_FAST_IGNORE_DENORMS,
    KML_ALLOCATE_POLICY,
    KML_ALLOCATE_HUGEPAGE_SIZE,
    KML_PXGETRF_FORCE_CALU,
    KML_FAST_EIGENSOLVER,
    KML_TSQR_CHECK,
    SETTINGS_FLAG_COUNT
};

enum SettingsStates { SETTINGS_ENTRY_NOT_INITED, SETTINGS_ENTRY_INITED };

union KmlSettingValue {
    size_t sizetValue;
    double doubleValue;
    char *stringValue;
    int intValue;
};

struct KmlSettingEntry {
    union KmlSettingValue value;
    enum SettingsStates entryState;
};

enum KmlCfmaAccelerationOption {
    KML_CFMA_DISABLED = 0,
    KML_CFMA_ENABLED = 1,
    KML_CFMA_FORCE_ENABLED = 2,
};

enum KmlPxgetrfPivotingOption {
    KML_PXGETRF_FULLPIVOTING = 0,
    KML_PXGETRF_LOCALPIVOTING = 1,
    KML_PXGETRF_BLOCKPIVOTING = 2,
};

enum KmlEigensolverOption {
    KML_EIGENSOLVER_EXACT = 0,
    KML_EIGENSOLVER_FAST = 1,
};

enum KmlTsqrCheckOption {
    KML_TSQR_CHECK_DEFAULT = 0,
    KML_TSQR_CHECK_WEAK = 1,
    KML_TSQR_CHECK_OFF = 2,
};

enum KmlThreadUsingPolicyOption {
    KML_USE_ALL_THREADS = 0,
    KML_THRESHOLD_ENABLED = 1,
    KML_CROSSPOINTS_ENABLED = 2,
};

enum KmlSkipZerosOption {
    KML_BLAS_NO_ZERO_SKIP = 0,
    KML_BLAS_NO_INTRUSIVE_ZERO_SKIP = 1,
    KML_BLAS_ENFORCED_ZERO_SKIP = 2,
};

enum KmlGemmToGemM3MOption {
    KML_GEMM_TO_GEMM_3M_NEVER = 0,
    KML_GEMM_TO_GEMM_3M_OPTIMIZED = 1,
    KML_GEMM_TO_GEMM_3M_ALWAYS = 2,
};

enum KmlAllocatePolicy {
    KML_HUGEPAGES_NEVER = 0,
    KML_HUGEPAGES_ALWAYS = 1,
    KML_HUGEPAGES_AS_HEAP = 2,
    KML_COVERAGE_ALLOCATOR = 4,
};

enum KmlErrorT {
    KML_INTERNAL_ERROR = -413,
    KML_WORK_MEM_ERROR = -1313,
    KML_NULL_PTR_ERROR = 100000,
};

#ifndef CONCAT2
#define CONCAT2(a, b) CONCA_2(a, b)
#define CONCA_2(a, b) a##b
#endif

#ifndef __cplusplus

#define AUTOTYPE_MIN(a, b, _a, _b)       \
    ({                                   \
        __auto_type const _a = (a);      \
        __auto_type const _b = (b);      \
        (_a != _a || _a < _b) ? _a : _b; \
    })
#define KmlMin(a, b)                                                                                                   \
    ({                                                                                                                 \
        _Static_assert(__builtin_types_compatible_p(typeof((a)), typeof((b))), "KmlMin doesn't allow implicit casts"); \
        AUTOTYPE_MIN((a), (b), CONCAT2(_a, __COUNTER__), CONCAT2(_b, __COUNTER__));                                    \
    })

#define AUTOTYPE_MAX(a, b, _a, _b)       \
    ({                                   \
        __auto_type const _a = (a);      \
        __auto_type const _b = (b);      \
        (_a != _a || _a > _b) ? _a : _b; \
    })
#define KmlMax(a, b)                                                                                                   \
    ({                                                                                                                 \
        _Static_assert(__builtin_types_compatible_p(typeof((a)), typeof((b))), "KmlMax doesn't allow implicit casts"); \
        AUTOTYPE_MAX((a), (b), CONCAT2(_a, __COUNTER__), CONCAT2(_b, __COUNTER__));                                    \
    })

#endif

// settings.c
extern KML_EXPORT void KmlLoadSettings(const struct KmlSettingEntry *entries, size_t envtriesLen);
extern KML_EXPORT const struct KmlSettingEntry *KmlStoreSettings(void);
extern KML_EXPORT bool KmlVerbose(void);
extern KML_EXPORT void KmlSetSkipZeros(enum KmlSkipZerosOption);
extern KML_EXPORT enum KmlSkipZerosOption KmlGetSkipZeros(void);
extern KML_EXPORT enum KmlCfmaAccelerationOption KmlGetCfmaAccelerationOption(void);
extern KML_EXPORT void KmlSetGemmToGemm3m(enum KmlGemmToGemM3MOption);
extern KML_EXPORT enum KmlGemmToGemM3MOption KmlGetGemmToGemm3m(void);
extern KML_EXPORT void KmlSetThreadUsingPolicy(enum KmlThreadUsingPolicyOption);
extern KML_EXPORT enum KmlThreadUsingPolicyOption KmlGetThreadUsingPolicy(void);
extern KML_EXPORT enum KmlPxgetrfPivotingOption KmlGetPxgetrfPivotingOption(void);
extern KML_EXPORT enum KmlEigensolverOption KmlGetEigensolverOption(void);
extern KML_EXPORT enum KmlTsqrCheckOption KmlGetTsqrCheckOption(void);
extern KML_EXPORT const union KmlSettingValue *KmlGetFlag(enum KmlSetting flag);
extern KML_EXPORT union KmlSettingValue KmlGetFlagOrDefault(enum KmlSetting flag);
extern KML_EXPORT void KmlSetFlag(enum KmlSetting flag, union KmlSettingValue value);

// memory.c
extern KML_EXPORT void *KML_ATTR_MALLOC KML_ATTR_ALLOC_SIZE(1) KmlAllocate(size_t sz, size_t align);
extern KML_EXPORT void *KML_ATTR_MALLOC KML_ATTR_ALLOC_SIZE(1, 2) KmlCalloc(size_t num, size_t size, size_t align);
extern KML_EXPORT void KmlTypedFree(char *p);
extern KML_EXPORT void KmlFree(void *p);

// timer.c
extern KML_EXPORT double dsecnd(void);
extern KML_EXPORT float second(void);
extern KML_EXPORT float second_(void);
extern KML_EXPORT double dsecnd_(void);

// parallel.c
extern KML_EXPORT int KmlGetNumProcs(void);
extern KML_EXPORT int KmlGetMaxThreads(void);
extern KML_EXPORT void KmlSetNumThreads(int numThreads);

// utils.c
extern KML_EXPORT void KmlCannotContinue(const char *msg, ...);
extern KML_EXPORT void KmlInform(const char *msg, ...);
extern KML_EXPORT int Xerbla(const char *func, const int *param, int funcLength);
extern KML_EXPORT int xerbla_(const char *func, const int *param, int funcLength);

// version.c
extern KML_EXPORT const char *KmlGetBuildConfig(void);

#ifdef __cplusplus
}
#endif

#endif // #ifndef LIBKSERVICE_KMLSERVICE_H
