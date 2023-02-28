/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: rsa-demo
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "tee_client_api.h"

/*-------------------------------------------------------------------*
 * This Demo is only for demonstrate how RSA Crypto APIs works.
 * CA should ONLY get touch with Encrypted data,
 * and Original message (Decrypt Data) shold ONLY exist in
 * TA or User's Server.
 * ------------------------------------------------------------------*/
/* RSA CRYPTO DEMO uuid: f68fd704-6eb1-4d14-b218-722850eb3ef0 */
static const TEEC_UUID RSA_CRYPTO_uuid = {
    0xf68fd704, 0x6eb1, 0x4d14,
    { 0xb2, 0x18, 0x72, 0x28, 0x50, 0xeb, 0x3e, 0xf0 }
};

#define TEEC_DEBUG
#define RSA_CRYPTO_DEMO_TA_PATH     "/data/f68fd704-6eb1-4d14-b218-722850eb3ef0.sec"
#define RSA_KEY_SIZE                256    // 2048bits
#define RSA_MASSAGE_SIZE            100    // see rsa-demoTA.c, massage size has some limitation.
#define RSA_KEY_1                   1
#define RSA_ALG_OAEP_SHA512         1      // use TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512 mode for crypto
#define RSA_ALG_PSS_SHA256          2      // use TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA256 mode for sign
#define RSA_INPUT_ERROR_PARAMETER   0x10000001
#define PRINTF_SIZE                 32

#define PARAMS_INDEX_2              2      // params参数的下标索引 2
#define PARAMS_INDEX_3              3      // params参数的下标索引 3

enum {
    CMD_GENERATE_RANDOM = 0,
    CMD_GENERATE_KEYPAIR,
    CMD_SAVE_KEYPAIR,
    CMD_READ_KEYPAIR,
    CMD_ENC_OAEP_MGF1_SHA512,
    CMD_DEC_OAEP_MGF1_SHA512,
    CMD_SIGN_PSS_MGF1_SHA256,
    CMD_VERIFY_PSS_MGF1_SHA256,
};

TEEC_Context g_context;
TEEC_Session g_session;

#ifdef TEEC_DEBUG
void DumpBuff(const char *buffer, size_t bufLen)
{
    size_t i;
    if (buffer == NULL || bufLen == 0) {
        return;
    }

    printf("\n--------------------------------------------------\n");
    printf("bufLen = %d\n", bufLen);
    for (i = 0; i < bufLen; i++) {
        if (i % PRINTF_SIZE == 0) {
            printf("\n");
        }
        printf("%02x ", *(buffer + i));
    }
    printf("\n--------------------------------------------------\n");
    return;
}
#else
void DumpBuff(char *buffer, size_t bufLen)
{
    return;
}
#endif

/* initialize context and opensession */
TEEC_Result TeecInit(void)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin;

    result = TEEC_InitializeContext(NULL, &g_context);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("teec initial failed");
        return result;
    }

    /* pass TA's FULLPATH to TEE, then OpenSession. */
    /* CA MUST use the TEEC_LOGIN_IDENTIFY mode to login. */
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_NONE,
        TEEC_NONE,
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_INPUT);
    g_context.ta_path = (uint8_t *)RSA_CRYPTO_DEMO_TA_PATH;

    result = TEEC_OpenSession(&g_context, &g_session, &RSA_CRYPTO_uuid, TEEC_LOGIN_IDENTIFY, NULL, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        printf("open session failed: result:%x orgin: %d.\n", (int)result, origin);
        TEEC_FinalizeContext(&g_context);
        return result;
    }

    TEEC_Debug("teec init OK.");
    return result;
}

void TeecClose(void)
{
    TEEC_CloseSession(&g_session);
    TEEC_FinalizeContext(&g_context);

    TEEC_Debug("teec uninit OK.");
}

