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
 * Description: cert_assign_ta.c
 * Create: 2022-08-15
 */
#include <string.h>
#include <sys/types.h>
#include "openssl/evp.h"
#include "openssl/sha.h"
#include "openssl/x509.h"
#include "openssl/pem.h"
#include "openssl/bn.h"
#include "tee_core_api.h"
#include "tee_defines.h"
#include "tee_log.h"
#include "tee_crypto_api.h"
#include "tee_object_api.h"
#include "cert_assign_ta.h"

#define RSA_KEY_SIZE 256
#define SERIAL_RAND_BITS 159

void DumpBuff(const char *buffer, const size_t bufferLen, const char *name)
{
    if (buffer == NULL || bufferLen == 0) {
        return;
    }
    SLog("-----------------------");
    SLog("%s len: %d", name, bufferLen);
    size_t size = 8;
    size_t i;
    for (i = 0; i < bufferLen / size * size; i += size) {
        SLog("%02x %02x %02x %02x %02x %02x %02x %02x",
             *(buffer + i), *(buffer + i + 1), *(buffer + i + 2), *(buffer + i + 3),
             *(buffer + i + 4), *(buffer + i + 5), *(buffer + i + 6), *(buffer + i + 7));
    }
    for (i = bufferLen /  size * size; i < bufferLen; i++) {
        SLog("%02x", *(buffer + i));
    }
    SLog("-----------------------\n");
    return;
}

typedef struct {
    char username[USERNAME_MAX];
} SessionIdentity;

// hashed password
const char HASHED_PASSWORD[HASHED_PASSWORD_MAX] = {
    131, 14, 145, 176, 89, 86, 218, 6, 40, 84, 71, 41, 10, 16, 225, 149, 157,
    173, 24, 163, 185, 159, 128, 211, 220, 126, 122, 191, 185, 13, 209, 251
};

// username, password, salt
const User g_user = {"admin", "TgnB54bLRChHVSNGc9sqwloIOUekaJtx", (char *)HASHED_PASSWORD};

TEE_Result TA_CreateEntryPoint(void)
{
    SLogTrace("----- TA_CreateEntryPoint ----- ");
    SLogTrace("TA version: %s ", TA_VERSION);

    // 验证CA程序路径和运行用户
    if (addcaller_ca_exec(CLIENT_APPLICATION_NAME, "root") != TEE_SUCCESS) {
        SLogError("TA_CreateEntryPoint: addcaller_ca_exec failed.");
        return TEE_ERROR_GENERIC;
    }
    SLogTrace("TA_CreateEntryPoint: addcaller_ca_exec ok.");
    return TEE_SUCCESS;
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE], void **sessionContext)
{
    TEE_Result ret = TEE_SUCCESS;
    SLogTrace("---- TA_OpenSessionEntryPoint -------- ");
    if (TEE_PARAM_TYPE_MEMREF_INPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX0)
        || TEE_PARAM_TYPE_MEMREF_INPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX1)) {
        SLogError("Invalid parameters.");
        return TEE_ERROR_BAD_PARAMETERS;
    }
    char *username = params[PARAMS_IDX0].memref.buffer;
    if (strncmp(username, g_user.username, USERNAME_MAX) != 0) {
        SLogError("Incorrect user name or password. Username: %s.", username);
        return TEE_ERROR_ACCESS_DENIED;
    }
    char *password = params[PARAMS_IDX1].memref.buffer;
    unsigned char *hashed_password = (unsigned char *)TEE_Malloc(HASHED_PASSWORD_MAX, 0);
    if (hashed_password == NULL) {
        SLogError("malloc failed.");
        return TEE_ERROR_GENERIC;
    }

    // 使用PBKDF2_HMAC算法生成密码哈希值
    if (PKCS5_PBKDF2_HMAC(password, params[PARAMS_IDX1].memref.size, (unsigned char *)g_user.salt, strlen(g_user.salt), HASH_ITER,
                          EVP_sha256(), HASHED_PASSWORD_MAX, hashed_password) == 0) {
        SLogError("Get hashed password failed.");
        TEE_Free(hashed_password);
        return TEE_ERROR_GENERIC;
    }

    // 校验密码哈希值是否正确
    if (memcmp(hashed_password, g_user.hashed_password, HASHED_PASSWORD_MAX) != 0) {
        SLogError("Incorrect user name or password. Username: %s.", username);
        TEE_Free(hashed_password);
        return TEE_ERROR_ACCESS_DENIED;
    }
    TEE_Free(hashed_password);
    SessionIdentity *identity = NULL;
    identity = (SessionIdentity *)TEE_Malloc(sizeof(SessionIdentity), 0);
    if (identity == NULL) {
        SLogError("Failed to allocate mem for identity.");
        return TEE_ERROR_GENERIC;
    }
    TEE_MemMove(identity->username, username, strlen(username));
    *sessionContext = (void *)identity;

    return ret;
}

