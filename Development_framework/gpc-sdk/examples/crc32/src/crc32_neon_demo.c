/*
 * Copyright 2022 Huawei Technologies Co., Ltd
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
 * Description: crc32_neon_demo.c
 * Create: 2022-07-20
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <arm_acle.h>
#include <arm_neon.h>

#include "crc32_neon_demo.h"


unsigned int NeonCrc32(unsigned int crcInit, unsigned char *s, unsigned int n)
{
    register uint32_t crcResult;
    register const uint8_t *buf1 = NULL;
    register const uint16_t *buf2 = NULL;
    register const uint32_t *buf4 = NULL;
    register const uint64_t *buf8 = NULL;
    int64_t length = (int64_t)n;
    crcResult = crcInit ^ 0xffffffffUL;

    buf8 = (const uint64_t *)(const void *)s;
    while (length >= sizeof(uint64_t)) {
        crcResult = __crc32cd(crcResult, *buf8++);
        length -= sizeof(uint64_t);
    }

    buf4 = (const uint32_t *)(const void *)buf8;
    if (length >= sizeof(uint32_t)) {
        crcResult = __crc32cw(crcResult, *buf4++);
        length -= sizeof(uint32_t);
    }

    buf2 = (const uint16_t *)(const void *)buf4;
    if (length >= sizeof(uint16_t)) {
        crcResult = __crc32ch(crcResult, *buf2++);
        length -= sizeof(uint16_t);
    }

    buf1 = (const uint8_t *)(const void *)buf2;
    if (length >= sizeof(uint8_t)) {
        crcResult = __crc32cb(crcResult, *buf1);
        length -= sizeof(uint8_t);
    }

    return (crcResult ^ 0xffffffffL);
}