/* InvokeCommand - Generate Random number as a message to be encrypt later. */
TEEC_Result GenerateRandomCmd(char* msgBuf, uint32_t bufLen)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin;

    if (msgBuf == NULL || (bufLen < RSA_MASSAGE_SIZE)) {
        TEEC_Error("invoke GenerateRandomCmd has wrong params.");
        return (TEEC_Result)RSA_INPUT_ERROR_PARAMETER;
    }

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);

    operation.params[0].tmpref.buffer = msgBuf;
    operation.params[0].tmpref.size = RSA_MASSAGE_SIZE;

    result = TEEC_InvokeCommand(&g_session, CMD_GENERATE_RANDOM, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke GenerateRandomCmd failed, codes=0x%x, origin=0x%x", result, origin);
        return result;
    }

    if (operation.params[0].tmpref.size != RSA_MASSAGE_SIZE) {
        TEEC_Error("invoke GenerateRandomCmd failed, returned size is %d.", operation.params[0].tmpref.size);
    } else {
        TEEC_Debug("invoke GenerateRandomCmd is successed.");
        printf("random msg is : \n");
        DumpBuff(msgBuf, operation.params[0].tmpref.size);
    }

    return result;
}

/* InvokeCommand - Generate a RSA Keypair. */
TEEC_Result RsaGenerateKeypairCmd(void)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);

    operation.params[0].value.a = RSA_KEY_SIZE;
    operation.params[0].value.b = RSA_KEY_1;

    result = TEEC_InvokeCommand(&g_session, CMD_GENERATE_KEYPAIR, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke RsaGenerateKeypairCmd failed, codes=0x%x, origin=0x%x", result, origin);
    }

    return result;
}

/* InvokeCommand - Save Keypair to secure storage. */
TEEC_Result RsaSaveKeypairCmd(char* path, uint32_t pathLen)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin;

    if (pathLen == 0) {
        TEEC_Error("RsaSaveKeypairCmd: pathlen is 0.");
        return (TEEC_Result)RSA_INPUT_ERROR_PARAMETER;
    }

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_NONE,
        TEEC_NONE);

    operation.params[0].value.a = RSA_KEY_1;
    operation.params[1].tmpref.buffer = path;
    operation.params[1].tmpref.size = pathLen;

    result = TEEC_InvokeCommand(&g_session, CMD_SAVE_KEYPAIR, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke RsaSaveKeypairCmd failed, codes=0x%x, origin=0x%x", result, origin);
    }

    return result;
}

/* InvokeCommand - Read Keypair from secure storage. */
TEEC_Result RsaReadKeypairCmd(char* path, uint32_t len)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin;

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_NONE,
        TEEC_NONE);

    operation.params[0].value.a = RSA_KEY_1;
    operation.params[1].tmpref.buffer = path;
    operation.params[1].tmpref.size = len;

    result = TEEC_InvokeCommand(&g_session, CMD_READ_KEYPAIR, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke RsaReadKeypairCmd failed, codes=0x%x, origin=0x%x", result, origin);
    }

    return result;
}

TEEC_Result InitRsaOperation(TEEC_Operation *operation, char* msgBuf1, uint32_t msgLen1,
                             char* msgBuf2, uint32_t msgLen2)
{
    if (msgBuf1 == NULL || msgLen1 == 0 || msgBuf2 == NULL || msgLen2 == 0) {
        TEEC_Error("InitRsaOperation has wrong params.");
        return (TEEC_Result)RSA_INPUT_ERROR_PARAMETER;
    }

    operation->started = 1;
    operation->paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_NONE,
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_OUTPUT);

    operation->params[0].value.a = RSA_KEY_1;
    operation->params[0].value.b = RSA_ALG_OAEP_SHA512;
    operation->params[PARAMS_INDEX_2].tmpref.buffer = msgBuf1;
    operation->params[PARAMS_INDEX_2].tmpref.size = msgLen1;
    operation->params[PARAMS_INDEX_3].tmpref.buffer = msgBuf2;
    operation->params[PARAMS_INDEX_3].tmpref.size = msgLen2;

    return TEEC_SUCCESS;
}

