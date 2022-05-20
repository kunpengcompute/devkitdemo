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
 * Description: secret_vote_ca.h
 * Create: 2022-05-10
 */
#ifndef __SECRET_VOTE_CA_H
#define __SECRET_VOTE_CA_H

#define TEEC_DEBUG

#define PARAMS_IDX0                0            // Subscript index 0 of the params parameter.
#define PARAMS_IDX1                1            // Subscript index 1 of the params parameter.
#define PARAMS_IDX2                2            // Subscript index 2 of the params parameter.
#define PARAMS_IDX3                3            // Subscript index 3 of the params parameter.

#ifndef SECRET_VOTE_TA_UUID
    #define SECRET_VOTE_TA_UUID    "53648059-2257-485e-a940-2482b65a21da"    // Developer certificate UUID. [Change it based on the site requirements.]
#endif

#define SECRET_VOTE_TA_DIR_NAME    "/data/"
#define SECRET_VOTE_TA_SUFFIX      ".sec"

#define SUCCESS                    0
#define GENERIC_ERROR              1
#define CRYPTO_SUCCESS             1
#define CRYPTO_ERROR               0

#define AES_KEY_LEN                32
#define AES_NONCE_LEN              12
#define AES_AAD_LEN                16
#define AES_TAG_LEN                16

#define MAX_PEM_PUBKEY_LEN         1024
#define RSA_KEY_BITS               2048
#define RSA_KEY_SIZE               256

#define VOTE_DATA_LEN              32

typedef enum {
    CMD_CREATE_ROOT_KEYPAIR = 1,
    CMD_CREATE_USER_KEYPAIR,
    CMD_AES_KEY_EXCHANGE,
    CMD_ENCRYPT_VOTE
} SecretVoteCMD;

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

struct PubkeyParam {
    uint8_t modulusBuffer[RSA_KEY_SIZE];
    size_t modulusBufferLen;
    uint8_t pubexpBuffer[RSA_KEY_SIZE];
    size_t pubexpBufferLen;
};

struct UserPubkey {
    char *username;
    size_t usernameLen;
    char *pubkeyPath;
    size_t pubkeyPathLen;
    char *sign;
    size_t signLen;
};

struct VoteInfo {
    char *voteData;
    size_t voteDataLen;
    char *voteRes;
    size_t *voteResLen;
};

int CreateRootPubKey(const char *path, const size_t pathLen);

int CreateTaskPubKey(struct UserPubkey *userPubkey);

int Vote(const char *rootPubkeyPath, const size_t rootPubkeyPathLen,
         struct UserPubkey *userPubkey, struct VoteInfo *voteInfo);

#endif
