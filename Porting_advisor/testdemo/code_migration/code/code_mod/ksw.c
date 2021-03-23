#if defined __aarch64__
    #include "avx2neon.h"
    //suggestion: visit 'https://github.com/kunpengcomputer/AvxToneon' and obtain the 'avx2neon.h' source code according to the README.md file.
#endif
#include <stdlib.h>
#include <stdint.h>
#include "ksw.h"


__m128i add_epi(unsigned int data1, unsigned int data2)
{
    __m128i i1281 = _mm_set1_epi16(data1);
    __m128i i1282 = _mm_set1_epi16(data2);
    __m128i result128 = _mm_sub_epi16(i1281, i1282);
    return result128;
}

__m128i max_epi(unsigned int data1, unsigned int data2)
{
    __m128i i1281 = _mm_set1_epi16(data1);
    __m128i i1282 = _mm_set1_epi16(data2);
    __m128i result128 = _mm_max_epi16(i1281, i1282);
    return result128;
}
