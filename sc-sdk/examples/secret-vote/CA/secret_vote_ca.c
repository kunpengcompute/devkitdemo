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
 * Description: secret_vote_ca.c
 * Create: 2022-05-10
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/pem.h>

#include "securec.h"
#include "tee_client_api.h"
#include "secret_vote_ca.h"

TEEC_Context g_context;
TEEC_Session g_session;

#define PRINTF_SIZE 8

void DumpBuff(const char *buffer, size_t bufLen)
{
    #ifdef TEEC_DEBUG
    size_t i;
    if (buffer == NULL || bufLen == 0) {
        return;
    }

    printf("-----------------------\n");
    printf("bufLen = %d", bufLen);
    for (i = 0; i < bufLen; i++) {
        if (i % PRINTF_SIZE == 0) {
            printf("\n");
        }
        printf("%02x ", *(buffer + i));
    }
    printf("\n-----------------------\n\n");
    #endif
    return;
}

static errno_t HexStrToLong(const char* src, int count, long int* res)
{
    char dest[9];
    char* endptr;
    errno_t err;
    err = strncpy_s(dest, 9, src, count);
    if (err != EOK)
    {
        return err;
    }
    *res = strtol(dest, &endptr, 16);
    return EOK;
}

static errno_t GetUUID(TEEC_UUID* taUUID)
{
    errno_t err;
    long int res;
    int i;
    char* src = SECRET_VOTE_TA_UUID;
    err = HexStrToLong(src, 8, &res);
    if (err != EOK)
    {
        return err;
    }
    taUUID->timeLow = (uint32_t)res;

    err = HexStrToLong(src + 9, 4, &res);
    if (err != EOK)
    {
        return err;
    }
    taUUID->timeMid = (uint16_t)res;

    err = HexStrToLong(src + 14, 4, &res);
    if (err != EOK)
    {
        return err;
    }
    taUUID->timeHiAndVersion = (uint16_t)res;
    for (i = 0; i < 2; i++)
    {
        err = HexStrToLong(src + 19 + i * 2, 2, &res);
        if (err != EOK)
        {
            return err;
        }
        taUUID->clockSeqAndNode[i] = (uint8_t)res;
    }

    for (i = 0; i < 6; i++)
    {
        err = HexStrToLong(src + 24 + i * 2, 2, &res);
        if (err != EOK)
        {
            return err;
        }
        taUUID->clockSeqAndNode[i + 2] = (uint8_t)res;
    }
    return EOK;
}

/* Initialize context and opensession. */
static TEEC_Result TeecInit(char *username, size_t usernameLen)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin = 0;

    result = TEEC_InitializeContext(NULL, &g_context);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("TEEC initial failed.");
        return result;
    }
    TEEC_Debug("TEEC_InitializeContext OK.");

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_NONE,
        TEEC_NONE,
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_INPUT);
    operation.params[PARAMS_IDX2].tmpref.buffer = username;
    operation.params[PARAMS_IDX2].tmpref.size = usernameLen;

    char secretVoteTAPath[PATH_MAX] = {0};
    int ret = sprintf(secretVoteTAPath, "%s%s%s", SECRET_VOTE_TA_DIR_NAME, SECRET_VOTE_TA_UUID, SECRET_VOTE_TA_SUFFIX);
    if (ret <= 0) {
        printf("LOG: Failed to get TA path.\n");
        TEEC_FinalizeContext(&g_context);
        return TEEC_ERROR_GENERIC;
    }
    g_context.ta_path = (uint8_t*)secretVoteTAPath;

    TEEC_UUID secretVoteUUID;
    if (GetUUID(&secretVoteUUID) != EOK)
    {
        printf("LOG: Failed to get UUID.\n");
        TEEC_FinalizeContext(&g_context);
        return TEEC_ERROR_GENERIC;
    }
    g_context.ta_path = (uint8_t*)secretVoteTAPath;

    result = TEEC_OpenSession(&g_context, &g_session, &secretVoteUUID, TEEC_LOGIN_IDENTIFY, NULL, &operation, &origin);
    if (result != TEEC_SUCCESS)
    {
        printf("LOG: Open session failed: result:%d, orgin: %d.\n", result, origin);
        TEEC_FinalizeContext(&g_context);
        return result;
    }
    TEEC_Debug("TEEC initialize context and open session success, session id: 0x%x, service id:0x%x, context 0x%x.",
        g_session.session_id, g_session.service_id, g_session.context);
    TEEC_Debug("TEEC_OpenSession OK.");
    return result;
}

