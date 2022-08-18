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
    (void)paramTypes;
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

    if (PKCS5_PBKDF2_HMAC(password, params[PARAMS_IDX1].memref.size, (unsigned char *)g_user.salt, strlen(g_user.salt), HASH_ITER,
                          EVP_sha256(), HASHED_PASSWORD_MAX, hashed_password) == 0) {
        SLogError("Get hashed password failed.");
        return TEE_ERROR_GENERIC;
    }
    DumpBuff((char *)hashed_password, HASHED_PASSWORD_MAX, "hashed_password");
    if (memcmp(hashed_password, g_user.hashed_password, HASHED_PASSWORD_MAX) != 0) {
        SLogError("Incorrect user name or password. Username: %s.", username);
        return TEE_ERROR_ACCESS_DENIED;
    }
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

#define RSA_KEY_SIZE 256

# define SERIAL_RAND_BITS 159

int SignRootCert(EVP_PKEY *caPkey, X509_REQ *req, X509 *x, const EVP_MD *md)
{
    SLogError("X509Sign");
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
        SLogError("sign err!\n");
        return -1;
    }
    SLogError("GenerateREQ end");
    return 0;
}

int GenRSA(RSA *rsa)
{
    int ret;
    unsigned long e = RSA_F4;
    BIGNUM *bne;
    bne = BN_new();
    ret = BN_set_word(bne, e);
    if (ret != 1) {
        SLogError("BN_set_word err!\n");
        return -1;
    }
    SLogError("RSA_generate_key_ex\n");
    ret = RSA_generate_key_ex(rsa, 4096, bne, NULL);
    if (ret != 1) {
        SLogError("RSA_generate_key_ex err!\n");
        return -1;
    }
    return 0;
}

int GenSM2(EC_KEY *eckey)
{
    EC_GROUP *group = NULL;
    int nid = NID_sm2;
    group = EC_GROUP_new_by_curve_name(nid);
    EC_KEY_set_group(eckey, group);
    EC_KEY_generate_key(eckey);
    return 0;
}

