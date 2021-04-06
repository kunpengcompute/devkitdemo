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
#include <stdlib.h>
#include <stdint.h>
#include "ksw.h"


__m256i add_epi(unsigned int data1, unsigned int data2)
{
    __m256i i2561 = _mm256_set1_epi32(data1);
    __m256i i2562 = _mm256_set1_epi32(data2);
    __m256i result256 = _mm256_sub_epi32(i2561, i2562);
    return result256;
}

__m256i max_epi(unsigned int data1, unsigned int data2)
{
    __m256i i2561 = _mm256_set1_epi32(data1);
    __m256i i2562 = _mm256_set1_epi32(data2);
    __m256i result256 = _mm256_max_epi32(i2561, i2562);
    return result256;
}