static void TeecClose(void)
{
    TEEC_CloseSession(&g_session);
    TEEC_FinalizeContext(&g_context);
    TEEC_Debug("TEEC uninit OK.");
}

static int AESEncrypt(AESParam *param, char *data, size_t dataLen, AESOutput *output)
{
    printf("AESEncrypt.\n");
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        printf("Failed to init ctx.\n");
        return 1;
    }
    /* Set cipher type and mode */
    EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
    /* Set IV length if default 96 bits is not appropriate */
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, param->nonceLen, NULL);
    /* Initialise key and IV */
    EVP_EncryptInit_ex(ctx, NULL, NULL, param->key, param->nonce);
    /* Zero or more calls to specify any AAD */
    EVP_EncryptUpdate(ctx, NULL, &(output->cipherLen), param->aad, param->aadLen);
    /* Encrypt plaintext */
    EVP_EncryptUpdate(ctx, output->cipher, &(output->cipherLen), data, dataLen);
    printf("data:\n");
    DumpBuff(data, dataLen);
    printf("cipher:\n");
    DumpBuff(output->cipher, output->cipherLen);
    int len = 0;
    EVP_EncryptFinal_ex(ctx, NULL, &len);
    /* Get tag */
    EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, output->tagLen, output->tag);
    printf("EVP_CTRL_GCM_GET_TAG:\n");
    DumpBuff(output->tag, output->tagLen);
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

TEEC_Result CreateRootKeyPair(char *modulusBuffer,size_t *modulusBufferLen, char *pubexpBuffer, size_t *pubexpBufferLen)
{
    printf("GenerateRootKeyPair.\n");
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin = 0;
    operation.started = 1;
    operation.cancel_flag = 0;

    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[PARAMS_IDX0].tmpref.buffer = modulusBuffer;
    operation.params[PARAMS_IDX0].tmpref.size = *modulusBufferLen;
    operation.params[PARAMS_IDX1].tmpref.buffer = pubexpBuffer;
    operation.params[PARAMS_IDX1].tmpref.size = *pubexpBufferLen;

    result = TEEC_InvokeCommand(&g_session, CMD_CREATE_ROOT_KEYPAIR, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        printf("Failed to invoke command: CMD_CREATE_ROOT_KEYPAIR, result: 0x%x, origin: 0x%x.\n", result, origin);
        return result;
    }
    DumpBuff(modulusBuffer, operation.params[PARAMS_IDX0].tmpref.size);
    DumpBuff(pubexpBuffer, operation.params[PARAMS_IDX1].tmpref.size);
    *modulusBufferLen = operation.params[PARAMS_IDX0].tmpref.size;
    *pubexpBufferLen = operation.params[PARAMS_IDX1].tmpref.size;
    return TEEC_SUCCESS;
}

int CreateRootPubKey(const char *path, const size_t pathLen)
{
    if (path == NULL || pathLen ==0) {
        printf("params error.");
        return 1;
    }

    char username[] = "admin";
    int ret = 0;
    if (TEEC_SUCCESS != TeecInit(username, strlen(username))) {
        printf("Failed to TeecInit.\n");
        return GENERIC_ERROR;
    }

    char modulusBuffer[RSA_KEY_SIZE] = {0};
    char pubexpBuffer[RSA_KEY_SIZE] = {0};
    size_t modulusBufferLen = RSA_KEY_SIZE;
    size_t pubexpBufferLen = RSA_KEY_SIZE;
    if (CreateRootKeyPair(modulusBuffer, &modulusBufferLen, pubexpBuffer, &pubexpBufferLen) != TEEC_SUCCESS) {
        printf("Failed to CreateRootKeyPair.\n");
        TeecClose();
        return 1;
    }
    printf("modulusBufferLen: %d, pubexpBufferLen: %d\n", modulusBufferLen, pubexpBufferLen);

    RSA *rsa = RSA_new();
    BIGNUM *modulus = BN_bin2bn(modulusBuffer, modulusBufferLen, NULL);
    BIGNUM *pubexp = BN_bin2bn(pubexpBuffer, pubexpBufferLen, NULL);
    RSA_set0_key(rsa, modulus, pubexp, NULL);
    FILE *fp = fopen(path, "w");
    if (fp == NULL) {
        RSA_free(rsa);
        TeecClose();
        return 1;
    }
    if (PEM_write_RSA_PUBKEY(fp, rsa) == 0) {
        printf("Failed to write RSA pubkey.\n");
        ret = 1;
    }
    fclose(fp);
    RSA_free(rsa);
    TeecClose();
    return ret;
}

