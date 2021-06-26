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
#include <unistd.h>
#include <thread>
#include <stdio.h>
#include <atomic>
using namespace std;

int r1, r2;

std::atomic<int> x, y;

void* thread0(void*)
{
    r2 = x.load(std::memory_order_acquire); // here
    r1 = y.load(std::memory_order_acquire); // here
    return nullptr;
}

void* thread1(void*)
{
    y.fetch_add(1, std::memory_order_release); // here
    x.fetch_xor(1, std::memory_order_relaxed); // here
    return nullptr;
}

int main()
{
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread0, NULL);
    pthread_create(&t2, NULL, thread1, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}