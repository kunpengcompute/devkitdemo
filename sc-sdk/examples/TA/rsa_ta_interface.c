/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: RSA-TA
 * Create: 2020-04-15
 */
#include "rsa_ta_interface.h"
#include <string.h>
#include "tee_core_api.h"
#include "tee_defines.h"
#include "tee_log.h"
#include "tee_crypto_api.h"
#include "tee_object_api.h"
// #include "tee_trusted_storage_api.h"

TEE_ObjectHandle g_rsaKeyObj1;
/*********************************************************
 * 函数名： CmdRSAGenRandom
 * 功能描述:调用硬件生成器生成一些随机数据
 * 参数   msgbuf：保存 随机数据的地址
 *        msglen： 产生随机数据的长度
 * 返回值：TEE_SUCCESS 表示正常返回
 *********************************************************/
TEE_Result CmdRSAGenRandom(char *msgbuf, uint32_t *msglen)
{
    TEE_Result ret;
    uint32_t len;
    if (msgbuf == NULL || msglen == NULL || *msglen == 0) {
        SLogError("CmdRSAGenRandom: Bad expected parameters.");
        return TEE_ERROR_BAD_PARAMETERS;
    }
    len = *msglen;
    TEE_GenerateRandom(msgbuf, len);

    char *buf = (char *)TEE_Malloc(len, 0);
    if (buf == NULL) {
        SLogError("malloc failed");
        return TEE_ERROR_OUT_OF_MEMORY;
    }

    if (memcmp(msgbuf, buf, len) == 0) {
        *msglen = 0;
        ret = TEE_ERROR_GENERIC;
        SLogError("CmdRSAGenRandom: TEE_GenerateRandom Failed.");
    } else {
        ret = TEE_SUCCESS;
    }
    TEE_Free(buf);
    return ret;
}
/**************************************************************
 * 函数名称： CmdRSAGenKeypair
 * 功能描述:  随机产生一些的RSA秘钥数据
 * 参数 ：    keysize：秘钥数据的长度
 *            num_key： 秘钥键值
 * 返回值：TEE_SUCCESS 表示随机生成秘钥数据成功
 **************************************************************/
TEE_Result CmdRSAGenKeypair(uint32_t keySize, uint32_t numKey)
{
    TEE_Result ret;
    TEE_ObjectHandle tmpKeyObj = NULL;
    ret = TEE_AllocateTransientObject(TEE_TYPE_RSA_KEYPAIR, keySize, &tmpKeyObj);
    if (ret != TEE_SUCCESS) {
        SLogError("CmdRSAGenKeypair: TEE_AllocateTransientObject failed.");
        return ret;
    }

    ret = TEE_GenerateKey(tmpKeyObj, keySize, NULL, 0);
    if (ret != TEE_SUCCESS) {
        SLogError("CmdRSAGenKeypair: TEE_GenerateKey failed.");
        TEE_FreeTransientObject(tmpKeyObj);
        return ret;
    }

    switch (numKey) {
        case RSA_KEY_1:
            if (g_rsaKeyObj1 == NULL) {
                ret = TEE_AllocateTransientObject(TEE_TYPE_RSA_KEYPAIR, keySize, &g_rsaKeyObj1);
                if (ret != TEE_SUCCESS) {
                    SLogError("CmdRSAGenKeypair: TEE_AllocateTransientObject failed.");
                    break;
                }
            }
            TEE_CopyObjectAttributes(g_rsaKeyObj1, tmpKeyObj);
            break;
        default:
            SLogError("CmdRSAGenKeypair: key number: %d is not defined.", numKey);
            ret = TEE_ERROR_BAD_PARAMETERS;
    }
    TEE_FreeTransientObject(tmpKeyObj);
    return ret;
}

/****************************************************************************
 * 函数名：  CmdRSASaveKeypair
 * 功能描述: 将秘钥数据保存到/data/sec_storage_data 路径内
 * 参数    ：num_key ：秘钥键值
 *          path：    保存秘钥数据的路径
 *          pathlen： 保存秘钥数据路径长度
 * 返回值：TEE_SUCCESS 表示秘钥数据保存成功
 ****************************************************************************/