TEEC_Result CreateTaskKeyPair(char *username, size_t usernameLen,
                              char *modulusBuffer, size_t *modulusBufferLen,
                              char *pubexpBuffer, size_t *pubexpBufferLen,
                              char *sign, size_t signLen)
{
    TEEC_Debug("CreateTaskKeyPair.");
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin = 0;

    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_MEMREF_TEMP_INPUT);
    operation.params[PARAMS_IDX0].tmpref.buffer = modulusBuffer;
    operation.params[PARAMS_IDX0].tmpref.size = *modulusBufferLen;
    operation.params[PARAMS_IDX1].tmpref.buffer = pubexpBuffer;
    operation.params[PARAMS_IDX1].tmpref.size = *pubexpBufferLen;
    operation.params[PARAMS_IDX2].tmpref.buffer = sign;
    operation.params[PARAMS_IDX2].tmpref.size = signLen;
    operation.params[PARAMS_IDX3].tmpref.buffer = username;
    operation.params[PARAMS_IDX3].tmpref.size = usernameLen;

    result = TEEC_InvokeCommand(&g_session, CMD_CREATE_USER_KEYPAIR, &operation, &origin);
    if (result != TEEC_SUCCESS) 
    {
        printf("Failed to invoke command: CMD_CREATE_USER_KEYPAIR, result: 0x%x, origin: 0x%x.\n", result, origin);
        return result;
    }
    *modulusBufferLen = operation.params[PARAMS_IDX0].tmpref.size;
    *pubexpBufferLen = operation.params[PARAMS_IDX1].tmpref.size;
    printf("sign: \n");
    DumpBuff(operation.params[PARAMS_IDX2].tmpref.buffer, operation.params[PARAMS_IDX2].tmpref.size);
    return TEEC_SUCCESS;
}
 
int CreateTaskPubKey(struct UserPubkey *userPubkey)
{
    printf("CreateTaskPubKey\n");
    int ret = 0;
    if (TEEC_SUCCESS != TeecInit(userPubkey->username, userPubkey->usernameLen)) {
        printf("Failed to TeecInit.\n");
        return GENERIC_ERROR;
    }

    char modulusBuffer[RSA_KEY_SIZE] = {0};
    char pubexpBuffer[RSA_KEY_SIZE] = {0};
    size_t modulusBufferLen = RSA_KEY_SIZE;
    size_t pubexpBufferLen = RSA_KEY_SIZE;
    if (CreateTaskKeyPair(userPubkey->username, userPubkey->usernameLen,
                          modulusBuffer, &modulusBufferLen,
                          pubexpBuffer, &pubexpBufferLen,
                          userPubkey->sign, userPubkey->signLen) != TEEC_SUCCESS) {
        printf("Failed to GenerateRootKeyPair.\n");
        ret = 1;
    }

    RSA *rsa = RSA_new();
    BIGNUM *modulus = BN_bin2bn(modulusBuffer, modulusBufferLen, NULL);
    BIGNUM *pubexp = BN_bin2bn(pubexpBuffer, pubexpBufferLen, NULL);
    RSA_set0_key(rsa, modulus, pubexp, NULL);
    FILE *fp = fopen(userPubkey->pubkeyPath, "w");
    if (fp == NULL) {
        RSA_free(rsa);
        TeecClose();
        return 1;
    }
    if (PEM_write_RSA_PUBKEY(fp, rsa) == 0) {
        printf("Failed to write RSA pubkey.\n");
        ret = 1;
    }
    fclose(fp);
    RSA_free(rsa);
    TeecClose();
    return ret;
}

