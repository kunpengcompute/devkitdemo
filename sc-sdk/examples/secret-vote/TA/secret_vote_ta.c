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
 * Description: secret_vote_ta.c
 * Create: 2022-05-10
 */
#include <stdio.h>
#include <string.h>

#include "tee_core_api.h"
#include "tee_defines.h"
#include "tee_log.h"
#include "tee_crypto_api.h"
#include "tee_object_api.h"
#include "tee_ext_api.h"
#include "secret_vote_ta.h"

static int GetStoragePath(char *prefix, size_t prefixLen, char *username, size_t usernameLen, char *storagePath)
{
    if (prefix == NULL || username == NULL || storagePath == NULL) {
        SLogError("GetStoragePath: param is NULL.");
        return GENERIC_ERROR;
    }
    if (prefixLen + usernameLen + strlen(SEC_STORAGE_PATH) >= 128) {
        SLogError("GetStoragePath: path too long.");
        return GENERIC_ERROR;
    }
    if (sprintf(storagePath, "%s%s/%s", SEC_STORAGE_PATH, prefix, username) < 0) {
        SLogError("GetStoragePath: Failed to get storage path.");
        return GENERIC_ERROR;
    }
    return SUCCESS;
}

TEE_Result TA_CreateEntryPoint(void)
{
    SLogTrace("TA_CreateEntryPoint");
    if (addcaller_ca_exec(PYTHON3_PATH, "root") != TEE_SUCCESS) {
        SLogError("TA_CreateEntryPoint: add caller for ca exec failed.");
        return TEE_ERROR_GENERIC;
    }
    SLogTrace("TA_CreateEntryPoint: add caller for ca exec ok.");
    return TEE_SUCCESS;
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE], void **sessionContext)
{
    struct User *user = NULL;
    uint32_t nameLen;
    SLogTrace("Open session entry point.");
    if (!(TEE_PARAM_TYPE_MEMREF_INPUT == TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX3)
          || TEE_PARAM_TYPE_MEMREF_OUTPUT == TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX3)
          || TEE_PARAM_TYPE_MEMREF_INOUT == TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX3))) {
        SLogError("Bad parameters (ParamTypes, 3)");
        return TEE_ERROR_BAD_PARAMETERS;
    }
    nameLen = params[PARAMS_IDX2].memref.size;
    if ((nameLen == 0) || (nameLen > 32)) {
        SLogError("Invalid size of username len.");
        return TEE_ERROR_BAD_PARAMETERS;
    }
    user = (struct User*)TEE_Malloc(sizeof(struct User), 0);
    if (user == NULL) {
        SLogError("Failed to allocate mem for User.");
        return TEE_ERROR_GENERIC;
    }
    user->len = nameLen;
    TEE_MemMove((char *)(user->name), params[PARAMS_IDX2].memref.buffer, params[PARAMS_IDX2].memref.size);
    *sessionContext = (void *)user;
    SLogTrace("Open session entry point ok.");

    return TEE_SUCCESS;
}

void DumpBuff(char *buffer, size_t bufLen)
{
    if (buffer == NULL || bufLen == 0) {
        return;
    }
    SLogError("-----------------------");
    SLogError("bufLen = %d", bufLen);
    size_t size = 8;
    size_t i;
    for (i = 0; i < bufLen / size; i++) {
        SLogError("%02x %02x %02x %02x %02x %02x %02x %02x",
                  *(buffer + i * size), *(buffer + i * size + 1), *(buffer + i * size + 2), *(buffer + i * size + 3),
                  *(buffer + i * size + 4), *(buffer + i * size + 5), *(buffer + i * size + 6), *(buffer + i * size + 7));
    }
    for (i = bufLen /  size * size; i < bufLen; i++) {
        SLogError("%02x", *(buffer + i));
    }
    SLogError("-----------------------\n");
    return;
}

