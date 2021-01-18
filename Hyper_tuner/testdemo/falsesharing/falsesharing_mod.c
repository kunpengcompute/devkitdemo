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

// gcc falsesharing_mod.c -lpthread -D _GNU_SOURCE -o falsesharing_mod -g
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdalign.h>
#include <sched.h>
#include <ctype.h>
#include <string.h>

#define TIME_S    2000000000
#define NUM_THREADS    4

struct Foo {
    alignas(128) long x;
    alignas(128) long y;
};

static struct Foo f;
static struct Foo testf;

void *inc_b1(void *arg)
{
    int i;
    cpu_set_t mask;
    cpu_set_t get;
    int *threadNum = (int *)arg;
    CPU_ZERO(&mask);
    CPU_SET(*threadNum, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
        printf("warning: could not set CPU affinity, continuing...\n");
    }
    CPU_ZERO(&get);
    if (sched_getaffinity(0, sizeof(get), &get) == -1) {
        printf("warning: could not get thread affinity, continuing...\n");
    }
    if (CPU_ISSET(*threadNum, &get)) {
        printf("inc_b1 is running in %d\n", get);
    }
    for (i = 0; i < TIME_S; ++i) {
        ++testf.y;
    }
}

void *sum_a1(void *arg)
{
    int i;
    cpu_set_t mask;
    cpu_set_t get;
    int *threadNum = (int *)arg;
    CPU_ZERO(&mask);
    CPU_SET(*threadNum, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
        printf("warning: could not set CPU affinity, continuing...\n");
    }
    CPU_ZERO(&get);
    if (sched_getaffinity(0, sizeof(get), &get) == -1) {
        printf("warning: could not get thread affinity, continuing...\n");
    }
    if (CPU_ISSET(*threadNum, &get)) {
        printf("sum_a1 is running in %d\n", get);
    }
    int s = 0;
    for (i = 0; i < TIME_S; ++i) {
        s += testf.x;
    }
}

void *sum_a(void *arg)
{
    int i;
    cpu_set_t mask;
    cpu_set_t get;
    int *threadNum = (int *)arg;
    CPU_ZERO(&mask);
    CPU_SET(*threadNum, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
        printf("warning: could not set CPU affinity, continuing...\n");
    }
    CPU_ZERO(&get);
    if (sched_getaffinity(0, sizeof(get), &get) == -1) {
        printf("warning: could not get thread affinity, continuing...\n");
    }
    if (CPU_ISSET(*threadNum, &get)) {
        printf("sum_a is running in %d\n", get);
    }
    int s = 0;
    for (i = 0; i < TIME_S; ++i) {
        s += f.x;
    }
}

void *inc_b(void *arg)
{
    int i;
    cpu_set_t mask;
    cpu_set_t get;
    int *threadNum = (int *)arg;
    CPU_ZERO(&mask);
    CPU_SET(*threadNum, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
        printf("warning: could not set CPU affinity, continuing...\n");
    }
    CPU_ZERO(&get);
    if (sched_getaffinity(0, sizeof(get), &get) == -1) {
        printf("warning: could not get thread affinity, continuing...\n");
    }
    if (CPU_ISSET(*threadNum, &get)) {
        printf("inc_b is running in %d\n", get);
    }
    for (i = 0; i < TIME_S; ++i) {
        ++f.y;
    }
}

int main()
{
    int ret = 0;
    int i;
    int tid[NUM_THREADS];
    pthread_t tids[NUM_THREADS];
    printf("start the threads\n");

    for (i = 0; i < NUM_THREADS; i++) {
        tid[i] = i;
    }

    ret = pthread_create(&tids[0], NULL, sum_a, (void*)&tid[0]);
    if (ret != 0) {
        printf("pthread_create sum_a error: error_code = %d\n", ret);
        exit(EXIT_FAILURE);
    }
    ret = pthread_create(&tids[1], NULL, inc_b, (void*)&tid[1]);
    if (ret != 0) {
        printf("pthread_create inc_b error: error_code = %d\n", ret);
        exit(EXIT_FAILURE);
    }
    ret = pthread_create(&tids[2], NULL, sum_a1, (void*)&tid[2]);
    if (ret != 0) {
        printf("pthread_create sum_a1 error: error_code = %d\n", ret);
        exit(EXIT_FAILURE);
    }
    ret = pthread_create(&tids[3], NULL, inc_b1, (void*)&tid[3]);
    if (ret != 0) {
        printf("pthread_create inc_b1 error: error_code = %d\n", ret);
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(tids[i], NULL);
    }

    return 0;
}