// 生成自签名根证书
int SignRootCert(EVP_PKEY *caPkey, X509_REQ *req, X509 *x, const EVP_MD *md)
{
    SLogError("SignRootCert");
    ASN1_INTEGER *sno = ASN1_INTEGER_new();
    BIGNUM *btmp = BN_new();
    BN_rand(btmp, SERIAL_RAND_BITS, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);
    BN_to_ASN1_INTEGER(btmp, sno);

    X509_set_serialNumber(x, sno);
    X509_set_issuer_name(x, X509_REQ_get_subject_name(req));
    X509_set_subject_name(x, X509_REQ_get_subject_name(req));
    int days = 360;

    X509_gmtime_adj(X509_getm_notBefore(x), 0);
    X509_time_adj_ex(X509_getm_notAfter(x), days, 0, NULL);
    EVP_PKEY *pkey = X509_REQ_get0_pubkey(req);
    X509_set_pubkey(x, pkey);
    X509_sign(x, caPkey, md);
    return 0;
}

// 生成证书请求文件
int GenerateREQ(EVP_PKEY *pkey, X509_REQ *req, char *commonName, const EVP_MD *md)
{
    int ret;
    long version;
    X509_NAME *name;
    X509_NAME_ENTRY *entry = NULL;
    char mdout[32];
    unsigned int mdlen;

    version = 0;
    ret = X509_REQ_set_version(req, version);
    name = X509_NAME_new();
    int nid;
    nid = OBJ_txt2nid("C");
    unsigned long chtype = MBSTRING_ASC;
    X509_NAME_add_entry_by_NID(name, nid, chtype, (unsigned char *)"CN", 2, -1, 0);
    nid = OBJ_txt2nid("CN");
    X509_NAME_add_entry_by_NID(name, nid, chtype, (unsigned char *)commonName, strlen(commonName), -1, 0);

    // subject name
    ret = X509_REQ_set_subject_name(req, name);

    X509_NAME_free(name);
    X509_NAME_ENTRY_free(entry);

    ret = X509_REQ_set_pubkey(req, pkey);
    ret = X509_REQ_digest(req, md, (unsigned char *)mdout, &mdlen);
    ret = X509_REQ_sign(req, pkey, md);
    if (!ret) {
        SLogError("sign err!");
        return -1;
    }
    return 0;
}

// 生成RSA key
int GenRSA(RSA *rsa)
{
    int ret;
    unsigned long e = RSA_F4;
    BIGNUM *bne;
    bne = BN_new();
    ret = BN_set_word(bne, e);
    if (ret != 1) {
        SLogError("BN_set_word err!");
        return -1;
    }
    ret = RSA_generate_key_ex(rsa, 2048, bne, NULL);
    if (ret != 1) {
        SLogError("RSA_generate_key_ex err!");
        return -1;
    }
    return 0;
}

// 生成SM2 key
int GenSM2(EC_KEY *eckey)
{
    EC_GROUP *group = NULL;
    int nid = NID_sm2;
    group = EC_GROUP_new_by_curve_name(nid);
    EC_KEY_set_group(eckey, group);
    EC_KEY_generate_key(eckey);
    return 0;
}