TEE_Result AESDecrypt(AESParam *param, AESOutput *output, char *plain, size_t *plainLen)
{
    TEE_ObjectHandle key = NULL;
    TEE_OperationHandle operation = NULL;
    TEE_Result ret = TEE_SUCCESS;

    void *aad = TEE_Malloc(param->aadLen, 0);
    void *nonce = TEE_Malloc(param->nonceLen, 0);
    void *cipher = TEE_Malloc(output->cipherLen, 0);
    void *tag = TEE_Malloc(output->tagLen, 0);

    if (!nonce || !cipher || !aad || !tag) {
        SLogError("Out of memory");
        ret = TEE_ERROR_GENERIC;
        goto err;
    }
    TEE_MemMove(cipher, output->cipher, output->cipherLen);
    TEE_MemMove(nonce, param->nonce, param->nonceLen);
    TEE_MemMove(tag, output->tag, output->tagLen);
    TEE_MemMove(aad, param->aad, param->aadLen);

    // AES key
    TEE_Attribute aesKey = {0};
    aesKey.attributeID = TEE_ATTR_SECRET_VALUE;
    aesKey.content.ref.length = param->keyLen;
    aesKey.content.ref.buffer = (void *)param->key;

    ret = TEE_AllocateTransientObject(TEE_TYPE_AES, RSA_KEY_SIZE, &key);
    if (ret != TEE_SUCCESS) {
        SLogError("Failed to alloc transient object handle: 0x%x", ret);
        goto err;
    }

    ret = TEE_PopulateTransientObject(key, &aesKey, 1);
    if (ret != TEE_SUCCESS) {
        SLogError("TEE_PopulateTransientObject failure: 0x%x", ret);
        TEE_FreeTransientObject(key);
        goto err;
    }

    ret = TEE_AllocateOperation(&operation, TEE_ALG_AES_GCM, TEE_MODE_DECRYPT, RSA_KEY_SIZE);
    if (ret != TEE_SUCCESS) {
        SLogError("Cant alloc first handler");
        TEE_FreeTransientObject(key);
        goto err;
    }

    ret = TEE_SetOperationKey(operation, key);
    if (ret != TEE_SUCCESS) {
        SLogError("Failed to set first operation key: 0x%x", ret);
        TEE_FreeTransientObject(key);
        TEE_FreeOperation(operation);
        goto err;
    }

    ret = TEE_AEInit(operation, nonce, param->nonceLen, output->tagLen * 8, 0, 0);
    if (ret != TEE_SUCCESS) {
        SLogError("Failed TEE_AEInit: 0x%x", ret);
        TEE_FreeTransientObject(key);
        TEE_FreeOperation(operation);
        goto err;
    }

    TEE_AEUpdateAAD(operation, aad, param->aadLen);

    ret = TEE_AEDecryptFinal(operation, cipher, output->cipherLen, plain, plainLen, tag, output->tagLen);
    if (ret != TEE_SUCCESS) {
        SLogError("Failed to TEE_AEDecryptFinal: 0x%x", ret);
        TEE_FreeTransientObject(key);
        TEE_FreeOperation(operation);
        goto err;
    }
    SLogError("plain len: %d", *plainLen);
    DumpBuff(plain, *plainLen);
    SLogError("tag TEE_AEDecryptFinal");
    DumpBuff(tag, output->tagLen);
    ret = TEE_SUCCESS;

err:
    TEE_Free(cipher);
    TEE_Free(aad);
    TEE_Free(nonce);
    TEE_Free(tag);
    return ret;
}

TEE_Result GenerateRSAKeyPair(TEE_ObjectHandle *tmpObj, uint32_t keySize, char *path, size_t pathLen)
{
    SLogTrace("GenerateRSAKeyPair.");
    TEE_Result ret;
    TEE_ObjectHandle obj = NULL;
    ret = TEE_AllocateTransientObject(TEE_TYPE_RSA_KEYPAIR, keySize, tmpObj);
    if (ret != TEE_SUCCESS) {
        SLogError("GenerateRSAKeyPair: TEE_AllocateTransientObject fail. ret: %x", ret);
        return ret;
    }

    ret = TEE_GenerateKey(*tmpObj, keySize, NULL, 0);
    if (ret != TEE_SUCCESS) {
        SLogError("GenerateRSAKeyPair: TEE_GenerateKey failed.");
        return ret;
    }

    ret = TEE_CreatePersistentObject(TEE_OBJECT_STORAGE_PRIVATE, path, pathLen, TEE_DATA_FLAG_ACCESS_WRITE,
                                     *tmpObj, NULL, 0, &obj);
    if (ret != TEE_SUCCESS) {
        SLogError("GenerateRSAKeyPair: TEE_CreatePersistentObject failed. ret=%x", ret);
    }
    if (obj) {
        TEE_CloseObject(obj);
    }
    return ret;
}