TEE_Result GetCertState(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE],
                        SessionIdentity *identity)
{
    SLogTrace("---- CmdCertState------- ");
    TEE_Result ret;
    char rootCertPath[SEC_STORAGE_PATH_MAX] = {0};
    char rootKeyPath[SEC_STORAGE_PATH_MAX] = {0};
    sprintf(rootCertPath, "%s/%s/cert.pem", SEC_STORAGE_PATH, identity->username);
    sprintf(rootKeyPath, "%s/%s/key.pem", SEC_STORAGE_PATH, identity->username);
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

TEE_Result SaveData(char *buffer, size_t bufferLen, char *path) {
    TEE_Result ret;
    TEE_ObjectHandle object = NULL;
    ret = TEE_CreatePersistentObject(TEE_OBJECT_STORAGE_PRIVATE, path, strlen(path), TEE_DATA_FLAG_ACCESS_WRITE,
                                     TEE_HANDLE_NULL, NULL, 0, &object);
    if (ret != TEE_SUCCESS) {
        SLogError("SaveKeypair: Failed to invoke TEE_CreatePersistentObject. ret: %x", ret);
        return TEE_ERROR_GENERIC;
    }
    ret = TEE_WriteObjectData(object, (void *)buffer, bufferLen);
    if (ret != TEE_SUCCESS) {
        SLogError("SaveKeypair: Failed to invoke TEE_WriteObjectData. ret: %x", ret);
        TEE_CloseObject(object);
        return TEE_ERROR_GENERIC;
    }
    TEE_CloseObject(object);
    return TEE_SUCCESS;
}

int SaveKeypair(BIO *bio, char *path) {
    int ret = 0;
    size_t bufferLen = BIO_ctrl_pending(bio);
    SLogError("%d\n", bufferLen);
    char *buffer = (char *)TEE_Malloc(bufferLen, 0);
    BIO_read(bio, buffer, bufferLen);
    DumpBuff(buffer, bufferLen, "Keypair");
    if (SaveData(buffer, bufferLen, path) != TEE_SUCCESS) {
        ret = 1;
    }
    TEE_Free(buffer);
    return ret;
}

TEE_Result CreateRootCert(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE],
                          SessionIdentity *identity)
{
    SLogTrace("---- CreateRootCert------- ");
    TEE_Result ret;
    char *commonName = params[PARAMS_IDX0].memref.buffer;
    char *cipher = params[PARAMS_IDX1].memref.buffer;
    EVP_PKEY *pkey = EVP_PKEY_new();
    X509_REQ *req = X509_REQ_new();
    X509 *x = X509_new();
    BIO *keyBIO = BIO_new(BIO_s_mem());
    char rootCertPath[SEC_STORAGE_PATH_MAX] = {0};
    char rootKeyPath[SEC_STORAGE_PATH_MAX] = {0};
    sprintf(rootCertPath, "%s/%s/cert.pem", SEC_STORAGE_PATH, identity->username);
    sprintf(rootKeyPath, "%s/%s/key.pem", SEC_STORAGE_PATH, identity->username);
    if (strcmp(cipher, "RSA") == 0) {
        RSA *rsa = RSA_new();
        GenRSA(rsa);
        EVP_PKEY_assign_RSA(pkey, rsa);
        GenerateREQ(pkey, req, commonName, EVP_sha256());
        SignRootCert(pkey, req, x, EVP_sha256());
        // PEM_write_bio_RSAPrivateKey(keyBIO, rsa, EVP_aes_256_cbc(), (unsigned char *)"admin", 5, NULL, NULL);
        PEM_write_bio_RSAPrivateKey(keyBIO, rsa, NULL, NULL, 0, NULL, NULL);
        RSA_free(rsa);
        ret = TEE_SUCCESS;
    } else if (strcmp(cipher, "SM2") == 0) {
        EC_KEY *eckey = EC_KEY_new();
        GenSM2(eckey);
        EVP_PKEY_assign_EC_KEY(pkey, eckey);
        GenerateREQ(pkey, req, commonName, EVP_sha256());
        SignRootCert(pkey, req, x, EVP_sha256());
        // PEM_write_bio_ECPrivateKey(keyBIO, eckey, EVP_sm4_cbc(), (unsigned char *)"admin", 5, NULL, NULL);
        PEM_write_bio_ECPrivateKey(keyBIO, eckey, NULL, NULL, 0, NULL, NULL);
        EC_KEY_free(eckey);
        ret = TEE_SUCCESS;
    } else {
        SLogError("Invalid parameters. cipher: %s", cipher);
        ret = TEE_ERROR_GENERIC;
    }
    SaveKeypair(keyBIO, rootKeyPath);
    BIO_free(keyBIO);
    SLogError("certBIO");
    BIO *certBIO = BIO_new(BIO_s_mem());
    PEM_write_bio_X509(certBIO, x);
    // PEM_write_bio_X509_REQ(certBIO, req);
    size_t bufferLen = BIO_ctrl_pending(certBIO);
    SLogError("%d\n", bufferLen);
    char *buffer = (char *)TEE_Malloc(bufferLen, 0);
    BIO_read(certBIO, buffer, bufferLen);
    DumpBuff(buffer, bufferLen, "cert");
    SaveData(buffer, bufferLen, rootCertPath);
    SLogError("TEE_MemMove");
    TEE_MemMove(params[PARAMS_IDX2].memref.buffer, buffer, bufferLen);
    params[PARAMS_IDX2].memref.size = bufferLen;
    TEE_Free(buffer);
    SLogError("BIO_free");
    BIO_free(certBIO);
    SLogError("EVP_PKEY_free");
    EVP_PKEY_free(pkey);
    // SLogError("X509_REQ_free");
    // X509_REQ_free(req);
    SLogError("X509_free");
    X509_free(x);
    SLogError("free");
    return ret;
}

int pass_cb(char *buf, int size, int rwflag, void *u)
{
    char *tmp = (char *)u;
    if (tmp == NULL)
        return -1;

    int len = strlen(tmp);

    if (len > size)
        len = size;
    memcpy(buf, tmp, len);
    return len;
}

