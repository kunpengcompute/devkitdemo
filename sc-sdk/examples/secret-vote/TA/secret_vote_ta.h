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
 * Description: secret_vote_ta.h
 * Create: 2022-05-10
 */
#ifndef __SECRET_VOTE_TA_H
#define __SECRET_VOTE_TA_H

#include <stddef.h>
#include "tee_defines.h"

#define SECRET_VOTE_TA_VERSION     "v1.0.0"
#define PYTHON3_PATH               "/home/kunpeng/devkitdemo/sc-sdk/examples/secret-vote/venv/bin/python3"

#define PARAMS_IDX0                0            // Subscript index 0 of the params parameter.
#define PARAMS_IDX1                1            // Subscript index 1 of the params parameter.
#define PARAMS_IDX2                2            // Subscript index 2 of the params parameter.
#define PARAMS_IDX3                3            // Subscript index 3 of the params parameter.
#define PARAMS_SIZE                4            // Size of the params array.

#define SUCCESS                    0
#define GENERIC_ERROR              1

#define SEC_STORAGE_PATH_MAX       128
#define SEC_STORAGE_PATH           "sec_storage_data/secret_vote/"
#define ROOT_USERNAME              "root"
#define ROOT_RSA_KEY_PREFIX        "root_rsa_key"
#define USER_RSA_KEY_PREFIX        "user_rsa_key"
#define USER_AES_KEY_PREFIX        "user_aes_key"

#define AES_KEY_LEN                32
#define AES_NONCE_LEN              12
#define AES_AAD_LEN                16
#define AES_TAG_LEN                16

#define RSA_KEY_BITS               2048
#define RSA_KEY_SIZE               256
#define SHA256_SIZE                32

#define TEE_OBJECT_STORAGE_PRIVATE 0x00000001
#define TEE_DATA_FLAG_ACCESS_READ  0x00000001
#define TEE_DATA_FLAG_ACCESS_WRITE 0x00000002

typedef enum {
    CMD_CREATE_ROOT_KEYPAIR = 1,
    CMD_CREATE_USER_KEYPAIR,
    CMD_AES_KEY_EXCHANGE,
    CMD_ENCRYPT_VOTE,
} SecretVoteCMD;

struct User {
    uint32_t len;
    char name[32];
};

struct PubkeyParam {
    uint8_t modulusBuffer[RSA_KEY_SIZE];
    size_t modulusBufferLen;
    uint8_t pubexpBuffer[RSA_KEY_SIZE];
    size_t pubexpBufferLen;
};

typedef struct AESParam {
    char key[AES_KEY_LEN];
    size_t keyLen;
    char nonce[AES_NONCE_LEN];
    size_t nonceLen;
    char aad[AES_AAD_LEN];
    size_t aadLen;
} AESParam;

typedef struct AESOutput {
    char *cipher;
    size_t cipherLen;
    char *tag;
    size_t tagLen;
} AESOutput;

extern TEE_Result TEE_CreatePersistentObject(uint32_t storageID, const void* objectID, size_t objectIDLen,
                                             uint32_t flags, TEE_ObjectHandle attributes, 
                                             const void* initialData, size_t initialDataLen,
	                                         TEE_ObjectHandle* object);

extern TEE_Result TEE_OpenPersistentObject(uint32_t storageID, const void* objectID, size_t objectIDLen, 
                                           uint32_t flags, TEE_ObjectHandle* object);

extern TEE_Result TEE_ReadObjectData(TEE_ObjectHandle object, void* buffer, size_t size, uint32_t* count);

extern TEE_Result TEE_WriteObjectData(TEE_ObjectHandle object, const void* buffer, size_t size);

#endif