TEE_Result GetPubkeyParam(TEE_ObjectHandle obj, uint8_t *modulusBuffer, size_t *modulusBufferLen,
                          uint8_t *pubexpBuffer, size_t *pubexpBufferLen)
{
    TEE_Result ret;
    ret = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_RSA_MODULUS, (void *)modulusBuffer, modulusBufferLen);
    if (ret != TEE_SUCCESS) {
        SLogError("TEE_GetObjectBufferAttribute: Failed to get modulus. ret: %x", ret);
        return ret;
    }
    ret = TEE_GetObjectBufferAttribute(obj, TEE_ATTR_RSA_PUBLIC_EXPONENT, (void *)pubexpBuffer, pubexpBufferLen);
    if (ret != TEE_SUCCESS) {
        SLogError("TEE_GetObjectBufferAttribute: Failed to get pubexp. ret: %x", ret);
        return ret;
    }
    return TEE_SUCCESS;
}

TEE_Result GenerateRootKeyPair(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE])
{
    SLogTrace("GenerateRootKeyPair.");
    char rootKeyPath[SEC_STORAGE_PATH_MAX] = {0};
    if (GetStoragePath(ROOT_RSA_KEY_PREFIX, strlen(ROOT_RSA_KEY_PREFIX), ROOT_USERNAME, strlen(ROOT_USERNAME), rootKeyPath) != SUCCESS) {
        return TEE_ERROR_GENERIC;
    }

    TEE_Result ret;
    TEE_ObjectHandle obj = NULL;
    ret = GenerateRSAKeyPair(&obj, RSA_KEY_BITS, rootKeyPath, strlen(rootKeyPath));
    if (ret != TEE_SUCCESS) {
        SLogError("GenerateRootKeyPair: Failed to GenerateRSAKeyPair. ret: %x", ret);
        if (obj) {
            TEE_FreeTransientObject(obj);
        }
        return ret;
    }

    uint8_t modulusBuffer[RSA_KEY_SIZE];
    size_t modulusBufferLen = RSA_KEY_SIZE;
    uint8_t pubexpBuffer[RSA_KEY_SIZE];
    size_t pubexpBufferLen = RSA_KEY_SIZE;
    ret = GetPubkeyParam(obj, modulusBuffer, &modulusBufferLen, pubexpBuffer, &pubexpBufferLen);
    if (ret != TEE_SUCCESS) {
        SLogError("TEE_GetObjectBufferAttribute: Failed to get pubkey param. ret: %x", ret);
        TEE_FreeTransientObject(obj);
        return ret;
    }
    DumpBuff((char *)modulusBuffer, modulusBufferLen);
    DumpBuff((char *)pubexpBuffer, pubexpBufferLen);

    TEE_MemMove(params[PARAMS_IDX0].memref.buffer, modulusBuffer, modulusBufferLen);
    params[PARAMS_IDX0].memref.size = modulusBufferLen;
    TEE_MemMove(params[PARAMS_IDX1].memref.buffer, pubexpBuffer, pubexpBufferLen);
    params[PARAMS_IDX1].memref.size = pubexpBufferLen;
    TEE_FreeTransientObject(obj);
    return ret;
}

TEE_Result RSASign(TEE_ObjectHandle *rootKeyObj, void *buffer, size_t bufferLen, char *sign, size_t signLen)
{
    SLogTrace("RSASign.");
    TEE_Result ret;
    char bufferDigest[SHA256_SIZE] = {0};
    size_t bufferDigestLen = SHA256_SIZE;
    TEE_OperationHandle hashOperation = NULL;
    TEE_AllocateOperation(&hashOperation, TEE_ALG_SHA256, TEE_MODE_DIGEST, 0);
    TEE_DigestDoFinal(hashOperation, buffer, bufferLen, bufferDigest, &bufferDigestLen);
    if (hashOperation) {
        TEE_FreeOperation(hashOperation);
    }

    TEE_OperationHandle operation = NULL;
    TEE_AllocateOperation(&operation, TEE_ALG_RSASSA_PKCS1_V1_5_SHA256, TEE_MODE_SIGN, RSA_KEY_BITS);
    ret = TEE_SetOperationKey(operation, *rootKeyObj);
    if (ret != TEE_SUCCESS) {
        SLogError("TEE_SetOperationKey: Failed to set RSA key.");
        return ret;
    }

    ret = TEE_AsymmetricSignDigest(operation, NULL, 0, bufferDigest, bufferDigestLen, sign, &signLen);
    if (ret != TEE_SUCCESS) {
        SLogError("TEE_AsymmetricSignDigest: Failed to sign digest.");
        return ret;
    }
    if (operation) {
        TEE_FreeOperation(operation);
    }
    return ret;
}