int VerifySign(RSA *rsa, const char *rootPubkeyPath, const size_t rootPubkeyPathLen, const char *sign, const size_t signLen)
{
    int ret;
    printf("rootPubkeyPath: %s\n", rootPubkeyPath);
    DumpBuff(sign, signLen);
    FILE *fp = fopen(rootPubkeyPath, "r");
    if (fp == NULL) {
        printf("Failed to open root pubkey.\n");
        return 0;
    }

    RSA *rootRSA = RSA_new();
    rootRSA = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
    if (rootRSA == NULL) {
        printf("Failed to read root pubkey.\n");
        return 0;
    }
    const BIGNUM *modulus;
    const BIGNUM *pubexp;
    RSA_get0_key(rsa, &modulus, &pubexp, NULL);
    struct PubkeyParam *pubkeyParam = (struct PubkeyParam *)calloc(1, sizeof(struct PubkeyParam));
    char modulusBuffer[RSA_KEY_SIZE] = {0};
    char pubexpBuffer[RSA_KEY_SIZE] = {0};
    size_t modulusBufferLen = BN_bn2bin(modulus, modulusBuffer);
    size_t pubexpBufferLen =  BN_bn2bin(pubexp, pubexpBuffer);

    memcpy_s(pubkeyParam->modulusBuffer, RSA_KEY_SIZE, modulusBuffer, modulusBufferLen);
    pubkeyParam->modulusBufferLen = modulusBufferLen;
    memcpy_s(pubkeyParam->pubexpBuffer, RSA_KEY_SIZE, pubexpBuffer, pubexpBufferLen);
    pubkeyParam->pubexpBufferLen = pubexpBufferLen;

    unsigned char msgDigest[SHA256_DIGEST_LENGTH] = {0};
    SHA256((void *)pubkeyParam, sizeof(struct PubkeyParam), msgDigest);

    ret = RSA_verify(NID_sha256, msgDigest, SHA256_DIGEST_LENGTH, sign, signLen, rootRSA);
    if (ret == 1) {
        printf("verify ok.\n");
    } else {
        printf("verify fail.\n");
    }
    RSA_free(rootRSA);
    return ret;
}

TEEC_Result AESKeyExchange(char *username, size_t usernameLen, char *paramBuffer, size_t paramBufferLen)
{
    printf("AESKeyExchange.\n");
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin = 0;
    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[PARAMS_IDX0].tmpref.buffer = username;
    operation.params[PARAMS_IDX0].tmpref.size = usernameLen;
    operation.params[PARAMS_IDX1].tmpref.buffer = paramBuffer;
    operation.params[PARAMS_IDX1].tmpref.size = paramBufferLen;
    result = TEEC_InvokeCommand(&g_session, CMD_AES_KEY_EXCHANGE, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        printf("Failed to invoke command: CMD_AES_KEY_EXCHANGE, result: 0x%x, origin: 0x%x.\n", result, origin);
        return result;
    }
    return TEEC_SUCCESS;
}

TEEC_Result EncryptVote(char *username, size_t usernameLen, AESOutput *output, char *voteRes, size_t *voteResLen)
{
    printf("EncryptVote.\n");
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin = 0;

    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_OUTPUT);
    printf("output->cipher: \n");
    DumpBuff(output->cipher, output->cipherLen);
    printf("output->tag: \n");
    DumpBuff(output->tag, output->tagLen);
    operation.params[PARAMS_IDX0].tmpref.buffer = username;
    operation.params[PARAMS_IDX0].tmpref.size = usernameLen;
    operation.params[PARAMS_IDX1].tmpref.buffer = output->cipher;
    operation.params[PARAMS_IDX1].tmpref.size = output->cipherLen;
    operation.params[PARAMS_IDX2].tmpref.buffer = output->tag;
    operation.params[PARAMS_IDX2].tmpref.size = output->tagLen;
    operation.params[PARAMS_IDX3].tmpref.buffer = voteRes;
    operation.params[PARAMS_IDX3].tmpref.size = *voteResLen;

    result = TEEC_InvokeCommand(&g_session, CMD_ENCRYPT_VOTE, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        printf("Failed to invoke command: CMD_ENCRYPT_VOTE, result: 0x%x, origin: 0x%x.\n", result, origin);
        return result;
    }
    printf("voteRes: %s\n", operation.params[PARAMS_IDX3].tmpref.buffer);
    return TEEC_SUCCESS;
}

int Vote(const char *rootPubkeyPath, const size_t rootPubkeyPathLen,
         struct UserPubkey *userPubkey, struct VoteInfo *voteInfo)
{
    printf("Vote\n");
    int ret = 0;
    if (TEEC_SUCCESS != TeecInit(userPubkey->username, userPubkey->usernameLen)) {
        printf("Failed to TeecInit.\n");
        return ret;
    }

    printf("userPubkey->pubkeyPath: %s\n", userPubkey->pubkeyPath);
    FILE *fp = fopen(userPubkey->pubkeyPath, "r");
    if (fp == NULL) {
        printf("Failed to open pubkey.\n");
        TeecClose();
        return 1;
    }
    RSA *rsa = RSA_new();
    PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
    if (rsa == NULL) {
        printf("Failed to read pubkey.\n");
        TeecClose();
        return ret;
    }