int pkey_ctrl_string(EVP_PKEY_CTX *ctx, const char *value)
{
    int rv;
    char *stmp, *vtmp = NULL;
    stmp = OPENSSL_strdup(value);
    if (!stmp)
        return -1;
    vtmp = strchr(stmp, ':');
    if (vtmp) {
        *vtmp = 0;
        vtmp++;
    }
    rv = EVP_PKEY_CTX_ctrl_str(ctx, stmp, vtmp);
    OPENSSL_free(stmp);
    return rv;
}

static int do_sign_init(EVP_MD_CTX *ctx, EVP_PKEY *pkey,
                        const EVP_MD *md, STACK_OF(OPENSSL_STRING) *sigopts)
{
    EVP_PKEY_CTX *pkctx = NULL;
    int i, def_nid;

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
    for (i = 0; i < sk_OPENSSL_STRING_num(sigopts); i++) {
        char *sigopt = sk_OPENSSL_STRING_value(sigopts, i);
        if (pkey_ctrl_string(pkctx, sigopt) <= 0) {
            printf("parameter error \"%s\"\n", sigopt);
            return 0;
        }
    }
    return 1;
}

int do_X509_sign(X509 *x, EVP_PKEY *pkey, const EVP_MD *md,
                 STACK_OF(OPENSSL_STRING) *sigopts)
{
    int rv;
    EVP_MD_CTX *mctx = EVP_MD_CTX_new();

    rv = do_sign_init(mctx, pkey, md, sigopts);
    if (rv > 0)
        rv = X509_sign_ctx(x, mctx);
    EVP_MD_CTX_free(mctx);
    return rv > 0 ? 1 : 0;
}

static int x509_certify(const EVP_MD *digest,
                        X509 *x, X509 *xca, EVP_PKEY *pkey,
                        STACK_OF(OPENSSL_STRING) *sigopts,
                        int days)
{
    int ret = 0;
    ASN1_INTEGER *sno = NULL;
    EVP_PKEY *upkey;

    upkey = X509_get0_pubkey(xca);
    if (upkey == NULL) {
        printf("Error obtaining CA X509 public key\n");
        goto end;
    }
    EVP_PKEY_copy_parameters(upkey, pkey);

    sno = ASN1_INTEGER_new();

    BIGNUM *btmp = BN_new();
    BN_rand(btmp, SERIAL_RAND_BITS, BN_RAND_TOP_ANY, BN_RAND_BOTTOM_ANY);
    BN_to_ASN1_INTEGER(btmp, sno);

    if (!X509_check_private_key(xca, pkey)) {
        printf("CA certificate and CA private key do not match\n");
        goto end;
    }

    if (!X509_set_issuer_name(x, X509_get_subject_name(xca)))
        goto end;
    if (!X509_set_serialNumber(x, sno))
        goto end;

    X509_gmtime_adj(X509_getm_notBefore(x), 0);
    X509_time_adj_ex(X509_getm_notAfter(x), days, 0, NULL);

    if (!do_X509_sign(x, pkey, digest, sigopts))
        goto end;
    ret = 1;
 end:
    if (!ret)
        printf("errrrrrrrr");
    if (sno)
        ASN1_INTEGER_free(sno);
    return ret;
}

int x509SignCsr(X509 *x, EVP_PKEY *CApkey, X509_REQ *req, X509 *xca)
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
    x509_certify(EVP_sha256(), x, xca, CApkey, NULL, days);
    return 0;
}

int ReadFile(char *path, char *buffer, uint32_t *bufferLen)
{
    TEE_Result ret;
    TEE_ObjectHandle object = NULL;
    ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, path, strlen(path), TEE_DATA_FLAG_ACCESS_READ, &object);
    if (ret != TEE_SUCCESS) {
        SLogError("DecryptVote: Failed to invoke TEE_OpenPersistentObject. ret: %x", ret);
        return TEE_ERROR_GENERIC;
    }
    ret = TEE_ReadObjectData(object, (void *)buffer, *bufferLen, bufferLen);
    if (ret != TEE_SUCCESS) {
        TEE_CloseObject(object);
        SLogError("DecryptVote: Failed to invoke TEE_ReadObjectData. ret: %x", ret);
        return TEE_ERROR_GENERIC;
    }
    TEE_CloseObject(object);
    return TEE_SUCCESS;
}