// 获取根证书是否存在
TEE_Result GetRootCertState(SessionIdentity *identity)
{
    SLogTrace("---- GetRootCertState------- ");
    TEE_Result ret;
    char rootCertPath[SEC_STORAGE_PATH_MAX] = {0};
    char rootKeyPath[SEC_STORAGE_PATH_MAX] = {0};
    sprintf(rootCertPath, "%s/%s/root_cert.pem", SEC_STORAGE_PATH, identity->username);
    sprintf(rootKeyPath, "%s/%s/root_key.pem", SEC_STORAGE_PATH, identity->username);
    TEE_ObjectHandle object = NULL;
    ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, rootCertPath, strlen(rootCertPath), TEE_DATA_FLAG_ACCESS_READ, &object);
    TEE_CloseObject(object);
    if (ret != TEE_SUCCESS) {
        return TEE_ERROR_GENERIC;
    }
    ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, rootKeyPath, strlen(rootKeyPath), TEE_DATA_FLAG_ACCESS_READ, &object);
    TEE_CloseObject(object);
    if (ret != TEE_SUCCESS) {
        return TEE_ERROR_GENERIC;
    }
    return TEE_SUCCESS;
}

// 调用TEE安全存储接口保存数据
TEE_Result TEESaveData(char *buffer, size_t bufferLen, char *path) {
    TEE_Result ret;
    TEE_ObjectHandle object = NULL;
    ret = TEE_CreatePersistentObject(TEE_OBJECT_STORAGE_PRIVATE, path, strlen(path), TEE_DATA_FLAG_ACCESS_WRITE,
                                     TEE_HANDLE_NULL, NULL, 0, &object);
    if (ret != TEE_SUCCESS) {
        SLogError("TEESaveData: Failed to invoke TEE_CreatePersistentObject. ret: %x", ret);
        return TEE_ERROR_GENERIC;
    }
    ret = TEE_WriteObjectData(object, (void *)buffer, bufferLen);
    if (ret != TEE_SUCCESS) {
        SLogError("TEESaveData: Failed to invoke TEE_WriteObjectData. ret: %x", ret);
        TEE_CloseObject(object);
        return TEE_ERROR_GENERIC;
    }
    TEE_CloseObject(object);
    return TEE_SUCCESS;
}

// 调用TEE安全存储接口读取数据
TEE_Result TEEReadData(char *path, char *buffer, uint32_t *bufferLen)
{
    TEE_Result ret;
    TEE_ObjectHandle object = NULL;
    ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, path, strlen(path), TEE_DATA_FLAG_ACCESS_READ, &object);
    if (ret != TEE_SUCCESS) {
        SLogError("TEEReadData: Failed to invoke TEE_OpenPersistentObject. ret: %x", ret);
        return TEE_ERROR_GENERIC;
    }
    ret = TEE_ReadObjectData(object, (void *)buffer, *bufferLen, bufferLen);
    if (ret != TEE_SUCCESS) {
        TEE_CloseObject(object);
        SLogError("TEEReadData: Failed to invoke TEE_ReadObjectData. ret: %x", ret);
        return TEE_ERROR_GENERIC;
    }
    TEE_CloseObject(object);
    return TEE_SUCCESS;
}