/* InvokeCommand - Encrypt message with TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512. */
TEEC_Result RsaEncryptCmd(char* msgBuf, uint32_t msgLen, char* encBuf, uint32_t bufLen)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin;

    result = InitRsaOperation(&operation, msgBuf, msgLen, encBuf, bufLen);
    if (result != TEEC_SUCCESS) {
        return result;
    }

    result = TEEC_InvokeCommand(&g_session, CMD_ENC_OAEP_MGF1_SHA512, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke RsaEncryptCmd failed, codes=0x%x, origin=0x%x.", result, origin);
    } else if (operation.params[PARAMS_INDEX_3].tmpref.size != RSA_KEY_SIZE) {
        TEEC_Error("invoke RsaEncryptCmd failed, returned Encrypted data size is %d.",
                   operation.params[PARAMS_INDEX_3].tmpref.size);
    } else {
        printf("encBuf is : \n");
        DumpBuff(encBuf, operation.params[PARAMS_INDEX_3].tmpref.size);
    }

    return result;
}

/* InvokeCommand - Decrypt message with TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512. */
TEEC_Result RsaDecryptCmd(char* encBuf, uint32_t encBufLen, char* decBuf, uint32_t decBufLen)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin;

    result = InitRsaOperation(&operation, encBuf, encBufLen, decBuf, decBufLen);
    if (result != TEEC_SUCCESS) {
        return result;
    }

    result = TEEC_InvokeCommand(&g_session, CMD_DEC_OAEP_MGF1_SHA512, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke RsaDecryptCmd failed, codes=0x%x, origin=0x%x.", result, origin);
        return result;
    }
    if (operation.params[PARAMS_INDEX_3].tmpref.size < RSA_MASSAGE_SIZE) {
        TEEC_Error("invoke RsaDecryptCmd failed, returned Decrypted data size is %d.",
                   operation.params[PARAMS_INDEX_3].tmpref.size);
    } else {
        TEEC_Debug("invoke RsaDecryptCmd successed, returned Decrypted data size is %d.",
                   operation.params[PARAMS_INDEX_3].tmpref.size);
        printf("derypted_buffer is : \n");
        DumpBuff(decBuf, operation.params[PARAMS_INDEX_3].tmpref.size);
    }

    return result;
}

int VerifyRsaCrypto(const char* msgBuf, const char* decBuf, uint32_t len)
{
    if (msgBuf == NULL || decBuf == NULL) {
        TEEC_Error("VerifyRsaCrypto has wrong params.");
        return RSA_INPUT_ERROR_PARAMETER;
    }

    return memcmp(msgBuf, decBuf, len);
}

/* InvokeCommand - Sign message with TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512. */
static TEEC_Result RsaSignCmd(char* msgBuf, uint32_t msgLen, char* signBuf, uint32_t *bufLen)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin;

    if (msgBuf == NULL || signBuf == NULL || (bufLen == NULL)) {
        TEEC_Error("invoke RsaSignCmd has wrong params.");
        return (TEEC_Result)RSA_INPUT_ERROR_PARAMETER;
    }

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_NONE,
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_OUTPUT);

    operation.params[0].value.a = RSA_KEY_1;
    operation.params[0].value.b = RSA_ALG_PSS_SHA256;
    operation.params[PARAMS_INDEX_2].tmpref.buffer = msgBuf;
    operation.params[PARAMS_INDEX_2].tmpref.size = msgLen;
    operation.params[PARAMS_INDEX_3].tmpref.buffer = signBuf;
    operation.params[PARAMS_INDEX_3].tmpref.size = *bufLen;

    result = TEEC_InvokeCommand(&g_session, CMD_SIGN_PSS_MGF1_SHA256, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke RsaSignCmd failed, codes=0x%x, origin=0x%x.", result, origin);
    } else if (operation.params[PARAMS_INDEX_3].tmpref.size != RSA_KEY_SIZE) {
        TEEC_Error("invoke RsaSignCmd failed, returned Encrypted data size is %d.",
                   operation.params[PARAMS_INDEX_3].tmpref.size);
    } else {
        printf("signBuf is : \n");
        DumpBuff(signBuf, operation.params[PARAMS_INDEX_3].tmpref.size);
        *bufLen = operation.params[PARAMS_INDEX_3].tmpref.size;
    }

    return result;
}

