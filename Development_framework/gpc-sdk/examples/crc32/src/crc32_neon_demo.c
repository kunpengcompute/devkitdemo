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
#include <sys/time.h>
#include <arm_acle.h>
#include <arm_neon.h>


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

int main()
{
    unsigned char *s = NULL;
    unsigned int n = 0;
    unsigned int i;
    unsigned int seed;
    struct timeval time1, time2;

    printf("Please input test length:\n");
    scanf("%u", &n);
    if (n <= 0) {
        return -1;
    }
    s = (unsigned char *)malloc(n);
    if (s == NULL) {
        return -1;
    }
    printf("Please input random seed:\n");
    scanf("%u", &seed);
    srand(seed);
    for (i = 0; i < n; i++) {
        s[i] = (unsigned char)(rand() % 256u);
    }

    printf("Test random string len:%d\n", n);
    printf("First 10 byte:");
    for (i = 0; i < 10; i++) {
        printf("[%u] ", s[i]);
    }

    printf("\nNeon crc calc:\n");
    gettimeofday(&time1, NULL);
    unsigned int neonCrc = NeonCrc32(0, s, n);
    gettimeofday(&time2, NULL);
    printf("Neon crc:0x%x\n", neonCrc);
    printf("Time used: %ld ms\n", (time2.tv_sec - time1.tv_sec) * 1000 + (time2.tv_usec - time1.tv_usec) /1000);

    free(s);
    return 0;
}