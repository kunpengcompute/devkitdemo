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

// gcc -g pthread_mutex_long.c -o pthread_mutex_long -lpthread -lm
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/timeb.h>
#include <math.h>

#define N    100000

static long num = 1;
static long count = 100000000;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

long long getSystemTime()
{
    struct timeb t;
    ftime(&t);
    return 1000 * t.time + t.millitm;
}

void* fun(void *arg)
{
    int i = 1;
    for (; i <= count; ++i) {
        pthread_mutex_lock(&mutex);
        num += 1;

        float s = 0;
        int j;
        for (j = 0; j < N; j++) {
            s += sqrt(j);
        }

        printf("tid = %u, num = %d\n", pthread_self(), num);
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    int err;
    pthread_t thread1;
    pthread_t thread2;

    long long start = getSystemTime();

    //create thread
    err = pthread_create(&thread1, NULL, fun, NULL);
    if (err != 0) {
        perror("can't create thread1\n");
        exit(EXIT_FAILURE);
    }

    err = pthread_create(&thread2, NULL, fun, NULL);
    if (err != 0) {
        perror("can't create thread2\n");
        exit(EXIT_FAILURE);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    long long end = getSystemTime();

    printf("The num is %d, pay %lld ms\n", num, (end - start));

    return 0;
}