// 创建自签名根证书
TEE_Result CreateRootCert(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE],
                          SessionIdentity *identity)
{
    SLogTrace("---- CreateRootCert------- ");
    if (TEE_PARAM_TYPE_MEMREF_INPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX0)
        || TEE_PARAM_TYPE_MEMREF_INPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX1)
        || TEE_PARAM_TYPE_MEMREF_OUTPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX2)) {
        SLogError("Invalid parameters.");
        return TEE_ERROR_BAD_PARAMETERS;
    }
    TEE_Result ret;
    char *commonName = params[PARAMS_IDX0].memref.buffer;
    char *cipher = params[PARAMS_IDX1].memref.buffer;
    EVP_PKEY *pkey = EVP_PKEY_new();
    X509_REQ *req = X509_REQ_new();
    X509 *x = X509_new();
    BIO *keyBIO = BIO_new(BIO_s_mem());
    BIO *certBIO = BIO_new(BIO_s_mem());
    char rootCertPath[SEC_STORAGE_PATH_MAX] = {0};
    char rootKeyPath[SEC_STORAGE_PATH_MAX] = {0};
    sprintf(rootCertPath, "%s/%s/root_cert.pem", SEC_STORAGE_PATH, identity->username);
    sprintf(rootKeyPath, "%s/%s/root_key.pem", SEC_STORAGE_PATH, identity->username);
    if (strcmp(cipher, "RSA") == 0) {
        // 生成RSA key
        RSA *rsa = RSA_new();
        GenRSA(rsa);
        EVP_PKEY_assign_RSA(pkey, rsa);
        // 生成证书请求文件
        GenerateREQ(pkey, req, commonName, EVP_sha256());
        // 生成自签名根证书
        SignRootCert(pkey, req, x, EVP_sha256());
        // 将RSA key写入BIO
        PEM_write_bio_RSAPrivateKey(keyBIO, rsa, NULL, NULL, 0, NULL, NULL);
        RSA_free(rsa);
        ret = TEE_SUCCESS;
    } else if (strcmp(cipher, "SM2") == 0) {
        // 生成SM2 key
        EC_KEY *eckey = EC_KEY_new();
        GenSM2(eckey);
        EVP_PKEY_assign_EC_KEY(pkey, eckey);
        // 生成证书请求文件
        GenerateREQ(pkey, req, commonName, EVP_sha256());
        // 生成自签名根证书
        SignRootCert(pkey, req, x, EVP_sha256());
        // 将SM2 key写入BIO
        PEM_write_bio_ECPrivateKey(keyBIO, eckey, NULL, NULL, 0, NULL, NULL);
        EC_KEY_free(eckey);
        ret = TEE_SUCCESS;
    } else {
        SLogError("Invalid parameters. cipher: %s", cipher);
        ret = TEE_ERROR_GENERIC;
        goto end;
    }

    // 保存密钥对
    size_t keyBufferLen = BIO_ctrl_pending(keyBIO);
    char *keyBuffer = (char *)TEE_Malloc(keyBufferLen, 0);
    BIO_read(keyBIO, keyBuffer, keyBufferLen);
    if (TEESaveData(keyBuffer, keyBufferLen, rootKeyPath) != TEE_SUCCESS) {
        ret = TEE_ERROR_GENERIC;
        TEE_Free(keyBuffer);
        goto end;
    }
    TEE_Free(keyBuffer);

    // 保存根证书
    PEM_write_bio_X509(certBIO, x);
    size_t certBufferLen = BIO_ctrl_pending(certBIO);
    char *certBuffer = (char *)TEE_Malloc(certBufferLen, 0);
    BIO_read(certBIO, certBuffer, certBufferLen);
    DumpBuff(certBuffer, certBufferLen, "root_cert");
    if (TEESaveData(certBuffer, certBufferLen, rootCertPath) != TEE_SUCCESS) {
        ret = TEE_ERROR_GENERIC;
        TEE_Free(certBuffer);
        goto end;
    }

    // 根证书数据返回给CA
    TEE_MemMove(params[PARAMS_IDX2].memref.buffer, certBuffer, certBufferLen);
    params[PARAMS_IDX2].memref.size = certBufferLen;
    TEE_Free(certBuffer);

end:
    // 释放资源
    BIO_free(keyBIO);
    BIO_free(certBIO);
    EVP_PKEY_free(pkey);
    X509_free(x);
    return ret;
}

