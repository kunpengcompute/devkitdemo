#ifndef __DEMO_H_
#define __DEMO_H_
#if defined __x86_64__
    #include <emmintrin.h>
#elif defined __aarch64__
    #include "avx2neon.h"
#endif


__m128i add_epi(unsigned int data1, unsigned int data2);
__m128i max_epi(unsigned int data1, unsigned int data2);

#endif