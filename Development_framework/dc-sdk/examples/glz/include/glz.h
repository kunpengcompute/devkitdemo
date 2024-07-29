/* *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 *
 *
 *
 * @file    glz.h
 * @brief  GLZ无损压缩函数
 *
 * @author
 * @version  v1.0
 * @see
 * @since    2020/11/20
 */

#ifndef _GLZ_H
#define _GLZ_H

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define TOKEN_BYTES         3
#define TOKEN_BITS          (8*(TOKEN_BYTES))
#define GLZ_SAFE_8_BYTES    8
#define GLZ_SAFE_16_BYTES   16
#define MAX_SRC_SIZE_LOG    31
#define MOVE_TWO            2
#define SRC_MAX_SIZE        0x7FffFFff
#define SIZE_FOR_CEPH_HDD   17

#if defined GLZ_DEBUG
#define GLZ_DEBUGLOG(l, ...) do { \
                    fprintf(stderr, __FILE__ ": "); \
                    fprintf(stderr, __VA_ARGS__); \
                    fprintf(stderr, " \n"); \
} while (0)
#else
#define GLZ_DEBUGLOG(l, ...)      {}    /* disabled */
#endif

#if defined(__x86_64__) || defined(__x86_64_) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#define GLZ_FOR_X86_64
#endif

#if defined(__INTEL__) || defined(__X86__) || defined(_X86_) || defined(_M_IX86) || defined(__THW_INTEL__)
#define GLZ_FOR_X86
#endif

#if defined(__aarch64__)
#define GLZ_FOR_AARCH64
#endif

#if defined(GLZ_FOR_X86) || defined(GLZ_FOR_X86_64)
#define GLZ_FOR_INTEL
#endif

#if defined(__GNUC__) && (__GNUC__>=4)
#define GLZ_WITH_GCC_INTRINSICS
#endif

#if defined(GLZ_FOR_INTEL) || defined(HCORE_FOR_ARM)
#include "xmmintrin.h" /* __m128i, etc */
#endif

#ifdef __aarch64__
#include <arm_neon.h>
#endif