// 查询根证书信息
TEE_Result ShowRootCert(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE],
                        SessionIdentity *identity)
{
    SLogTrace("---- ShowRootCert------- ");
    if (TEE_PARAM_TYPE_MEMREF_OUTPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX0)) {
        SLogError("Invalid parameters.");
        return TEE_ERROR_BAD_PARAMETERS;
    }
    // 读取根证书并写入BIO
    char rootCertPath[SEC_STORAGE_PATH_MAX] = {0};
    sprintf(rootCertPath, "%s/%s/root_cert.pem", SEC_STORAGE_PATH, identity->username);
    BIO *rootCertBIO = BIO_new(BIO_s_mem());
    char rootCertBuffer[PEM_BUFFER_LEN] = {0};
    uint32_t rootCertBufferLen = PEM_BUFFER_LEN;
    if (TEEReadData(rootCertPath, rootCertBuffer, &rootCertBufferLen) != TEE_SUCCESS) {
        SLogError("Failed to read root cert.");
        BIO_free(rootCertBIO);
        return TEE_ERROR_GENERIC;
    }
    BIO_write(rootCertBIO, rootCertBuffer, rootCertBufferLen);

    // 生成X509
    X509 *xca;
    xca = PEM_read_bio_X509(rootCertBIO, NULL, 0, NULL);
    if (xca == NULL) {
        SLogError("Failed to read root cert.");
        BIO_free(rootCertBIO);
        return TEE_ERROR_GENERIC;
    }

    // 获取证书信息写入BIO
    BIO *out = BIO_new(BIO_s_mem());
    X509_print_ex(out, xca, XN_FLAG_ONELINE, 0);
    size_t bufferLen = BIO_ctrl_pending(out);
    char *buffer = (char *)TEE_Malloc(bufferLen, 0);
    BIO_read(out, buffer, bufferLen);

    // 证书信息返回给CA
    TEE_MemMove(params[PARAMS_IDX0].memref.buffer, buffer, bufferLen);
    params[PARAMS_IDX0].memref.size = bufferLen;

    // 释放资源
    TEE_Free(buffer);
    X509_free(xca);
    BIO_free(rootCertBIO);
    BIO_free(out);
    return TEE_SUCCESS;
}

// 调用OpenSSL函数进行签名
static int do_sign_init(EVP_MD_CTX *ctx, EVP_PKEY *pkey, const EVP_MD *md)
{
    EVP_PKEY_CTX *pkctx = NULL;
    int def_nid;

    if (ctx == NULL)
        return 0;
    /*
     * EVP_PKEY_get_default_digest_nid() returns 2 if the digest is mandatory
     * for this algorithm.
     */
    if (EVP_PKEY_get_default_digest_nid(pkey, &def_nid) == 2
            && def_nid == NID_undef) {
        /* The signing algorithm requires there to be no digest */
        md = NULL;
    }
    if (!EVP_DigestSignInit(ctx, &pkctx, md, NULL, pkey))
        return 0;
    return 1;
}

int do_X509_sign(X509 *x, EVP_PKEY *pkey, const EVP_MD *md)
{
    int rv;
    EVP_MD_CTX *mctx = EVP_MD_CTX_new();

    rv = do_sign_init(mctx, pkey, md);
    if (rv > 0)
        rv = X509_sign_ctx(x, mctx);
    EVP_MD_CTX_free(mctx);
    return rv > 0 ? 1 : 0;
}