TEE_Result GenerateTaskKeyPair(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE])
{
    SLogTrace("GenerateTaskKeyPair.");
    char rootKeyPath[SEC_STORAGE_PATH_MAX] = {0};
    if (GetStoragePath(ROOT_RSA_KEY_PREFIX, strlen(ROOT_RSA_KEY_PREFIX), ROOT_USERNAME, strlen(ROOT_USERNAME), rootKeyPath) != SUCCESS) {
        return TEE_ERROR_GENERIC;
    }
    TEE_Result ret;
    TEE_ObjectHandle obj = NULL;

    char *username = params[PARAMS_IDX3].memref.buffer;
    size_t usernameLen = params[PARAMS_IDX3].memref.size;
    ret = GenerateRSAKeyPair(&obj, RSA_KEY_BITS, username, usernameLen);
    if (ret != TEE_SUCCESS) {
        SLogError("GenerateRSAKeyPair: Failed to GenerateRSAKeyPair. ret= %x", ret);
        if (obj) {
            TEE_FreeTransientObject(obj);
        }
        return ret;
    }

    uint8_t modulusBuffer[RSA_KEY_SIZE];
    size_t modulusBufferLen = RSA_KEY_SIZE;
    uint8_t pubexpBuffer[RSA_KEY_SIZE];
    size_t pubexpBufferLen = RSA_KEY_SIZE;
    ret = GetPubkeyParam(obj, modulusBuffer, &modulusBufferLen, pubexpBuffer, &pubexpBufferLen);
    if (ret != TEE_SUCCESS) {
        SLogError("TEE_GetObjectBufferAttribute: Failed to get pubkey param. ret: %x", ret);
        TEE_FreeTransientObject(obj);
        return ret;
    }
    TEE_FreeTransientObject(obj);
    TEE_MemMove(params[PARAMS_IDX0].memref.buffer, modulusBuffer, modulusBufferLen);
    params[PARAMS_IDX0].memref.size = modulusBufferLen;
    TEE_MemMove(params[PARAMS_IDX1].memref.buffer, pubexpBuffer, pubexpBufferLen);
    params[PARAMS_IDX1].memref.size = pubexpBufferLen;
    struct PubkeyParam *pubkeyParam = (struct PubkeyParam *)TEE_Malloc(sizeof(struct PubkeyParam), 0);
    TEE_MemMove(pubkeyParam->modulusBuffer, modulusBuffer, modulusBufferLen);
    pubkeyParam->modulusBufferLen = modulusBufferLen;
    TEE_MemMove(pubkeyParam->pubexpBuffer, pubexpBuffer, pubexpBufferLen);
    pubkeyParam->pubexpBufferLen = pubexpBufferLen;

    TEE_ObjectHandle rootKeyObj = NULL;
    TEE_ObjectHandle tmpRootKeyObj = NULL;
    ret = TEE_AllocateTransientObject(TEE_TYPE_RSA_KEYPAIR, RSA_KEY_BITS, &tmpRootKeyObj);
    if (ret != TEE_SUCCESS) {
        SLogError("GenerateRootKeyPair: TEE_AllocateTransientObject failed. ret: %x", ret);
        TEE_Free(pubkeyParam);
        return ret;
    }
    ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, rootKeyPath, strlen(rootKeyPath), TEE_DATA_FLAG_ACCESS_READ, &rootKeyObj);
    if (ret != TEE_SUCCESS) {
        SLogError("TEE_OpenPersistentObject: Failed to read root key.");
        TEE_FreeTransientObject(rootKeyObj);
        TEE_FreeTransientObject(tmpRootKeyObj);
        TEE_Free(pubkeyParam);
        return ret;
    }
    TEE_CopyObjectAttributes(tmpRootKeyObj, rootKeyObj);

    char sign[RSA_KEY_SIZE] = {0};
    ret = RSASign(&tmpRootKeyObj, (void *)pubkeyParam, sizeof(struct PubkeyParam), sign, RSA_KEY_BITS);
    if (ret != TEE_SUCCESS) {
        SLogError("RSASign: Failed to sign.");
        TEE_FreeTransientObject(tmpRootKeyObj);
        TEE_Free(pubkeyParam);
        return ret;
    }
    DumpBuff(sign, RSA_KEY_SIZE);

    TEE_MemMove(params[PARAMS_IDX2].memref.buffer, sign, RSA_KEY_SIZE);
    params[PARAMS_IDX2].memref.size = RSA_KEY_SIZE;
    TEE_Free(pubkeyParam);
    if (rootKeyObj) {
        TEE_CloseObject(rootKeyObj);
    }
    if (tmpRootKeyObj) {
        TEE_FreeTransientObject(tmpRootKeyObj);
    }
    return ret;
}