TEE_Result CmdRSASaveKeypair(uint32_t numKey, char *path, uint32_t pathlen)
{
    TEE_Result ret;
    TEE_ObjectHandle tmpKeyObj = NULL;

    if ((pathlen < PATHLEN_LIMIT_START) || (pathlen >= PATHLEN_LIMIT_END)) {
        SLogError("CMD_SAVE_KEYPAIR: Bad expected pathlen: %d.", pathlen);
        return TEE_ERROR_BAD_PARAMETERS;
    }
    switch (numKey) {
        case RSA_KEY_1:
            ret = TEE_CreatePersistentObject(TEE_OBJECT_STORAGE_PRIVATE, path, pathlen, TEE_DATA_FLAG_ACCESS_WRITE,
                                             g_rsaKeyObj1, NULL, 0, (&tmpKeyObj));
            if (ret != TEE_SUCCESS) {
                SLogError("CmdRSASaveKeypair: TEE_CreatePersistentObject failed. ret= %x", ret);
            }
            break;
        default:
		    SLogError("CmdRSASaveKeypair: key number: %d is not defined.", numKey);
            ret = TEE_ERROR_BAD_PARAMETERS;
    }

    if (tmpKeyObj) {
        TEE_CloseObject(tmpKeyObj);
    }
    return ret;
}
/**************************************************************************
 * 函数名：  CmdRSAReadKeypair
 * 功能描述: 从/data/sec_storage_data读取秘钥数据
 * 参数    ：num_key ：秘钥键值
 *          path：    保存秘钥数据的路径
 *          pathlen： 保存秘钥数据路径长度
 * 返回值：TEE_SUCCESS 表示秘钥数据读取成功
 ***************************************************************************/
TEE_Result CmdRSAReadKeypair(uint32_t numKey, char *path, uint32_t pathlen)
{
    TEE_Result ret;
    TEE_ObjectHandle tmpKeyObj = NULL;

    if ((pathlen < PATHLEN_LIMIT_START) || (pathlen >= PATHLEN_LIMIT_END)) {
        SLogError("CMD_READ_KEYPAIR: Bad expected pathlen: %d.", pathlen);
        return TEE_ERROR_BAD_PARAMETERS;
    }
    switch (numKey) {
        case RSA_KEY_1:
            ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, path, pathlen,
                                           TEE_DATA_FLAG_ACCESS_READ, (&tmpKeyObj));
            if (ret != TEE_SUCCESS) {
                SLogError("CmdRSAReadKeypair: TEE_OpenPersistentObject failed.");
                break;
            }
            if (g_rsaKeyObj1 == NULL) {
                ret = TEE_AllocateTransientObject(TEE_TYPE_RSA_KEYPAIR, RSA_KEY_SIZE, &g_rsaKeyObj1);
                if (ret != TEE_SUCCESS) {
                    SLogError("CmdRSAReadKeypair: TEE_AllocateTransientObject failed.");
                    break;
                }
                TEE_CopyObjectAttributes(g_rsaKeyObj1, tmpKeyObj);
            }
            break;
        default:
            SLogError("CmdRSAReadKeypair: key number: %d is not defined.", numKey);
            ret = TEE_ERROR_BAD_PARAMETERS;
    }

    if (tmpKeyObj) {
        TEE_CloseObject(tmpKeyObj);
    }
    return ret;
}
/************************************************************************************************************
 * 函数名称：CmdRSACryptoParamCheck
 * 功能描述：检测加密原文数据地址和加密数据的缓存的地址以及缓存大小是否存在
 * 参数    ：Operation ：TEE操作引擎
 *           buf1：     原文数据存储地址
 *           buf1_len： 原文长度
 *           buf2：     密文数据存储地址
 *           buf2_len   密文数据长度
 *           cryptmode：加密模式
 *           algorithm：加密算法的索引(即使用那种算法加密)
 * 返回类型：TEE_SUCCESS表示 参数检查完毕，且无异常
 * **********************************************************************************************************/
TEE_Result CmdRSACryptoParamCheck(TEE_OperationHandle *Operation, Buffer *buf1, Buffer *buf2,
                                  uint32_t cryptmode, uint32_t algorithm)
{
    TEE_Result ret;
    if ((buf1->buffer == NULL) || (buf1->size == 0) || (buf2->buffer == NULL) || (buf2->size == 0)) {
        SLogError("CmdRSACrypto: buf is null, or bufLen is 0.");
        return TEE_ERROR_BAD_PARAMETERS;
    }
    ret = TEE_AllocateOperation(Operation, algorithm, cryptmode, buf2->size);
    if (ret != TEE_SUCCESS) {
        SLogError("CmdRSACrypto TEE_AllocateOperation failed.");
        return ret;
    }
    return TEE_SUCCESS;
}
/*****************************************************************************************************
 * 函数名称： CmdRSACrypto
 * 功能描述:  用之前生成的秘钥去加密之前生成的数据
 * 参数      num_key：  键值
 *           alg：      加密算法的索引
 *           buf1：     原文数据存储地址
 *           buf1_len： 原文长度
 *           buf2：     密文数据存储地址
 *           buf2_len   密文数据长度
 *           cryptmode：加密模式
 * 返回值 ：TEE_SUCCESS表示加密成功
 ***************************************************************************************************/
