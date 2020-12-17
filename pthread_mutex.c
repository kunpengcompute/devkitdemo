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

// gcc -g pthread_mutex.c -o pthread_mutex -lpthread
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/timeb.h>

static long num = 1;
static long count = 10000000000;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void Perror(const char *s)
{
    perror(s);
    exit(EXIT_FAILURE);
}

long long getSystemTime()
{
    struct timeb t;
    ftime(&t);
    return 1000 * t.time + t.millitm;
}

void* fun2(void *arg)
{
    pthread_t thread_id = pthread_self();
    printf("The thread2 id is %ld\n", (long)thread_id);
    int i = 1;
    for (; i <= count; ++i) {
        pthread_mutex_lock(&mutex);
        num++;
        num *= 2;
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    int err;
    pthread_t thread1;
    pthread_t thread2;

    thread1 = pthread_self();
    printf("The thread1 id is %ld\n", (long)thread1)

    long long start = getSystemTime();

    //create thread
    err = pthread_create(&thread2, NULL, fun2, NULL);
    if (err != 0) {
        Perror("can't create thread2\n");
    }

    int i = 1;
    for (; i <= count; ++i) {
        pthread_mutex_lock(&mutex);
        num++;
        num *= 2;
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_join(thread2, NULL);
    long long end = getSystemTime();

    printf("The num is %d, pay %lld ms\n", num, (end - start));

    return 0;
}