TEE_Result SaveAESKey(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE])
{
    SLogTrace("SaveAESKey.");
    TEE_Result ret;
    char *username = params[PARAMS_IDX0].memref.buffer;
    size_t usernameLen = params[PARAMS_IDX0].memref.size;
    char *cipherParam = params[PARAMS_IDX1].memref.buffer;
    size_t cipherParamLen = params[PARAMS_IDX1].memref.size;
    DumpBuff(cipherParam, cipherParamLen);

    TEE_ObjectHandle rootKeyObj = NULL;
    TEE_ObjectHandle tmpRootKeyObj = NULL;
    ret = TEE_AllocateTransientObject(TEE_TYPE_RSA_KEYPAIR, RSA_KEY_BITS, &tmpRootKeyObj);
    if (ret != TEE_SUCCESS) {
        SLogError("SaveAESKey: TEE_AllocateTransientObject failed.");
        return ret;
    }
    ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, username, usernameLen, TEE_DATA_FLAG_ACCESS_READ, &rootKeyObj);
    if (ret != TEE_SUCCESS) {
        SLogError("SaveAESKey: Failed to TEE_OpenPersistentObject.");
        return ret;
    }
    TEE_CopyObjectAttributes(tmpRootKeyObj, rootKeyObj);

    TEE_OperationHandle operation = NULL;
    ret = TEE_AllocateOperation(&operation, TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA1, TEE_MODE_DECRYPT, RSA_KEY_BITS);
    if (ret != TEE_SUCCESS) {
        SLogError("SaveAESKey TEE_AllocateOperation failed.");
        return ret;
    }
    ret = TEE_SetOperationKey(operation, tmpRootKeyObj);
    if (ret != TEE_SUCCESS) {
        SLogError("SaveAESKey: Failed to set operation key.");
        TEE_FreeOperation(operation);
        return ret;
    }

    char paramBuffer[RSA_KEY_SIZE] = {0};
    size_t paramBufferLen = RSA_KEY_SIZE;
    ret = TEE_AsymmetricDecrypt(operation, NULL, 0, cipherParam, cipherParamLen, paramBuffer, &paramBufferLen);
    if (ret != TEE_SUCCESS) {
        SLogError("SaveAESKey: Failed to decrypt. ret %x", ret);
        return TEE_ERROR_GENERIC;
    }
    char userAESKeyPath[SEC_STORAGE_PATH_MAX] = {0};
    if (GetStoragePath(USER_AES_KEY_PREFIX, strlen(USER_AES_KEY_PREFIX), username, usernameLen, userAESKeyPath) != SUCCESS) {
        return TEE_ERROR_GENERIC;
    }
    TEE_ObjectHandle object = NULL;
    ret = TEE_CreatePersistentObject(TEE_OBJECT_STORAGE_PRIVATE, userAESKeyPath, strlen(userAESKeyPath), TEE_DATA_FLAG_ACCESS_WRITE,
                                     TEE_HANDLE_NULL, NULL, 0, &object);
    if (ret != TEE_SUCCESS) {
        SLogError("SaveAESKey: Failed to create persistent object. ret= %x", ret);
        return TEE_ERROR_GENERIC;
    }
    ret = TEE_WriteObjectData(object, (void*)paramBuffer, paramBufferLen);
    if (ret != TEE_SUCCESS) {
        SLogError("SaveAESKey: Failed to write data. ret= %x", ret);
        TEE_CloseObject(object);
        return TEE_ERROR_GENERIC;
    }
    TEE_CloseObject(object);
    return ret;
}

