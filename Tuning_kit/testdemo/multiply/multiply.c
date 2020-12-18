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

/*
一维矩阵计算
gcc -g multiply.c -o multiply
*/

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>

#define N    1000000000
#define SEED    0x1234

float *a, *b, *c;

void gen_data(void)
{
    unsigned i;
    a = (float*) malloc(N * sizeof(float));
    b = (float*) malloc(N * sizeof(float));
    c = (float*) malloc(N * sizeof(float));
    
    for (i = 0; i < N; i++) {
        a[i] = b[i] = (float)(SEED * 0.1);
    }
}

void free_data(void)
{
    free(a);
    free(b);
    free(c);
}

void multiply(void)
{
    unsigned i;
    for (i = 0; i < N; i++) {
        c[i] = a[i] * b[i];
    }
}

void print_data(void)
{
    printf("%f, %f, %f, %f\n", c[0], c[1], c[N-2], c[N-1]);
}

int main(void)
{
    double start = 0.0, stop = 0.0, msecs;
    struct timeval before, after;

    gen_data();
    gettimeofday(&before, NULL);
    multiply();
    gettimeofday(&after, NULL);

    // 1000 is used to convert tv_sec and tv_usec to msec.
    msecs = (after.tv_sec - before.tv_sec) * 1000.0 + (after.tv_usec - before.tv_usec) / 1000.0;
    print_data();
    printf("Execution time = %2.3lf ms\n", msecs);

    free_data();
    return 0;
}