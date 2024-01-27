#ifndef __THREAD_TYPES_H__
#define __THREAD_TYPES_H__

#include <posix_types.h>

typedef struct {
    cref_t thread;
    void *stack;
} hm_thread;

#endif