/* InvokeCommand - Sign message with TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512. */
static TEEC_Result RsaVerifyCmd(char* msgBuf, uint32_t msgLen, char* signBuf, uint32_t bufLen)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin;

    if (msgBuf == NULL || (msgLen == 0) || signBuf == NULL || (bufLen == 0)) {
        TEEC_Error("invoke RsaVerifyCmd has wrong params.");
        return (TEEC_Result)RSA_INPUT_ERROR_PARAMETER;
    }

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_VALUE_INPUT,
        TEEC_NONE,
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_INPUT);

    operation.params[0].value.a = RSA_KEY_1;
    operation.params[0].value.b = RSA_ALG_PSS_SHA256;
    operation.params[PARAMS_INDEX_2].tmpref.buffer = msgBuf;
    operation.params[PARAMS_INDEX_2].tmpref.size = msgLen;
    operation.params[PARAMS_INDEX_3].tmpref.buffer = signBuf;
    operation.params[PARAMS_INDEX_3].tmpref.size = bufLen;

    result = TEEC_InvokeCommand(&g_session, CMD_VERIFY_PSS_MGF1_SHA256, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke RsaVerifyCmd failed, codes=0x%x, origin=0x%x.", result, origin);
    }
    return result;
}

int main(void)
{
    char msgBuf[RSA_KEY_SIZE] = {0};
    char encBuf[RSA_KEY_SIZE] = {0};
    char decBuf[RSA_KEY_SIZE] = {0};
    char signature[RSA_KEY_SIZE] = {0};
    uint32_t bufLen = RSA_KEY_SIZE;
    char keyStorePath[] = "sec_storage_data/rsa_demo/keypair_1";
    uint32_t pathLen = strlen(keyStorePath);

    if (TEEC_SUCCESS != TeecInit()) {
        printf("TeecInit Failed!\n");
        return -1;
    }

    /* Generate Random data */
    if (GenerateRandomCmd(msgBuf, bufLen) != TEEC_SUCCESS) {
        goto out;
    }

    /* Generate RSA keypair */
    if (RsaGenerateKeypairCmd() != TEEC_SUCCESS) {
        goto out;
    }

    /* save keypair to secure storage */
    if (RsaSaveKeypairCmd(keyStorePath, pathLen) != TEEC_SUCCESS) {
        goto out;
    }

    /* read keypair to secure storage */
    if (RsaReadKeypairCmd(keyStorePath, pathLen) != TEEC_SUCCESS) {
        goto out;
    }

    /* Encrypt */
    if (RsaEncryptCmd(msgBuf, RSA_MASSAGE_SIZE, encBuf, bufLen) != TEEC_SUCCESS) {
        goto out;
    }

    /* Decrypt */
    if (RsaDecryptCmd(encBuf, bufLen, decBuf, bufLen) != TEEC_SUCCESS) {
        goto out;
    }

    /* verify */
    if (VerifyRsaCrypto(msgBuf, decBuf, RSA_KEY_SIZE) < 0) {
        printf("Error: rsa decBuf is not match msgBuf!\n");
        goto out;
    }

    /* sign */
    if (RsaSignCmd(msgBuf, RSA_MASSAGE_SIZE, signature, &bufLen) != TEEC_SUCCESS) {
        goto out;
    }

    /* verify */
    if (RsaVerifyCmd(msgBuf, RSA_MASSAGE_SIZE, signature, bufLen) != TEEC_SUCCESS) {
        printf("Error: rsa signature is not match msgBuf!\n");
    } else {
        printf("Succeed to load and excute RSA crypto,sign and verify!\n");
    }

    TeecClose();
    return 0;
out:
    TeecClose();
    return -1;
}
