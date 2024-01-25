/*******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 * Description: Part of KML library
 * Author: KML
 * Create: 2022
 ******************************************************************************/
#ifndef KML_SERVICE_INTERNAL_ATTRIBUTES
#define KML_SERVICE_INTERNAL_ATTRIBUTES

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KML_ATTR_MALLOC
#define KML_ATTR_MALLOC __attribute__((malloc))
#endif

#ifndef KML_ATTR_WEAK
#define KML_ATTR_WEAK __attribute__((weak))
#endif

#ifndef KML_ATTR_PUBLIC
#define KML_ATTR_PUBLIC __attribute__((visibility("default")))
#endif

#ifndef KML_ATTR_ALLOC_SIZE
#define KML_ATTR_ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#endif

#ifndef KML_ATTR_UNUSED
#define KML_ATTR_UNUSED __attribute__((unused))
#endif

#ifndef KML_ATTR_ALWAYS_INLINE
#define KML_ATTR_ALWAYS_INLINE __attribute__((always_inline))
#endif

#ifndef KML_ATTR_NO_INLINE
#define KML_ATTR_NO_INLINE __attribute__((noinline))
#endif

#ifndef KML_ATTR_CONSTRUCTOR
#define KML_ATTR_CONSTRUCTOR(x) __attribute__((constructor((x))))
#endif

#ifndef KML_ATTR_DESTRUCTOR
#define KML_ATTR_DESTRUCTOR __attribute__((destructor))
#endif

#ifndef KML_FALLTHROUGH
#define KML_FALLTHROUGH __attribute__((fallthrough))
#endif

#ifndef KML_ATTR_ALIGNED
#define KML_ATTR_ALIGNED(x) __attribute__((aligned((x))))
#endif

#ifndef KML_ATTR_RESTRICT
#define KML_ATTR_RESTRICT __restrict__
#endif

#ifndef KML_THREAD_LOCAL
#define KML_THREAD_LOCAL __thread
#endif

#ifndef LIKELY
#define LIKELY(x) __builtin_expect(!!(x), 1)
#endif

#ifndef UNLIKELY
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#endif

#ifndef SELDOM
#define SELDOM UNLIKELY
#endif

#ifndef OFTEN
#define OFTEN LIKELY
#endif

#ifdef __cplusplus
}
#endif

#endif // #ifndef KML_SERVICE_INTERNAL_ATTRIBUTES
