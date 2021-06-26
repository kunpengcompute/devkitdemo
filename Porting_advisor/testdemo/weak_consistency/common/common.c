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
#include <pthread.h>
#include <stdio.h>

int x, y, r1, r2;

void thread0()
{
    x = 1;
    y = 1; // 扫描结果1
}

void thread1()
{
    r1 = y;
    r2 = x; // 扫描结果2
}

int main()
{
    {
        pthread_t t1, t2;
        x = y = r1 = r2 = 0;
        pthread_create(&t1, NULL, (void *)thread0, NULL);
        pthread_create(&t2, NULL, (void *)thread1, NULL);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        if (r1 == 1 && r2 == 0) {
            printf("error\n");
        }
    }
    return 0;
}