#ifdef  __cplusplus
extern "C" {
#endif

#if defined(__cplusplus)
#elif !defined(__STDC__) || (defined(__STDC_VERSION__) && __STDC_VERSION__ < 199901L)
typedef enum { false = 0, true = 1 } bool;
#endif

#if defined(__GNUC__)
#define GLZ_UNUSED __attribute__((__unused__))
#else
#define GLZ_UNUSED
#endif

#ifdef FORCE_INLINE
#undef FORCE_INLINE
#endif

#ifdef INLINE
#undef INLINE
#endif

#ifdef NO_INLINE
#undef NO_INLINE
#endif

#ifdef FORCE_NOINLINE
#undef FORCE_NOINLINE
#endif

#if defined(__cplusplus) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)  /* C99 */
#if defined(__GNUC__) || defined(__clang__)
#define FORCE_INLINE static inline __attribute__((always_inline))
#define INLINE inline
#define NO_INLINE __attribute__((noinline))
#define FORCE_NOINLINE static __attribute__((__noinline__))
#else
#define FORCE_INLINE static inline
#define INLINE inline
#define NO_INLINE static
#define FORCE_NOINLINE static
#endif
#else
#define FORCE_INLINE static
#define INLINE
#define NO_INLINE static
#define FORCE_NOINLINE static
#endif

FORCE_INLINE unsigned MASK(unsigned n)
{
    return ((1U<<(n))-1U);
}

FORCE_INLINE unsigned MASK64(unsigned n)
{
    return ((1ULL<<(n))-1ULL);
}

FORCE_INLINE bool x_overlap_with_y(const uint8_t * const x_start, const uint8_t * const x_end,
    const uint8_t * const y_start, const uint8_t * const y_end)
{
    return (y_start <= x_start && x_start < y_end) || (x_start <= y_start && y_start < x_end);
}

#if defined GLZ_WITH_GCC_INTRINSICS
FORCE_INLINE unsigned glz_formatting_bit(unsigned x)
{
    return (unsigned)(__builtin_expect((x) == 0, false) ? 1 : 31 ^ (unsigned)__builtin_clz((unsigned)(x)));
}
#else
unsigned glz_formatting_bit(unsigned x);
#endif

GLZ_UNUSED FORCE_INLINE unsigned glz_format_mask2log_internal(unsigned x, unsigned log2)
{
    return (unsigned)((x + MASK64(log2)) & ~MASK64(log2));
}

GLZ_UNUSED FORCE_INLINE unsigned glz_format_mask2log(unsigned x)
{
    const unsigned format_x = glz_formatting_bit(x);
    return glz_format_mask2log_internal(x, (unsigned)format_x);
}

typedef enum {
    GLZ_ERROR_TYPE_NO_ERROR = 0,
    GLZ_ERROR_TYPE_WRITE_ERROR = -1,
    GLZ_ERROR_TYPE_READ_ERROR  = -2,
    GLZ_ERROR_TYPE_FAILED	   = -3,
} GLZ_Status;

#define GLZ_FALSE false
#define GLZ_NO_ERROR GLZ_ERROR_TYPE_NO_ERROR
#define GLZ_ERR_isFAILED GLZ_ERROR_TYPE_FAILED
#define GLZ_ERR_isREAD GLZ_ERROR_TYPE_READ_ERROR
#define GLZ_ERR_isWRITE GLZ_ERROR_TYPE_WRITE_ERROR
#define GLZ_ZERO 0

static int glz_copy8(uint8_t* dst, const uint8_t* src)
{
#ifdef __aarch64__
    vst1_u8((uint8_t*)dst, vld1_u8((const uint8_t*)src));
#else
    __builtin_memcpy(dst, src, sizeof(int64_t));
#endif
    return 0;
}

static uint32_t glz_read32(const uint8_t* ptr)
{
    uint32_t val;
    __builtin_memcpy(&val, ptr, sizeof(val));
    return val;
}
static uint64_t glz_read64(const uint8_t* ptr)
{
    uint64_t val;
    __builtin_memcpy(&val, ptr, sizeof(val));
    return val;
}

#if defined(GLZ_FOR_INTEL)
FORCE_INLINE void STORE_ALIGNED_FROM_UNALIGNED_16(uint8_t* dst, const uint8_t* src)
{
    _mm_store_si128((__m128i*)(dst), _mm_loadu_si128((__m128i const *)(src)));
}
#endif

FORCE_INLINE bool GLZ_COMPARE_4BYTES(const uint8_t* q, const uint8_t* r)
{
    return (glz_read32(q) == glz_read32(r));
}

FORCE_INLINE void glz_copy16(uint8_t* d, const uint8_t* s)
{
    glz_copy8((d), (s));
    glz_copy8((d) + sizeof(int64_t), (s) + sizeof(int64_t));
}

typedef enum { glz_raw, glz_rle, glz_hc, glz_fse } sequence_type;

typedef struct glz_encoder glz_encoder;

typedef enum {
    GLZ_FAST = 0,
    GLZ_HIGH_CR
} encode_model;

typedef enum {
    WITHOUT_DICT = 0,
    PRE_BLOCK_MODEL,
    EXT_DICT_MODEL,
    DICT_CTX_MODEL
} dict_model;

typedef enum {
    GLZ_STATUS_WRITE_ERROR = -1,
    GLZ_STATUS_READ_ERROR  = -2,
    GLZ_STATUS_FAILED      = -3,
} GLZ_HC_Status;

/* ++外部接口函数 */
glz_encoder *glz_compress_initial(long unsigned src_size);

void glz_compress_delete(glz_encoder **glz_state);

int glz_compress_frame_fast(glz_encoder * const glz_state,
                            const char * const src_buffer,
                            char *dst_buffer,
                            const dict_model dictInf,
                            const int GLZ_param_array[]);

int glz_decompress_frame_fast(const char *src_buffer,
                              char * const dst_buffer,
                              unsigned src_size,
                              unsigned dst_size);

size_t glz_decompress_frame_tight(const char *dst_buffer,
                                  unsigned dst_size,
                                  const char * const src_buffer,
                                  unsigned src_size,
                                  int64_t windowLog);

int glz_compress_default(const glz_encoder * const glz_encode_state,
                         char *dst_buffer,
                         unsigned dst_size,
                         const char * const src_buffer,
                         unsigned src_size,
                         const encode_model encodeInf,
                         int level);

int glz_decompress_default(const glz_encoder * const glz_decode_state,
                           char *dst_buffer,
                           unsigned dst_size,
                           const char * const src_buffer,
                           unsigned src_size,
                           const encode_model encodeInf);
/* --外部接口函数 */
#ifdef  __cplusplus
}
#endif
#endif /* _GLZ_H */