TEE_Result CmdRSACrypto(uint32_t numKey, uint32_t alg, Buffer *buf1, Buffer *buf2, uint32_t cryptmode)
{
    TEE_Result ret;
    TEE_OperationHandle Operation = NULL;
    size_t outlen;
    uint32_t algorithm;
    algorithm = alg;
    outlen = buf2->size;

    ret = CmdRSACryptoParamCheck(&Operation, buf1, buf2, cryptmode, algorithm);
    if (ret != TEE_SUCCESS) {
        return ret;
    }

    switch (numKey) {
        case RSA_KEY_1:
            ret = TEE_SetOperationKey(Operation, g_rsaKeyObj1);
            if (ret != TEE_SUCCESS) {
                SLogError("CmdRSACrypto TEE_SetOperationKey failed.");
                TEE_FreeOperation(Operation);
                return ret;
            }
            break;
        default:
            SLogError("CmdRSACrypto: key number: %d is not defined.", numKey);
            ret = TEE_ERROR_BAD_PARAMETERS;
            TEE_FreeOperation(Operation);
            return ret;
    }
    if (cryptmode == TEE_MODE_ENCRYPT) {
        ret = TEE_AsymmetricEncrypt(Operation, NULL, 0, buf1->buffer, buf1->size, buf2->buffer, &outlen);
        if (ret != TEE_SUCCESS) {
            SLogError("CmdRSACrypto TEE_AsymmetricEncrypt failed. ret %x", ret);
        }
    } else if (cryptmode == TEE_MODE_DECRYPT) {
        ret = TEE_AsymmetricDecrypt(Operation, NULL, 0, buf1->buffer, buf1->size, buf2->buffer, &outlen);
        if (ret != TEE_SUCCESS) {
            SLogError("CmdRSACrypto TEE_AsymmetricDecrypt failed. ret %x", ret);
        }
    } else {
        SLogError("CmdRSACrypto TEE_AsymmetricDecrypt unkown mode: %d.", cryptmode);
        ret = TEE_ERROR_BAD_PARAMETERS;
    }
    if (ret == TEE_SUCCESS) {
        buf2->size = outlen;
    } else {
        buf2->size = 0;
    }
    TEE_FreeOperation(Operation);
    return ret;
}

TEE_Result FreeOperationHandleParam(TEE_OperationHandle Operation1, TEE_OperationHandle Operation2,
                                    TEE_Result ret)
{
    if (Operation1) {
        TEE_FreeOperation(Operation1);
    }
    if (Operation2) {
        TEE_FreeOperation(Operation2);
    }
    return ret;
}

TEE_Result CheckRsaSha256Init(TEE_Result Sha256Result, TEE_Result TEE_DigestDoFinalResult,
                              TEE_Result returTEE_AllocateOperationnVlue2Result)
{
    if (Sha256Result != TEE_SUCCESS) {
        SLogError("TEE_AllocateOperation sha256 failed, ret %x\n", Sha256Result);
        return Sha256Result;
    }
    if (TEE_DigestDoFinalResult != TEE_SUCCESS) {
        SLogError("TEE_DigestDoFinal failed, ret %x\n", TEE_DigestDoFinalResult);
        return TEE_DigestDoFinalResult;
    }
    if (returTEE_AllocateOperationnVlue2Result != TEE_SUCCESS) {
        SLogError("CmdRSACrypto TEE_AllocateOperation failed.");
        return returTEE_AllocateOperationnVlue2Result;
    }
    return TEE_SUCCESS;
}

