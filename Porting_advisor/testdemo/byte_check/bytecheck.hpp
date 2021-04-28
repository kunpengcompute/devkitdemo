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

#include<vector>
using namespace std;


struct myStruct0{
    char value0_1;
    int value0_2;
    float value0_3;
    long value0_4;
    long int value0_5;
};

struct myStruct1{
    char value1_1;
    int value1_2;
    float value1_3;
};

struct myStruct2{
    short value2_1;
    int value2_2;
    long double value2_3;
    struct myStruct1 value2_4;
    struct myStruct1 value2_5;
    vector<char> value2_6;
};

typedef struct myStruct3{
    char value3_1;
    int value3_2;
    float value3_3;
    struct myStruct2 value3_4;
    vector<char> value3_5;
};