    printf("VerifySign\n");
    ret = VerifySign(rsa, rootPubkeyPath, rootPubkeyPathLen, userPubkey->sign, userPubkey->signLen);
    if (ret != 1){
        printf("Failed to verify sign.\n");
        RSA_free(rsa);
        TeecClose();
        return ret;
    }

    char key[AES_KEY_LEN] = {0};
    RAND_priv_bytes(key, AES_KEY_LEN);
    printf("key\n");
    DumpBuff(key, AES_KEY_LEN);
    char nonce[AES_NONCE_LEN] = {0};
    RAND_priv_bytes(nonce, AES_NONCE_LEN);
    printf("nonce\n");
    DumpBuff(nonce, AES_NONCE_LEN);
    char aad[AES_AAD_LEN] = {0};
    RAND_priv_bytes(aad, AES_AAD_LEN);
    printf("aad\n");
    DumpBuff(aad, AES_AAD_LEN);
    AESParam *param = (AESParam *)calloc(1, sizeof(AESParam));
    memcpy_s(param->key, AES_KEY_LEN, key, AES_KEY_LEN);
    param->keyLen = AES_KEY_LEN;
    memcpy_s(param->nonce, AES_NONCE_LEN, nonce, AES_NONCE_LEN);
    param->nonceLen = AES_NONCE_LEN;
    memcpy_s(param->aad, AES_AAD_LEN, aad, AES_AAD_LEN);
    param->aadLen = AES_AAD_LEN;
    char cipherParam[RSA_KEY_SIZE] = {0};
    printf("RSA_public_encrypt\n");

    if (RSA_public_encrypt(sizeof(AESParam), (char *)param, cipherParam, rsa, RSA_PKCS1_OAEP_PADDING) <= 0) {
        printf("RSA_public_encrypt failed.\n");
        RSA_free(rsa);
        TeecClose();
        return TEEC_ERROR_GENERIC;
    }
    RSA_free(rsa);
    printf("cipherParam\n");
    DumpBuff(cipherParam, RSA_KEY_SIZE);

    printf("AESKeyExchange\n");
    ret = AESKeyExchange(userPubkey->username, userPubkey->usernameLen, cipherParam, RSA_KEY_SIZE);
    if (ret != TEEC_SUCCESS) {
        printf("Failed to AES key exchange.\n");
        TeecClose();
        return ret;
    }
    char *cipher = calloc(1, voteInfo->voteDataLen);
    char tag[AES_TAG_LEN] = {0};
    AESOutput *output = (AESOutput *)calloc(1, sizeof(AESOutput));
    output->cipher = cipher;
    output->cipherLen = voteInfo->voteDataLen;
    output->tag = tag;
    output->tagLen = AES_TAG_LEN;
    printf("AESEncrypt\n");

    AESEncrypt(param, voteInfo->voteData, voteInfo->voteDataLen, output);
    printf("output->cipher: \n");
    DumpBuff(output->cipher, output->cipherLen);

    printf("EncryptVote\n");
    ret = EncryptVote(userPubkey->username, userPubkey->usernameLen, output, voteInfo->voteRes, voteInfo->voteResLen);
    free(output);
    if (ret != TEEC_SUCCESS) {
        printf("Failed to encrypt vote.\n");
        free(cipher);
        TeecClose();
        return ret;
    }
    free(cipher);
    TeecClose();
    return ret;
}

int test(void)
{
    int ret;
    char rootPubkeyPath[] = "root_pubkey.pem";
    ret = CreateRootPubKey(rootPubkeyPath, strlen(rootPubkeyPath));
    if (ret != 0) {
        return ret;
    }

    char username[] = "user1";
    char pubkeyPath[] = "user1_pubkey.pem";
    char sign[RSA_KEY_SIZE] = {0};
    size_t signLen = RSA_KEY_SIZE;
    struct UserPubkey userPubkey = {username, strlen(username), pubkeyPath, strlen(pubkeyPath), sign, signLen};
    ret = CreateTaskPubKey(&userPubkey);
    if (ret != 0) {
        return ret;
    }
    DumpBuff(sign, RSA_KEY_SIZE);

    char voteRes[VOTE_DATA_LEN] = {0};
    size_t voteResLen = VOTE_DATA_LEN;
    char voteData[] = "4";
    struct VoteInfo voteInfo = {voteData, strlen(voteData), voteRes, &voteResLen};
    ret = Vote(rootPubkeyPath, strlen(rootPubkeyPath), &userPubkey, &voteInfo);
    if (ret != 0) {
        return ret;
    }
    printf("voteID: %s\n", voteData);
    printf("voteRes: %s\n", voteRes);
    return ret;
}

