/*
 * Copyright 2022 Huawei Technologies Co., Ltd
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

#define PARAMS_IDX0        0  //Subscript index 0 of the params parameter.
#define PARAMS_IDX1        1  //Subscript index 1 of the params parameter.
#define PARAMS_IDX2        2  //Subscript index 2 of the params parameter.
#define PARAMS_IDX3        3  //Subscript index 3 of the params parameter.

#define DATA_SEALING_TA_UUID        "7824610d-0e54-4448-a905-86fffeeff327"    //Developer certificate UUID. [Change it based on the site requirements.]
#define DATA_SEALING_TA_DIR_NAME    "/data/"
#define DATA_SEALING_TA_SUFFIX      ".sec"

#define DATA_SEALING_MODE_WRITE (TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_CREATE)

typedef enum
{
	CMD_DATA_SEALING_READ_FILE = 0X1,
	CMD_DATA_SEALING_WRITE_FILE,
} DataSealingCMD;

#define KEY_LENGTH_MAX    128    //Maximum number of characters of the key.
#define FILE_SIZE_MAX     4096   //Maximum number of bytes written to the file.

#define BASE_STR    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

#endif