TEE_Result CmdSignCert(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE],
                       SessionIdentity *identity)
{
    SLogTrace("---- CmdSignCert------- ");
    char rootCertPath[SEC_STORAGE_PATH_MAX] = {0};
    char rootKeyPath[SEC_STORAGE_PATH_MAX] = {0};
    sprintf(rootCertPath, "%s/%s/cert.pem", SEC_STORAGE_PATH, identity->username);
    sprintf(rootKeyPath, "%s/%s/key.pem", SEC_STORAGE_PATH, identity->username);
    BIO *rootKeyBIO = BIO_new(BIO_s_mem());
    BIO *rootCertBIO = BIO_new(BIO_s_mem());
    char rootCertBuffer[PEM_BUFFER_LEN] = {0};
    uint32_t rootCertBufferLen = PEM_BUFFER_LEN;
    ReadFile(rootCertPath, rootCertBuffer, &rootCertBufferLen);
    BIO_write(rootCertBIO, rootCertBuffer, rootCertBufferLen);
    char rootKeyBuffer[PEM_BUFFER_LEN] = {0};
    uint32_t rootKeyBufferLen = PEM_BUFFER_LEN;
    ReadFile(rootKeyPath, rootKeyBuffer, &rootKeyBufferLen);
    BIO_write(rootKeyBIO, rootKeyBuffer, rootKeyBufferLen);
    char *csrBuffer = params[PARAMS_IDX0].memref.buffer;
    size_t csrBufferLen = params[PARAMS_IDX0].memref.size;
    BIO *certBIO = BIO_new(BIO_s_mem());
    BIO *csrBIO = BIO_new(BIO_s_mem());
    BIO_write(csrBIO, csrBuffer, csrBufferLen);
    X509_REQ *req;
    req = PEM_read_bio_X509_REQ(csrBIO, NULL, NULL, NULL);
    EVP_PKEY *pkey = NULL;

    EVP_PKEY *CApkey;
    // CApkey = PEM_read_bio_PrivateKey(keyBIO, NULL, pass_cb, "admin");
    CApkey = PEM_read_bio_PrivateKey(rootKeyBIO, NULL, NULL, NULL);
    if (CApkey == NULL) {
        printf("CApkey is NULL\n");
    }
    X509 *x = X509_new();

    X509 *xca;
    xca = PEM_read_bio_X509(rootCertBIO, NULL, 0, NULL);
    if (xca == NULL) {
        printf("xca is NULL\n");
    }
    EVP_PKEY *tmpPkey;
    if ((tmpPkey = X509_REQ_get0_pubkey(req)) == NULL) {
        printf("error unpacking public key\n");
        goto end;
    }
    int i = X509_REQ_verify(req, tmpPkey);
    if (i < 0) {
        printf("Signature verification error\n");
        goto end;
    }
    if (i == 0) {
        printf("Signature did not match the certificate request\n");
        goto end;
    } else {
        printf("Signature ok\n");
    }

    x509SignCsr(x, CApkey, req, xca);

    PEM_write_bio_X509(certBIO, x);
    size_t bufferLen = BIO_ctrl_pending(certBIO);
    char *buffer = (char *)TEE_Malloc(bufferLen, 0);
    BIO_read(certBIO, buffer, bufferLen);
    DumpBuff(buffer, bufferLen, "cert");
    SLogError("TEE_MemMove");
    TEE_MemMove(params[PARAMS_IDX1].memref.buffer, buffer, bufferLen);
    params[PARAMS_IDX1].memref.size = bufferLen;
    TEE_Free(buffer);

end:
    EVP_PKEY_free(CApkey);
    X509_free(x);
    BIO_free(certBIO);
    BIO_free(csrBIO);
    BIO_free(rootCertBIO);
    BIO_free(rootKeyBIO);
    EVP_PKEY_free(pkey);
    return TEE_SUCCESS;
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
            ret = GetCertState(paramTypes, params, identity);
            break;

        case CMD_CREAT_ROOT_CERT:
            ret = CreateRootCert(paramTypes, params, identity);
            break;

        case CMD_SIGN_X509_CERT:
            ret = CmdSignCert(paramTypes, params, identity);
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
