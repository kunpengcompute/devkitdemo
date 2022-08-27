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
 * Description: cert_assign_ca.h
 * Create: 2022-08-15
 */
#ifndef __CERT_ASSIGN_CA_H
#define __CERT_ASSIGN_CA_H

#define TA_VERSION                    "v1.0.0"
#define CLIENT_APPLICATION_NAME       "/vendor/bin/cert-assign"

#define PARAMS_IDX0                0            // Subscript index 0 of the params parameter.
#define PARAMS_IDX1                1            // Subscript index 1 of the params parameter.
#define PARAMS_IDX2                2            // Subscript index 2 of the params parameter.
#define PARAMS_IDX3                3            // Subscript index 3 of the params parameter.

#ifndef TA_UUID
    #define TA_UUID                "d17fa523-ab23-47fa-97b4-ede34acf748c"    // Developer certificate UUID. [Change it based on the site requirements.]
#endif
#define TA_DIR_NAME                "/data/"
#define TA_SUFFIX                  ".sec"

#define PATH_LEN                   1024
#define USERNAME_LEN               32
#define INPUT_LEN                  64
#define MAX_BUFFER_LEN             4096

#define BASE_STR    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

typedef enum {
    CMD_GET_ROOT_CERT_STATE = 1,
    CMD_CREAT_ROOT_CERT,
    CMD_SHOW_ROOT_CERT,
    CMD_SIGN_X509_CERT
} CERT_ASSIGN_CMD;

#endif