static int X509_certify(const EVP_MD *digest,
                        X509 *x, X509 *xca, EVP_PKEY *pkey, int days)
{
    int ret = 0;
    ASN1_INTEGER *sno = NULL;
    EVP_PKEY *upkey;

    upkey = X509_get0_pubkey(xca);
    if (upkey == NULL) {
        SLogError("Error obtaining CA X509 public key.");
        goto end;
    }
    EVP_PKEY_copy_parameters(upkey, pkey);

    sno = ASN1_INTEGER_new();

    BIGNUM *btmp = BN_new();
    BN_rand(btmp, SERIAL_RAND_BITS, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);
    BN_to_ASN1_INTEGER(btmp, sno);

    if (!X509_check_private_key(xca, pkey)) {
        SLogError("CA certificate and CA private key do not match.");
        goto end;
    }

    if (!X509_set_issuer_name(x, X509_get_subject_name(xca)))
        goto end;
    if (!X509_set_serialNumber(x, sno))
        goto end;

    X509_gmtime_adj(X509_getm_notBefore(x), 0);
    X509_time_adj_ex(X509_getm_notAfter(x), days, 0, NULL);

    if (!do_X509_sign(x, pkey, digest))
        goto end;
    ret = 1;
 end:
    if (!ret)
        SLogError("Failed to verify certificate.");
    if (sno)
        ASN1_INTEGER_free(sno);
    return ret;
}

int X509SignReq(X509 *x, EVP_PKEY *CApkey, X509_REQ *req, X509 *xca)
{
    int days = 360;

    if (!X509_set_issuer_name(x, X509_REQ_get_subject_name(req)))
        return 1;
    if (!X509_set_subject_name(x, X509_REQ_get_subject_name(req)))
        return 1;
    X509_gmtime_adj(X509_getm_notBefore(x), 0);
    X509_time_adj_ex(X509_getm_notAfter(x), days, 0, NULL);

    EVP_PKEY *pkey;
    pkey = X509_REQ_get0_pubkey(req);
    X509_set_pubkey(x, pkey);
    if (X509_certify(EVP_sha256(), x, xca, CApkey, days) != 1) {
        return 1;
    }
    return 0;
}

// 使用根证书和根密钥对证书请求文件进行签名
TEE_Result SignX509Cert(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE],
                        SessionIdentity *identity)
{
    SLogTrace("---- SignX509Cert------- ");
    if (TEE_PARAM_TYPE_MEMREF_INPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX0)
        || TEE_PARAM_TYPE_MEMREF_OUTPUT != TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX1)) {
        SLogError("Invalid parameters.");
        return TEE_ERROR_BAD_PARAMETERS;
    }
    TEE_Result ret = TEE_ERROR_GENERIC;
    char rootCertPath[SEC_STORAGE_PATH_MAX] = {0};
    char rootKeyPath[SEC_STORAGE_PATH_MAX] = {0};
    sprintf(rootCertPath, "%s/%s/root_cert.pem", SEC_STORAGE_PATH, identity->username);
    sprintf(rootKeyPath, "%s/%s/root_key.pem", SEC_STORAGE_PATH, identity->username);
    char rootCertBuffer[PEM_BUFFER_LEN] = {0};
    uint32_t rootCertBufferLen = PEM_BUFFER_LEN;
    if (TEEReadData(rootCertPath, rootCertBuffer, &rootCertBufferLen) != TEE_SUCCESS) {
        SLogError("Failed to read root cert.");
        return ret;
    }
    char rootKeyBuffer[PEM_BUFFER_LEN] = {0};
    uint32_t rootKeyBufferLen = PEM_BUFFER_LEN;
    if (TEEReadData(rootKeyPath, rootKeyBuffer, &rootKeyBufferLen) != TEE_SUCCESS) {
        SLogError("Failed to read root key.");
        return ret;
    }
    BIO *rootKeyBIO = BIO_new(BIO_s_mem());
    BIO *rootCertBIO = BIO_new(BIO_s_mem());
    BIO_write(rootCertBIO, rootCertBuffer, rootCertBufferLen);
    BIO_write(rootKeyBIO, rootKeyBuffer, rootKeyBufferLen);
    char *reqBuffer = params[PARAMS_IDX0].memref.buffer;
    size_t reqBufferLen = params[PARAMS_IDX0].memref.size;
    BIO *certBIO = BIO_new(BIO_s_mem());
    BIO *reqBIO = BIO_new(BIO_s_mem());
    BIO_write(reqBIO, reqBuffer, reqBufferLen);

    X509_REQ *req = NULL;
    EVP_PKEY *pkey = NULL;
    EVP_PKEY *CApkey = NULL;
    EVP_PKEY *reqPkey = NULL;
    X509 *x = NULL;
    X509 *xca = NULL;

    req = PEM_read_bio_X509_REQ(reqBIO, NULL, NULL, NULL);
    if (req == NULL) {
        SLogError("Failed to read certificate signing request.");
        goto end;
    }

    CApkey = PEM_read_bio_PrivateKey(rootKeyBIO, NULL, NULL, NULL);
    if (CApkey == NULL) {
        SLogError("Failed to read root key.");
        goto end;
    }

    xca = PEM_read_bio_X509(rootCertBIO, NULL, 0, NULL);
    if (xca == NULL) {
        SLogError("Failed to read root cert.");
        goto end;
    }

    if ((reqPkey = X509_REQ_get0_pubkey(req)) == NULL) {
        SLogError("Failed to get public key.");
        goto end;
    }
    int i = X509_REQ_verify(req, reqPkey);
    if (i < 0) {
        SLogError("Signature verification error.");
        goto end;
    }
    if (i == 0) {
        SLogError("Signature did not match the certificate request.");
        goto end;
    }

    x = X509_new();
    if (X509SignReq(x, CApkey, req, xca) != 0) {
        goto end;
    }

    PEM_write_bio_X509(certBIO, x);
    size_t bufferLen = BIO_ctrl_pending(certBIO);
    char *buffer = (char *)TEE_Malloc(bufferLen, 0);
    BIO_read(certBIO, buffer, bufferLen);
    DumpBuff(buffer, bufferLen, "cert");
    TEE_MemMove(params[PARAMS_IDX1].memref.buffer, buffer, bufferLen);
    params[PARAMS_IDX1].memref.size = bufferLen;
    TEE_Free(buffer);
    ret = TEE_SUCCESS;

