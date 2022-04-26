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
 * Description: data-sealing-ca.h
 * Create: 2022-04-15
 */

#ifndef __DATA_SEALING_CA_H
#define __DATA_SEALING_CA_H

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

#define SECURE_STORAGE_INVALID_FD    (0xFFFFFFFF)

#define PARAMS_IDX0        0  //params参数的下标索引 0
#define PARAMS_IDX1        1  //params参数的下标索引 1
#define PARAMS_IDX2        2  //params参数的下标索引 2
#define PARAMS_IDX3        3  //params参数的下标索引 3

#define DATA_SEALING_TA_UUID        "7824610d-0e54-4448-a905-86fffeeff327"    //开发者证书UUID[需根据实际情况进行修改]
#define DATA_SEALING_TA_DIR_NAME    "/data/"
#define DATA_SEALING_TA_SUFFIX      ".sec"

#define DATA_SEALING_MODE_WRITE (TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_CREATE)

typedef enum
{
		CMD_DATA_SEALING_READ_FILE = 0X1,
		CMD_DATA_SEALING_WRITE_FILE,
} DataSealingCMD;

#define KEY_LENGTH_MAX    128    //key的最大字符数
#define FILE_SIZE_MAX     4096   //写入文件的最大字节数

#define BASE_STR    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

#endif