TEE_Result DecryptVote(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE])
{
    SLogTrace("DecryptVote.");
    TEE_Result ret;
    char *username = params[PARAMS_IDX0].memref.buffer;
    size_t usernameLen = params[PARAMS_IDX0].memref.size;
    char *cipher = params[PARAMS_IDX1].memref.buffer;
    size_t cipherLen = params[PARAMS_IDX1].memref.size;
    char *tag = params[PARAMS_IDX2].memref.buffer;
    size_t tagLen = params[PARAMS_IDX2].memref.size;
    char userAESKeyPath[SEC_STORAGE_PATH_MAX] = {0};
    if (GetStoragePath(USER_AES_KEY_PREFIX, strlen(USER_AES_KEY_PREFIX), username, usernameLen, userAESKeyPath) != SUCCESS) {
        return TEE_ERROR_GENERIC;
    }

    TEE_ObjectHandle object = NULL;
    ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, userAESKeyPath, strlen(userAESKeyPath), TEE_DATA_FLAG_ACCESS_READ, &object);
    if (ret != TEE_SUCCESS) {
        SLogError("DecryptVote: Failed to open persistent object. ret= %x", ret);
        return TEE_ERROR_GENERIC;
    }
    char paramBuffer[RSA_KEY_SIZE] = {0};
    uint32_t paramBufferLen = RSA_KEY_SIZE;
    ret = TEE_ReadObjectData(object, (void *)paramBuffer, RSA_KEY_SIZE, &paramBufferLen);
    if (ret != TEE_SUCCESS) {
        SLogError("DecryptVote: Failed to read data. ret= %x", ret);
        return TEE_ERROR_GENERIC;
    }
    char voteResBuffer[RSA_KEY_SIZE] = {0};
    size_t voteResBufferLen = RSA_KEY_SIZE;
    DumpBuff(paramBuffer, paramBufferLen);

    AESParam *param = (AESParam *)paramBuffer;
    AESOutput *output = TEE_Malloc(sizeof(AESOutput), 0);
    output->cipher = cipher;
    output->cipherLen = cipherLen;
    output->tag = tag;
    output->tagLen = tagLen;
    ret = AESDecrypt(param, output, voteResBuffer, &voteResBufferLen);
    if (ret != TEE_SUCCESS) {
        SLogError("DecryptVote: Failed to AESDecrypt. ret= %x", ret);
        TEE_CloseObject(object);
        TEE_Free(output);
        return TEE_ERROR_GENERIC;
    }
    TEE_Free(output);

    TEE_MemMove(params[PARAMS_IDX3].memref.buffer, voteResBuffer, voteResBufferLen);
    params[PARAMS_IDX3].memref.size = voteResBufferLen;
    TEE_CloseObject(object);
    return ret;
}

TEE_Result TA_InvokeCommandEntryPoint(void *sessionContext, uint32_t cmdId,
uint32_t paramTypes, TEE_Param params[PARAMS_SIZE])
{
    TEE_Result ret;
    SLogTrace("---- TA_InvokeCommandEntryPoint ----cmdid: %d------", cmdId);
    (void)sessionContext;
    switch (cmdId) {
        case CMD_CREATE_ROOT_KEYPAIR:
            ret = GenerateRootKeyPair(paramTypes, params);
            break;
        case CMD_CREATE_USER_KEYPAIR:
            ret = GenerateTaskKeyPair(paramTypes, params);
            break;
        case CMD_AES_KEY_EXCHANGE:
            ret = SaveAESKey(paramTypes, params);
            break;
        case CMD_ENCRYPT_VOTE:
            ret = DecryptVote(paramTypes, params);
            break;
    
        default:
            SLogError("Unknown CMD ID: %d", cmdId);
            ret = TEE_FAIL;
    }
    return ret;
}

void TA_CloseSessionEntryPoint(void *sessionContext)
{
    struct User *user = (struct User *)sessionContext;
    TEE_Free(user);
    SLogTrace("---- TA_CloseSessionEntryPoint ----- ");
}

void TA_DestroryEntryPoint(void)
{
    SLogTrace("---- TA_DestroryEntryPoint ---- ");
}

