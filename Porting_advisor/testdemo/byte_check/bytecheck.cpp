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

#include<iostream>
#include<vector>
#include "bytecheck.hpp"
using namespace std;

/* 验证引入其他自定义文件内struct的使用 */
typedef struct myStruct4{
	char value4_1;
	int value4_2;
	float value4_3;
	int value4_4[5][6];
	char value4_5[2][5];
	struct myStruct2 value4_6;
	struct myStruct3 value4_7;
};

/* 验证位域不提示 */
struct myStruct5{
    int value5_1;
    int value5_2 : 1;
} *mystruct5;

typedef struct myStruct6{
    char value6_1[50];
    char calue6_2;
    struct myStruct5 mystruct5;
}temp6;

/* 验证__attribute__((aligned(16))) */
typedef struct myStruct7{
    char value7_1;
    int calue7_2;
}temp7;

typedef struct myStruct8{
    char value8_1;
    int value8_2;
} __attribute__((aligned(16))) mystruct8;

/*
1、 验证 #pragma pack(n)
2、验证 #pragma pack 嵌套 attribute
*/
#pragma pack(8)

typedef struct myStruct9{
    char value9_1;
    int value9_2;
}temp9;

typedef struct myStruct10{
    char value10_1;
    int value10_2;
} __attribute__((aligned(16))) mystruct10;

typedef struct myStruct11{
    char value11_1;
    int value11_2;
} __attribute__((packed));

#pragma pack()

/*
1、验证union
2、验证enum
*/
//union
union myUnion1{
    char value1_1;
    int value1_2;
    double value1_3;
};

union myUnion2{
    char value2_1;
    int value2_2;
    float value2_3;
    union myUnion1 value2_4;
};

//enum
enum myEnum1{
    MON1='A', TUE1, WED1, THU1
};

enum myEnum2{
    MON2=-1, TUE2, WED2, THU2=2, FRI2='a', SAT2=123456
};

struct myStruct12{
    enum myEnum1 my_enum1;
    enum myEnum2 my_enum2;
};

struct myStruct13{
    union myUnion1 my_union1;
    union myUnion2 my_union2;
};

int main(){
    cout << "验证自定义基本类型" << endl;
    struct myStruct0 tempStruct0;
    struct myStruct1 tempStruct1;

    cout << "验证嵌套使用struct" << endl;
    struct myStruct2 tempStruct2;
    struct myStruct3 tempStruct3;

    cout << "验证引入其他自定义文件内struct的使用" << endl;
    struct myStruct4 tempStruct4;

    cout << "验证位域不提示" << endl;
    struct myStruct5 tempStruct5;
    struct myStruct6 tempStruct6;
    
    cout << "验证__attribute__((aligned(16)))" << endl;
    struct myStruct7 tempStruct7;
    mystruct8 tempStruct8_1;

     cout << "验证 #pragma pack(n)&&#pragma pack 嵌套 attribute" << endl;
    struct myStruct9 tempStruct9;
    mystruct10 tempStruct10;
    struct myStruct11 mystruct11;

    cout << "验证union&&验证enum" << endl;
    struct myStruct12 tempStruct12;
    struct myStruct13 tempStruct13;

    return 0;
}
