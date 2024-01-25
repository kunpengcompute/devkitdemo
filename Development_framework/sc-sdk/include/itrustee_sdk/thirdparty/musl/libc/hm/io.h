/*
 * @TAG(HUAWEI) // FIXME: this file needs rework
 */

#ifndef __LIBC_HM_IO_H_
#define __LIBC_HM_IO_H_

#include <stdint.h>
#include <sys/syscalls.h>

#ifdef __arm__
int __attribute__((noinline)) debug_call(uint64_t cap_ref, int64_t call_no, uint64_t arg1, uint64_t arg2);
#endif

static inline void hm_debug_putchar(char c)
{
    debug_call(0, KCALL_DEBUG_CMD_PUT_CHAR, (uint64_t)(uint8_t)c, 0);
}

static inline int hm_debug_getchar_noblock(void)
{
    return debug_call(0, KCALL_DEBUG_CMD_GET_CHAR, 0, 0);
}

static inline int hm_debug_putbytes(char *str, size_t str_len)
{
    return debug_call(0, KCALL_DEBUG_CMD_PUT_BYTES, ptr_to_uint64(str), str_len);
}

int putchar(int c);

int getchar(void);

int puts(const char *s);

int printf(const char *format, ...);

#endif /* __LIBC_HM_IO_H_ */
