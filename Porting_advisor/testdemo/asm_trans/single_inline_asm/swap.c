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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 存储大小端交换 */
unsigned int swap_big_endian(unsigned int data)
{
    unsigned int result;
    /* 汇编倒序指令 */
    __asm__("bswap %0" : "=r" (result) : "0" (data));

    return result;
}

int main()
{
    unsigned int data = 0x12345678;
    unsigned int result;

    result = swap_big_endian(data);
    printf("the result is 0x%x\n", result);
    return 0;
}