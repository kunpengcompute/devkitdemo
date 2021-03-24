/*
 * Copyright 2020 Huawei Technologies Co., Ltd
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if defined __aarch64__
    #include "avx2neon.h"
    //Suggestion: visit 'https://github.com/kunpengcomputer/AvxToNeon' and obtain the 'avx2neon.h' source code according to the README.md file.
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
