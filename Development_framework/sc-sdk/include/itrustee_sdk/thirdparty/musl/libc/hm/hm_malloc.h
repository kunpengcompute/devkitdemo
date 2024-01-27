#ifndef HM_MALLOC_H
#define HM_MALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*free_hook_fun)(void *, void *);
void set_free_heap_hook(free_hook_fun fn);
void use_bigheap_policy(void);
int shrink(void);

#ifdef __cplusplus
}
#endif

#endif