TEE_Result CheckRsaSha256Mode(TEE_Result TEE_AsymmetricSignDigest, TEE_Result TEE_AsymmetricVerifyDigest,
                              size_t *buf2_len, size_t outlen, uint32_t mode)
{
    if (mode == TEE_MODE_SIGN) {
        if (TEE_AsymmetricSignDigest != TEE_SUCCESS) {
            SLogError("CmdRSASignVerify TEE_AsymmetricSignDigest failed.");
            return TEE_AsymmetricSignDigest;
        }
        *buf2_len = outlen;
    } else if (mode == TEE_MODE_VERIFY) {
        if (TEE_AsymmetricVerifyDigest != TEE_SUCCESS) {
            SLogError("CmdRSASignVerify TEE_AsymmetricVerifyDigest failed.");
            return TEE_AsymmetricVerifyDigest;
        }
    } else {
        SLogError("CmdRSASignVerify unkown mode: %d", mode);
        return TEE_ERROR_BAD_PARAMETERS;
    }
    return TEE_SUCCESS;
}
/******************************************************************************************
 * 函数名： CmdRSASignVerify
 * 功能描述:用秘钥通过TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA256 去签名、验证明文数据
 * 参数：   num_key： 秘钥键值
 *          alg    ：  算法索引
 *          buf1   ：  存储原文数据地址
 *          buf1_len ：原文数据长度
 *          buf2     ：签名存储密文数据地址
 *          buf2_len ：签名密文数据长度
 *          mode     ：签名加密模式
*******************************************************************************************/
TEE_Result CmdRSASignVerify(uint32_t numKey, uint32_t alg, Buffer *buf1, Buffer *buf2, uint32_t mode)
{
    TEE_Result ret, retSign, retVer;
    TEE_OperationHandle Operation1 = NULL;
    TEE_OperationHandle Operation2 = NULL;
    size_t outlen;
    uint32_t algorithm = alg;
    char *buf[DIGEST_BUF_LEN] = {0};
    int bufLen = DIGEST_BUF_LEN;

    if ((buf1->buffer == NULL) || (buf1->size == 0) || (buf2->buffer == NULL) || (buf2->size == 0)) {
        SLogError("CmdRSACrypto: buf is null, or bufLen is 0.");
        return TEE_ERROR_BAD_PARAMETERS;
    }
    outlen = buf2->size;
    buf2->size = 0;
    /* SHA 256 签名初始化 */
    ret = CheckRsaSha256Init(TEE_AllocateOperation(&Operation1, TEE_ALG_SHA256, TEE_MODE_DIGEST, 0),
                             TEE_DigestDoFinal(Operation1, buf1->buffer, buf1->size, buf, (size_t *)&bufLen),
                             TEE_AllocateOperation(&Operation2, algorithm, mode, RSA_KEY_SIZE));
    if (ret != TEE_SUCCESS) {
        goto clear;
    }
    switch (numKey) {
        case RSA_KEY_1:
            ret = TEE_SetOperationKey(Operation2, g_rsaKeyObj1);
            if (ret != TEE_SUCCESS) {
                SLogError("CmdRSACrypto TEE_SetOperationKey failed.");
                goto clear;
            }
            break;
        default:
            SLogError("CmdRSACrypto: key number: %d is not defined.", numKey);
            ret = TEE_ERROR_BAD_PARAMETERS;
            goto clear;
    }
    if (mode == TEE_MODE_SIGN) {
        retSign = TEE_AsymmetricSignDigest(Operation2, NULL, 0, buf1->buffer, buf1->size, buf2->buffer, &outlen);
        retVer = TEE_SUCCESS;
    } else if (mode == TEE_MODE_VERIFY) {
        retSign = TEE_SUCCESS;
        retVer = TEE_AsymmetricVerifyDigest(Operation2, NULL, 0, buf1->buffer, buf1->size, buf2->buffer, outlen);
    } else {
        SLogError("invalid mode=%x for Sign or Verify", mode);
        goto clear;
    }
    ret = CheckRsaSha256Mode(retSign, retVer, &buf2->size, outlen, mode);
    if (ret != TEE_SUCCESS) {
        goto clear;
    }
clear:
    return FreeOperationHandleParam(Operation1, Operation2, ret);
}
/* ----------------------------------------------------------------------------
 *   Trusted Application Entry Points
 * ---------------------------------------------------------------------------- */
/**
 *  Function TA_CreateEntryPoint
 *  Description:
 *    The function TA_CreateEntryPoint is the Trusted Application's constructor,
 *    which the Framework calls when it creates a new instance of the Trusted Application.
 */