end:
    if (CApkey != NULL) {
        EVP_PKEY_free(CApkey);
    }
    if (reqPkey != NULL) {
        EVP_PKEY_free(reqPkey);
    }
    if (pkey != NULL) {
        EVP_PKEY_free(pkey);
    }
    if (x != NULL) {
        X509_free(x);
    }
    if (xca != NULL) {
        X509_free(xca);
    }
    if (req != NULL) {
        X509_REQ_free(req);
    }
    BIO_free(certBIO);
    BIO_free(reqBIO);
    BIO_free(rootCertBIO);
    BIO_free(rootKeyBIO);
    return ret;
}

TEE_Result TA_InvokeCommandEntryPoint(void *sessionContext, uint32_t cmdId,
                                      uint32_t paramTypes, TEE_Param params[PARAMS_SIZE])
{
    TEE_Result ret;

    SLogTrace("---- TA_InvokeCommandEntryPoint ----cmdid: %d------- ", cmdId);
    SessionIdentity *identity = (SessionIdentity *)sessionContext;
    if (strncmp((char *)identity->username, (char *)g_user.username, USERNAME_MAX) != 0) {
        return TEE_ERROR_ACCESS_DENIED;
    }
    switch (cmdId) {
        case CMD_GET_ROOT_CERT_STATE:
            ret = GetRootCertState(identity);
            break;

        case CMD_CREAT_ROOT_CERT:
            ret = CreateRootCert(paramTypes, params, identity);
            break;

        case CMD_SHOW_ROOT_CERT:
            ret = ShowRootCert(paramTypes, params, identity);
            break;

        case CMD_SIGN_X509_CERT:
            ret = SignX509Cert(paramTypes, params, identity);
            break;

        default:
            SLogError("Unknown CMD ID: %d", cmdId);
            ret = TEE_FAIL;
    }

    return ret;
}

void TA_CloseSessionEntryPoint(void *sessionContext)
{
    TEE_Free(sessionContext);
    SLogTrace("---- TA_CloseSessionEntryPoint ----- ");
}

void TA_DestroyEntryPoint(void)
{
    SLogTrace("---- TA_DestroyEntryPoint ---- ");
}
