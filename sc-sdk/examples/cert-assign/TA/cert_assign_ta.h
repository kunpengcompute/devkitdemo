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
 * Description: cert_assign_ta.h
 * Create: 2022-08-15
 */
#ifndef __CERT_ASSIGN_TA_H
#define __CERT_ASSIGN_TA_H

#include <stddef.h>
#include "tee_defines.h"

#define TA_VERSION                    "v1.0.0"
#define CLIENT_APPLICATION_NAME       "/vendor/bin/cert-assign"

#define PARAMS_IDX0                0            // Subscript index 0 of the params parameter.
#define PARAMS_IDX1                1            // Subscript index 1 of the params parameter.
#define PARAMS_IDX2                2            // Subscript index 2 of the params parameter.
#define PARAMS_IDX3                3            // Subscript index 3 of the params parameter.
#define PARAMS_SIZE                4            // Size of the params array.

#define USERNAME_MAX               32
#define SALT_MAX                   32
#define HASHED_PASSWORD_MAX        32
#define HASH_ITER                  1000

#define SEC_STORAGE_PATH_MAX       128
#define SEC_STORAGE_PATH           "sec_storage_data/cert_assign/"
#define PEM_BUFFER_LEN             4096

#define TEE_OBJECT_STORAGE_PRIVATE 0x00000001
#define TEE_DATA_FLAG_ACCESS_READ  0x00000001
#define TEE_DATA_FLAG_ACCESS_WRITE 0x00000002

typedef enum {
    CMD_GET_ROOT_CERT_STATE = 1,
    CMD_CREAT_ROOT_CERT,
    CMD_SHOW_ROOT_CERT,
    CMD_SIGN_X509_CERT
} CERT_ASSIGN_CMD;

typedef struct {
    char *username;
    char *salt;
    char *hashed_password;
} User;

extern TEE_Result TEE_CreatePersistentObject(uint32_t storageID, const void* objectID, size_t objectIDLen,
                                             uint32_t flags, TEE_ObjectHandle attributes, 
                                             const void* initialData, size_t initialDataLen,
	                                         TEE_ObjectHandle* object);

extern TEE_Result TEE_OpenPersistentObject(uint32_t storageID, const void* objectID, size_t objectIDLen, 
                                           uint32_t flags, TEE_ObjectHandle* object);

extern TEE_Result TEE_ReadObjectData(TEE_ObjectHandle object, void* buffer, size_t size, uint32_t* count);

extern TEE_Result TEE_WriteObjectData(TEE_ObjectHandle object, const void* buffer, size_t size);

extern TEE_Result addcaller_ca_exec(const char *ca_name, const char *user_name);

#endif