TEE_Result TA_CreateEntryPoint(void)
{
    SLogTrace("----- TA_CreateEntryPoint ----- ");
    SLogTrace("TA version: %s ", TA_RSA_CRYPTO_DEMO_VERSION);

    if (addcaller_ca_exec(CLIENT_APPLICATION_NAME, "root") == TEE_SUCCESS) {
        SLogTrace("TA_CreateEntryPoint: AddCaller_CA_exec ok.");
    } else {
        SLogError("TA_CreateEntryPoint: AddCaller_CA_exec failed.");
        return TEE_ERROR_GENERIC;
    }
    return TEE_SUCCESS;
}
/**
 *  函数名称： TA_OpenSessionEntryPoint
 *  功能描述:
 *    The Framework calls the function TA_OpenSessionEntryPoint
 *    when a client requests to open a session with the Trusted Application.
 *    The open session request may result in a new Trusted Application instance
 *    being created.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4], void **sessionContext)
{
    TEE_Result ret = TEE_SUCCESS;
    SLogTrace("---- TA_OpenSessionEntryPoint -------- ");
    (void)paramTypes;
    (void)params;
    (void)sessionContext;
    g_rsaKeyObj1 = NULL;

    return ret;
}

TEE_Result CmdGenerateRandom(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4])
{
    TEE_Result ret;

    if (TEE_PARAM_TYPE_MEMREF_OUTPUT != TEE_PARAM_TYPE_GET(paramTypes, 0)) {
        SLogError("CMD_GENERATE_RANDOM: Bad expected parameter types.");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    uint32_t msglen = params[0].memref.size;
    char *msgbuf = (char *)TEE_Malloc(msglen, 0);
    if (msgbuf ==  NULL) {
        SLogError("malloc failed");
        return TEE_ERROR_OUT_OF_MEMORY;
    }
    ret = CmdRSAGenRandom(msgbuf, &msglen);
    if (ret == TEE_SUCCESS) {
        TEE_MemMove(params[0].memref.buffer, msgbuf, msglen);
        params[0].memref.size = msglen;
    } else {
        params[0].memref.size = 0;
    }

    TEE_Free(msgbuf);
    return ret;
}

TEE_Result CmdGeneratePair(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4])
{
    TEE_Result ret;
    if (TEE_PARAM_TYPE_VALUE_INPUT != TEE_PARAM_TYPE_GET(paramTypes, 0)) {
        SLogError("CMD_GENERATE_KEYPAIR: Bad expected parameter types.");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    ret = CmdRSAGenKeypair(params[0].value.a, params[0].value.b);
    return ret;
}

TEE_Result CmdSaveKeyPair(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4])
{
    TEE_Result ret;

    if (TEE_PARAM_TYPE_VALUE_INPUT != TEE_PARAM_TYPE_GET(paramTypes, 0) ||
        TEE_PARAM_TYPE_MEMREF_INPUT != TEE_PARAM_TYPE_GET(paramTypes, 1)) {
        SLogError("CMD_SAVE_KEYPAIR: Bad expected parameter types.");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    uint32_t savepathlen = params[1].memref.size;
    char *path = (char *)TEE_Malloc(savepathlen, 0);
    if (path == NULL) {
        SLogError("malloc failed");
        return TEE_ERROR_OUT_OF_MEMORY;
    }
    TEE_MemMove(path, params[1].memref.buffer, params[1].memref.size);
    ret = CmdRSASaveKeypair(params[0].value.a, path, savepathlen);

    TEE_Free(path);
    return ret;
}

TEE_Result CmdReadKeyPair(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4])
{
    TEE_Result ret;

    if (TEE_PARAM_TYPE_VALUE_INPUT != TEE_PARAM_TYPE_GET(paramTypes, 0) ||
        TEE_PARAM_TYPE_MEMREF_INPUT != TEE_PARAM_TYPE_GET(paramTypes, 1)) {
        SLogError("CMD_READ_KEYPAIR: Bad expected parameter types.");
        return TEE_ERROR_BAD_PARAMETERS;
    }

    uint32_t readpathlen = params[PARAMS_IDX1].memref.size;
    char *path = (char *)TEE_Malloc(readpathlen, 0);
    if (path == NULL) {
        SLogError("malloc failed");
        return TEE_ERROR_OUT_OF_MEMORY;
    }
    TEE_MemMove(path, params[PARAMS_IDX1].memref.buffer, params[PARAMS_IDX1].memref.size);
    ret = CmdRSAReadKeypair(params[PARAMS_IDX0].value.a, path, readpathlen);
    TEE_Free(path);

    return ret;
}

TEE_Result ParamCheckEncOaepMgf1SHA512(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4], int numkey)
{
    switch (numkey) {
        case NUM_VALUE_0:
            if ((TEE_PARAM_TYPE_VALUE_INPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX0)) ||
                (TEE_PARAM_TYPE_MEMREF_INPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX2)) ||
                (TEE_PARAM_TYPE_MEMREF_OUTPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX3))) {
                SLogError("CMD_ENC_OAEP_MGF1_SHA512: Bad expected parameter types.");
                return TEE_ERROR_BAD_PARAMETERS;
            }
            break;
        default:
            if (params[PARAMS_IDX0].value.b != RSA_ALG_OAEP_SHA512) {
                SLogError("algorithm is invalid %d", params[PARAMS_IDX0].value.b);
                return TEE_ERROR_BAD_PARAMETERS;
            }
            if (params[PARAMS_IDX3].memref.size < RSA_2048_OUTPUT_SIZE) {
                SLogError("output size %d small than needed", params[PARAMS_IDX3].memref.size);
                return TEE_ERROR_BAD_PARAMETERS;
            }
            break;
    }
    return TEE_SUCCESS;
}
TEE_Result CmdEncOaepMgf1SHA512(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4])
{
    TEE_Result ret;
    Buffer msgbuf = {NULL, 0};
    Buffer encryptbuf = {NULL, 0};

    if ((ret = ParamCheckEncOaepMgf1SHA512(paramTypes, params, 0)) != TEE_SUCCESS ||
        (ret = ParamCheckEncOaepMgf1SHA512(paramTypes, params, 1)) != TEE_SUCCESS) {
        return ret;
    }

    msgbuf.size = params[PARAMS_IDX2].memref.size;
    msgbuf.buffer = (char *)TEE_Malloc(msgbuf.size, 0);
    if (msgbuf.buffer == NULL) {
        SLogError("malloc failed");
        return TEE_ERROR_OUT_OF_MEMORY;
    }
    TEE_MemMove(msgbuf.buffer, params[PARAMS_IDX2].memref.buffer, params[PARAMS_IDX2].memref.size);
    encryptbuf.size = params[PARAMS_IDX3].memref.size;
    encryptbuf.buffer = (char *)TEE_Malloc(encryptbuf.size, 0);
    if (encryptbuf.buffer == NULL) {
        SLogError("malloc failed");
        TEE_Free(msgbuf.buffer);
        return TEE_ERROR_OUT_OF_MEMORY;
    }
    ret = CmdRSACrypto(params[PARAMS_IDX0].value.a, TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512,
        &msgbuf, &encryptbuf, TEE_MODE_ENCRYPT);
    if (ret != TEE_SUCCESS) {
        SLogError("CmdRSACrypto failed");
        params[PARAMS_IDX3].memref.size = 0;
    } else {
        TEE_MemMove(params[PARAMS_IDX3].memref.buffer, encryptbuf.buffer, encryptbuf.size);
        params[PARAMS_IDX3].memref.size = encryptbuf.size;
    }

    TEE_Free(msgbuf.buffer);
    TEE_Free(encryptbuf.buffer);
    return TEE_SUCCESS;
}

TEE_Result ParamCheckDecOaepMgf1SHA512(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4], int numkey)
{
    switch (numkey) {
        case NUM_VALUE_0:
            if ((TEE_PARAM_TYPE_VALUE_INPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX0)) ||
                (TEE_PARAM_TYPE_MEMREF_INPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX2)) ||
                (TEE_PARAM_TYPE_MEMREF_OUTPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX3))) {
                SLogError("CMD_DEC_OAEP_MGF1_SHA512: Bad expected parameter types.");
                return TEE_ERROR_BAD_PARAMETERS;
            }
            break;
        default:
            if (params[PARAMS_IDX2].memref.size != RSA_2048_OUTPUT_SIZE) {
                SLogError("encrypted buffer len %d invalid", params[PARAMS_IDX2].memref.size);
                return TEE_ERROR_BAD_PARAMETERS;
            }
            break;
    }
    return TEE_SUCCESS;
}

TEE_Result DecOaepMgf1SHA512SLogError(char *encbuf, const char *strs)
{
    SLogError("%s", strs);
    TEE_Free(encbuf);
    return TEE_ERROR_OUT_OF_MEMORY;
}

TEE_Result CmdDecOaepMgf1SHA512(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4])
{
    TEE_Result ret;
    Buffer encbuf = {NULL, 0};
    Buffer decryptbuf = {NULL, 0};

    if ((ret = ParamCheckDecOaepMgf1SHA512(paramTypes, params, 0)) != TEE_SUCCESS ||
        (ret = ParamCheckDecOaepMgf1SHA512(paramTypes, params, 1)) != TEE_SUCCESS) {
        return ret;
    }

    encbuf.size = params[PARAMS_IDX2].memref.size;
    encbuf.buffer = (char *)TEE_Malloc(encbuf.size, 0);
    if (encbuf.buffer == NULL) {
        SLogError("malloc failed");
        return TEE_ERROR_OUT_OF_MEMORY;
    }
    TEE_MemMove(encbuf.buffer, params[PARAMS_IDX2].memref.buffer, params[PARAMS_IDX2].memref.size);
    decryptbuf.size = params[PARAMS_IDX3].memref.size;
    decryptbuf.buffer = (char *)TEE_Malloc(decryptbuf.size, 0);
    if (decryptbuf.buffer == NULL) {
        return DecOaepMgf1SHA512SLogError(decryptbuf.buffer, "malloc failed");
    }
    ret = CmdRSACrypto(params[0].value.a, TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512,
        &encbuf, &decryptbuf, TEE_MODE_DECRYPT);
    if (params[PARAMS_IDX3].memref.size < decryptbuf.size) {
        SLogError("output size %d too small", params[PARAMS_IDX3].memref.size);
        params[PARAMS_IDX3].memref.size = decryptbuf.size;
        ret = TEE_ERROR_BAD_PARAMETERS;
    }
    if (ret != TEE_SUCCESS) {
        params[PARAMS_IDX3].memref.size = 0;
    } else {
        TEE_MemMove(params[PARAMS_IDX3].memref.buffer, decryptbuf.buffer, decryptbuf.size);
        params[PARAMS_IDX3].memref.size = decryptbuf.size;
    }

    TEE_Free(encbuf.buffer);
    TEE_Free(decryptbuf.buffer);
    return ret;
}

TEE_Result ParamCheckSignPssMgf1SHA256(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4], int numkey)
{
    switch (numkey) {
        case NUM_VALUE_0:
            if ((TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX0) != TEE_PARAM_TYPE_VALUE_INPUT) ||
                (TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX2) != TEE_PARAM_TYPE_MEMREF_INPUT) ||
                (TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX3)) != TEE_PARAM_TYPE_MEMREF_OUTPUT) {
                SLogError("CMD_SIGN_PSS_MGF1_SHA256: Bad expected parameter types.");
                return TEE_ERROR_BAD_PARAMETERS;
            }
            break;
        case NUM_VALUE_1:
            if (params[PARAMS_IDX0].value.b != RSA_ALG_PSS_SHA256) {
                SLogError("algorithm is invalid %d", params[PARAMS_IDX0].value.b);
                return TEE_ERROR_BAD_PARAMETERS;
            }
            break;
        default:
            break;
    }
    return TEE_SUCCESS;
}

TEE_Result PssMgf1SHA256SLogError(uint32_t signatureSize, char *input, int rc, int errKey)
{
    switch (errKey) {
        case NUM_VALUE_0:
            SLogError("memcpy_s failed, rc %d", rc);
            TEE_Free(input);
            return TEE_ERROR_OUT_OF_MEMORY;
        case NUM_VALUE_1:
            SLogError("signatureSize %d invalid", signatureSize);
            TEE_Free(input);
            return TEE_ERROR_BAD_PARAMETERS;
        case NUM_VALUE_2:
            SLogError("signature malloc failed");
            TEE_Free(input);
            return TEE_ERROR_OUT_OF_MEMORY;
        default:
            break;
    }
    return TEE_SUCCESS;
}

TEE_Result CmdSignPssMgf1SHA256(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4])
{
    TEE_Result ret;
    Buffer input = {NULL, 0};
    Buffer signature = {NULL, 0};

    if ((ret = ParamCheckSignPssMgf1SHA256(paramTypes, params, 0)) != TEE_SUCCESS ||
        (ret = ParamCheckSignPssMgf1SHA256(paramTypes, params, 1)) != TEE_SUCCESS) {
        return ret;
    }

    uint32_t keyIndex = params[PARAMS_IDX0].value.a;
    input.size = params[PARAMS_IDX2].memref.size;
    input.buffer = (char *)TEE_Malloc(input.size, 0);
    if (input.buffer == NULL) {
        SLogError("CmdSignPssMgf1SHA256 input malloc failed");
        return TEE_ERROR_OUT_OF_MEMORY;
    }
    TEE_MemMove(input.buffer, params[PARAMS_IDX2].memref.buffer, params[PARAMS_IDX2].memref.size);
    signature.size = params[PARAMS_IDX3].memref.size;
    if (signature.size < RSA_2048_OUTPUT_SIZE) {
        return (PssMgf1SHA256SLogError(signature.size, input.buffer, 0, NUM_VALUE_1));
    }
    signature.buffer = (char *)TEE_Malloc(signature.size, 0);
    if (signature.buffer == NULL) {
        return (PssMgf1SHA256SLogError(0, input.buffer, 0, NUM_VALUE_2));
    }
    ret = CmdRSASignVerify(keyIndex, TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA256, &input, &signature, TEE_MODE_SIGN);
    if (ret != TEE_SUCCESS) {
        SLogError("CmdRSASignVerify failed, ret %x", ret);
    } else {
        TEE_MemMove(params[PARAMS_IDX3].memref.buffer, signature.buffer, signature.size);
        params[PARAMS_IDX3].memref.size = signature.size;
    }

    TEE_Free(input.buffer);
    TEE_Free(signature.buffer);
    return ret;
}

TEE_Result ParamCheckVerifyPssMgf1SHA256(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4], int numkey)
{
    switch (numkey) {
        case NUM_VALUE_0:
            if ((TEE_PARAM_TYPE_VALUE_INPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX0)) ||
                (TEE_PARAM_TYPE_MEMREF_INPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX2)) ||
                (TEE_PARAM_TYPE_MEMREF_INPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX3))) {
                SLogError("CMD_VERIFY_PSS_MGF1_SHA256: Bad expected parameter types.");
                return TEE_ERROR_BAD_PARAMETERS;
            }
            break;
        case NUM_VALUE_1:
            if (params[0].value.b != RSA_ALG_PSS_SHA256) {
                SLogError("algorithm is invalid %d", params[PARAMS_IDX0].value.b);
                return TEE_ERROR_BAD_PARAMETERS;
            }
            break;
        default:
            break;
    }
    return TEE_SUCCESS;
}

TEE_Result CmdVerifyPssMgf1SHA256(uint32_t paramTypes, TEE_Param params[PARAMS_IDX4])
{
    TEE_Result ret;
    Buffer input = {NULL, 0};
    Buffer signature = {NULL, 0};

    if ((ret = ParamCheckVerifyPssMgf1SHA256(paramTypes, params, 0)) != TEE_SUCCESS ||
        (ret = ParamCheckVerifyPssMgf1SHA256(paramTypes,  params, 1)) != TEE_SUCCESS) {
        return ret;
    }
    uint32_t keyIndex = params[PARAMS_IDX0].value.a;
    input.size = params[PARAMS_IDX2].memref.size;
    input.buffer = (char *)TEE_Malloc(input.size, 0);
    if (input.buffer == NULL) {
        SLogError("CmdVerifyPssMgf1SHA256 input malloc failed");
        return TEE_ERROR_OUT_OF_MEMORY;
    }
    TEE_MemMove(input.buffer, params[PARAMS_IDX2].memref.buffer, params[PARAMS_IDX2].memref.size);
    signature.size = params[PARAMS_IDX3].memref.size;
    if (signature.size != RSA_2048_OUTPUT_SIZE) {
        return PssMgf1SHA256SLogError(signature.size, input.buffer, 0, NUM_VALUE_1);
    }
    signature.buffer = (char *)TEE_Malloc(signature.size, 0);
    if (signature.buffer == NULL) {
        return PssMgf1SHA256SLogError(0, input.buffer, 0, NUM_VALUE_2);
    }
    TEE_MemMove(signature.buffer, params[PARAMS_IDX3].memref.buffer, params[PARAMS_IDX3].memref.size);
    ret = CmdRSASignVerify(keyIndex, TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA256, &input, &signature, TEE_MODE_VERIFY);
    if (ret != TEE_SUCCESS) {
        SLogError("CmdRSASignVerify failed, ret %x", ret);
    }

    TEE_Free(input.buffer);
    TEE_Free(signature.buffer);
    return TEE_SUCCESS;
}

TEE_Result TA_InvokeCommandEntryPoint(void *sessionContext, uint32_t cmdId,
                                      uint32_t paramTypes, TEE_Param params[PARAMS_IDX4])
{
    TEE_Result ret;

    SLogTrace("---- TA_InvokeCommandEntryPoint ----cmdid: %d------- ", cmdId);
    (void)sessionContext;
    switch (cmdId) {
        case CMD_GENERATE_RANDOM:
            ret = CmdGenerateRandom(paramTypes, params);
            break;

        case CMD_GENERATE_KEYPAIR:
            ret = CmdGeneratePair(paramTypes,  params);
            break;

        case CMD_SAVE_KEYPAIR:
            ret = CmdSaveKeyPair(paramTypes, params);
            break;

        case CMD_READ_KEYPAIR:
            ret = CmdReadKeyPair(paramTypes, params);
            break;

        case CMD_ENC_OAEP_MGF1_SHA512:
            ret = CmdEncOaepMgf1SHA512(paramTypes, params);
            break;

        case CMD_DEC_OAEP_MGF1_SHA512:
             ret = CmdDecOaepMgf1SHA512(paramTypes, params);
            break;

        case CMD_SIGN_PSS_MGF1_SHA256:
            ret = CmdSignPssMgf1SHA256(paramTypes, params);
            break;

        case CMD_VERIFY_PSS_MGF1_SHA256:
            ret = CmdVerifyPssMgf1SHA256(paramTypes, params);
            break;

        default:
            SLogError("Unknown CMD ID: %d", cmdId);
            ret = TEE_FAIL;
    }

    return  ret;
}

void TA_CloseSessionEntryPoint(void *sessionContext)
{
    (void)sessionContext;
    SLogTrace("---- TA_CloseSessionEntryPoint ----- ");
    if (g_rsaKeyObj1 != NULL)
    TEE_FreeTransientObject(g_rsaKeyObj1);
}

void TA_DestroyEntryPoint(void)
{
    SLogTrace("---- TA_DestroyEntryPoint ---- ");
}
