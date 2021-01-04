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
数组访问， Cache Hit
gcc -g cache_hit.c -o cache_hit
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ROW (10 * 1024)
#define COL (10 * 1024)
#define N   (ROW * COL)


int main(void)
{
    int i, j;
    int *p = (int*)malloc(sizeof(int) * N);
    if (p == NULL) {
        perror("Memory allocation through malloc failed");
        exit(EXIT_FAILURE);
    }
    double msecs;
    struct timeval before, after;

    gettimeofday(&before, NULL);

    for (j = 0; j < ROW; j++) {
        for (i = 0; i < COL; i++) {
            p[i + j * COL] = 'A';
        }
    }

    gettimeofday(&after, NULL);

    // 1000 ia used to convert tv_sec and tv_usec to msec.
    msecs = (after.tv_sec - before.tv_sec) * 1000.0 + (after.tv_usec - before.tv_usec) / 1000.0;
    printf("Execution time = %2.3lf ms\n", msecs);

    free(p);
    return 0;
}