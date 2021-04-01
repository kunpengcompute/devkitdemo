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
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include "ksw.h"

#define TEST_NUM 10000000

static int g_count = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* 存储大小端交换 */
unsigned int swap_big_endian(unsigned int data)
{
    unsigned int result = 0;
    /* 汇编倒序指令 */
    __asm__("bswap %0" : "=r" (result) : "0" (data));
    return result;
}

long int multi_inst(long int data1, long int data2)
{
    long int result = 0;
    __asm__ __volatile("nop\n\t"
    "movq %[data1], %%rax\n\t"
    "movl $4, %%ecx\n\t"
    "addq %%rax, %%rax\n\t"
    "orq %[data2], %%rax\n"
    "movq %%rax, %0\n"
    :"=r"(result)
    :[data1]"r"(data1), [data2]"r"(data2)
    :"rax", "ecx", "cc", "memory"
    );
    return result;
}

void inst_function_test_lock_inc(signed char V0)
{
    asm volatile(
        "lock ; incb %[cnt] \n\t"
        : [cnt] "+m"(V0)
        :
    );
}

/* 对全局变量g_count进行加操作 */
void* inc_count(void *arg)
{
    int i = 0;
    pthread_t thread_id = pthread_self();
    printf("the thread1 id is %ld\n", (long)thread_id);

    for(; i<=TEST_NUM; i++) {
        pthread_mutex_lock(&mutex);
        g_count += 1;
        pthread_mutex_unlock(&mutex);
    }
}

/* 对全局变量g_count进行减操作 */
void* dec_count(void *arg)
{
    int i = 0;
    pthread_t thread_id = pthread_self();
    printf("the thread2 id is %ld\n", (long)thread_id);

    for(; i<=TEST_NUM; i++) {
        pthread_mutex_lock(&mutex);
        g_count -= 1;
        pthread_mutex_unlock(&mutex);
    }
}

/* 程序功能 : 将data倒序存放，创建两个线程对全局变量g_count进行操作，将倒序存放的data和g_count比较 */
int main()
{
    int ret;
    int *p;
    pthread_t thread1;
    pthread_t thread2;
    unsigned int data = 0x12345678;
    unsigned int result1, result2;
    unsigned int out;
    int n, t;

    for (n = 1; n <= 20000000; n = n +1)
    {
        t += 1;
    }

    /* 计算a + a | b */
    result1 = multi_inst(0x2, 5);
    /* 数据翻转 */
    result2 = swap_big_endian(data);
    /* 向量加 */
    __m256i sumVec = add_epi(result1, result2);
    uint32_t *valVec = (uint32_t*)&sumVec;
    printf("sum vec: %x %x %x %x \n",
        valVec[0], valVec[1], valVec[2], valVec[3]);

    /* Create thread1 */
    ret = pthread_create(&thread1, NULL, inc_count, NULL);
    if (ret != 0 ) {
        printf("can't createthread1\n");
        return -1;
    }

    /* Create thread2 */
    ret = pthread_create(&thread2, NULL, dec_count, NULL);
    if (ret != 0 ) {
        printf("can't createthread2\n");
        return -1;
    }
    printf("get g_count=%d\n", g_count);
    
    /* show kunpeng plugin identified optimizerable functions in ARM or kunpeng chip */
    p = memcpy(&out, &result2, sizeof(int));
    if (p == NULL) {
        printf("memcpy failed\n");
        return -1;
    }
    printf("copy result to out=%x\n", out);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("destory thread to get g_count=%d\n", g_count);

    return 0;
}
