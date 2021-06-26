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

int gcd(int a, int b)
{
    int result;
    __asm__ __volatile__(
        "CONTD: cmpl $0, %%ebx\n\t"
        "je END\n\t"
        "xorl %%edx, %%edx\n\t"
        "divl %%ebx\n\t"
        "movl %%ebx, %%eax\n\t"
        "movl %%edx, %%ebx\n\t"
        "jmp CONTD\n\t"
        "END: movl %%eax, %0\n\t"
        : "=r" (result)
        : "a"(a), "b"(b)
    );
    return result;
}

int Gcd(int a, int b)
{
    if (b == 0) {
        return a;
    }
    return Gcd(b, a % b);
}

int main()
{
    printf("Enter two integers :\n45, 66\n");
    int first = 45;
    int second = 66;
    printf("GCD of %d & %d is %d by function Gcd\n", first, second, Gcd(first, second));
    printf("GCD of %d & %d is %d by function gcd\n", first, second, gcd(first, second));
    return 0;
}