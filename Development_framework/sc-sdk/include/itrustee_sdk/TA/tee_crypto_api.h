/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: tee crypto definitions
 */

#ifndef TEE_CRYPTO_API_H
#define TEE_CRYPTO_API_H

#include <pthread.h> /* pthread_mutex_t */
#include <tee_defines.h>
#include <tee_mem_mgmt_api.h>

#define TEE_MAX_KEY_SIZE_IN_BITS      (1024 * 8)
#define SW_RSA_KEYLEN                 1024
#define TEE_DH_MAX_SIZE_OF_OTHER_INFO 64 /* bytes */
#define TEE_PARAM_COUNT_MAX           9

enum __TEE_Operation_Constants {
    TEE_OPERATION_CIPHER               = 0x1,
    TEE_OPERATION_MAC                  = 3,
    TEE_OPERATION_AE                   = 4,
    TEE_OPERATION_DIGEST               = 5,
    TEE_OPERATION_ASYMMETRIC_CIPHER    = 6,
    TEE_OPERATION_ASYMMETRIC_SIGNATURE = 7,
    TEE_OPERATION_KEY_DERIVATION       = 8,
    TEE_OPERATION_KDF_KEY_DERIVATION   = 9,
};

enum __tee_crypto_algorithm_id {
    TEE_ALG_INVALID                      = 0x0,
    TEE_ALG_AES_ECB_NOPAD                = 0x10000010,
    TEE_ALG_AES_CBC_NOPAD                = 0x10000110,
    TEE_ALG_AES_CTR                      = 0x10000210,
    TEE_ALG_AES_CTS                      = 0x10000310,
    TEE_ALG_AES_XTS                      = 0x10000410,
    TEE_ALG_AES_CBC_MAC_NOPAD            = 0x30000110,
    TEE_ALG_AES_CBC_MAC_PKCS5            = 0x30000510,
    TEE_ALG_AES_CMAC                     = 0x30000610,
    TEE_ALG_AES_GMAC                     = 0x30000810,
    TEE_ALG_AES_CCM                      = 0x40000710,
    TEE_ALG_AES_GCM                      = 0x40000810,
    TEE_ALG_DES_ECB_NOPAD                = 0x10000011,
    TEE_ALG_DES_CBC_NOPAD                = 0x10000111,
    TEE_ALG_DES_CBC_MAC_NOPAD            = 0x30000111,
    TEE_ALG_DES_CBC_MAC_PKCS5            = 0x30000511,
    TEE_ALG_DES3_ECB_NOPAD               = 0x10000013,
    TEE_ALG_DES3_CBC_NOPAD               = 0x10000113,
    TEE_ALG_DES3_CBC_MAC_NOPAD           = 0x30000113,
    TEE_ALG_DES3_CBC_MAC_PKCS5           = 0x30000513,
    TEE_ALG_RSASSA_PKCS1_V1_5_MD5        = 0x70001830,
    TEE_ALG_RSASSA_PKCS1_V1_5_SHA1       = 0x70002830,
    TEE_ALG_RSASSA_PKCS1_V1_5_SHA224     = 0x70003830,
    TEE_ALG_RSASSA_PKCS1_V1_5_SHA256     = 0x70004830,
    TEE_ALG_RSASSA_PKCS1_V1_5_SHA384     = 0x70005830,
    TEE_ALG_RSASSA_PKCS1_V1_5_SHA512     = 0x70006830,
    TEE_ALG_RSASSA_PKCS1_PSS_MGF1_MD5    = 0x70111930,
    TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA1   = 0x70212930,
    TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA224 = 0x70313930,
    TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA256 = 0x70414930,
    TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA384 = 0x70515930,
    TEE_ALG_RSASSA_PKCS1_PSS_MGF1_SHA512 = 0x70616930,
    TEE_ALG_RSAES_PKCS1_V1_5             = 0x60000130,
    TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA1   = 0x60210230,
#ifdef GP_SUPPORT
    TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA224 = 0x60310230,
    TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA256 = 0x60410230,
    TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA384 = 0x60510230,
    TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512 = 0x60610230,
#else
    TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA224 = 0x60211230,
    TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA256 = 0x60212230,
    TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA384 = 0x60213230,
    TEE_ALG_RSAES_PKCS1_OAEP_MGF1_SHA512 = 0x60214230,
#endif
    TEE_ALG_RSA_NOPAD                    = 0x60000030,
    TEE_ALG_DSA_SHA1                     = 0x70002131,
    TEE_ALG_DSA_SHA224                   = 0x70003131,
    TEE_ALG_DSA_SHA256                   = 0x70004131,
    TEE_ALG_DH_DERIVE_SHARED_SECRET      = 0x80000032,
    TEE_ALG_MD5                          = 0x50000001,
    TEE_ALG_SHA1                         = 0x50000002,
    TEE_ALG_SHA224                       = 0x50000003,
    TEE_ALG_SHA256                       = 0x50000004,
    TEE_ALG_SHA384                       = 0x50000005,
    TEE_ALG_SHA512                       = 0x50000006,
    TEE_ALG_HMAC_MD5                     = 0x30000001,
    TEE_ALG_HMAC_SHA1                    = 0x30000002,
    TEE_ALG_HMAC_SHA224                  = 0x30000003,
    TEE_ALG_HMAC_SHA256                  = 0x30000004,
    TEE_ALG_HMAC_SHA384                  = 0x30000005,
    TEE_ALG_HMAC_SHA512                  = 0x30000006,
    TEE_ALG_HMAC_SM3                     = 0x30000007,
    TEE_ALG_AES_ECB_PKCS5                = 0x10000020,
    TEE_ALG_AES_CBC_PKCS5                = 0x10000220,
    TEE_ALG_AES_CBC_ISO_PADDING          = 0x10000330,
    TEE_ALG_ECDSA_SHA1                   = 0x70001042,
    TEE_ALG_ECDSA_SHA224                 = 0x70002042,
    TEE_ALG_ECDSA_SHA256                 = 0x70003042,
    TEE_ALG_ECDSA_SHA384                 = 0x70004042,
    TEE_ALG_ECDSA_SHA512                 = 0x70005042,
    TEE_ALG_ED25519                      = 0x70005043,
    TEE_ALG_ECDH_DERIVE_SHARED_SECRET    = 0x80000042,
    TEE_ALG_X25519                       = 0x80000044,
    TEE_ALG_ECC                          = 0x80000001,
    TEE_ALG_ECDSA_P192                   = 0x70001042,
    TEE_ALG_ECDSA_P224                   = 0x70002042,
    TEE_ALG_ECDSA_P256                   = 0x70003042,
    TEE_ALG_ECDSA_P384                   = 0x70004042,
    TEE_ALG_ECDSA_P521                   = 0x70005042,
    TEE_ALG_ECDH_P192                    = 0x80001042,
    TEE_ALG_ECDH_P224                    = 0x80002042,
    TEE_ALG_ECDH_P256                    = 0x80003042,
    TEE_ALG_ECDH_P384                    = 0x80004042,
    TEE_ALG_ECDH_P521                    = 0x80005042,
    TEE_ALG_SIP_HASH                     = 0xF0000002,
    TEE_ALG_SM2_DSA_SM3                  = 0x70006045,
    TEE_ALG_SM2_PKE                      = 0x80000045,
    TEE_ALG_SM3                          = 0x50000007,
    TEE_ALG_SM4_ECB_NOPAD                = 0x10000014,
    TEE_ALG_SM4_ECB_PKCS7                = 0x10000024,
    TEE_ALG_SM4_CBC_NOPAD                = 0x10000114,
    TEE_ALG_SM4_CBC_PKCS7                = 0xF0000003,
    TEE_ALG_SM4_CTR                      = 0x10000214,
    TEE_ALG_SM4_CFB128                   = 0xF0000000,
    TEE_ALG_SM4_XTS                      = 0x10000414,
    TEE_ALG_SM4_OFB                      = 0x10000514,
    TEE_ALG_AES_OFB                      = 0x10000510,
    TEE_ALG_SM4_GCM                      = 0xF0000005,
    TEE_ALG_PBKDF2_HMAC_SHA1_DERIVE_KEY  = 0x800020C2,
    TEE_ALG_PBKDF2_HMAC_SHA256_DERIVE_KEY = 0x800040C2,
    TEE_ALG_PBKDF2_HMAC_SHA384_DERIVE_KEY = 0x800050C2,
    TEE_ALG_PBKDF2_HMAC_SHA512_DERIVE_KEY = 0x800060C2,
    TEE_ALG_HKDF                         = 0x80000047,
    TEE_ALG_PRF                          = 0xF0000006,
};

typedef enum __tee_crypto_algorithm_id tee_crypto_algorithm_id;

#define TEE_OPTIONAL_ELEMENT_NONE 0x00000000

typedef enum {
    TEE_ECC_CURVE_NIST_P192 = 0x00000001,
    TEE_ECC_CURVE_NIST_P224 = 0x00000002,
    TEE_ECC_CURVE_NIST_P256 = 0x00000003,
    TEE_ECC_CURVE_NIST_P384 = 0x00000004,
    TEE_ECC_CURVE_NIST_P521 = 0x00000005,
    TEE_ECC_CURVE_SM2       = 0x00000300, /* CURVE_SM2 256 bits */
    TEE_ECC_CURVE_25519     = 0x00000200, /* CURVE_25519 256 bits */
} TEE_ECC_CURVE;

typedef enum {
    TEE_DH_HASH_SHA1_mode   = 0,
    TEE_DH_HASH_SHA224_mode = 1,
    TEE_DH_HASH_SHA256_mode = 2,
    TEE_DH_HASH_SHA384_mode = 3,
    TEE_DH_HASH_SHA512_mode = 4,
    TEE_DH_HASH_NumOfModes,
} TEE_DH_HASH_Mode;

typedef enum {
    TEE_DH_PKCS3_mode     = 0, /* PKCS3 */
    TEE_DH_ANSI_X942_mode = 1, /* X942 */
    TEE_DH_NumOfModes,         /* num of modes */
} TEE_DH_OpMode_t;

typedef enum {
    TEE_DH_ASN1_DerivMode          = 0,                      /* ASN1_DerivMode */
    TEE_DH_ConcatDerivMode         = 1,                      /* ConcatDerivMode */
    TEE_DH_X963_DerivMode          = TEE_DH_ConcatDerivMode, /* X963_DerivMode */
    TEE_DH_OMADRM_DerivMode        = 2,                      /* OMADRM_DerivMode */
    TEE_DH_ISO18033_KDF1_DerivMode = 3,                      /* ISO18033_KDF1_DerivMode */
    TEE_DH_ISO18033_KDF2_DerivMode = 4,                      /* ISO18033_KDF2_DerivMode */
    TEE_DH_DerivFunc_NumOfModes,                             /* num of modes */
} TEE_DH_DerivFuncMode;

enum __TEE_DK_ObjectAttribute {
    TEE_DK_SECRECT = 0,    /* A pointer to shared secret value */
    TEE_DK_OTHER,          /* A pointer to structure containing other data */
    TEE_DK_HASH_MODE,      /* The enumerator ID of the HASH function to be used */
    TEE_DK_DERIVATION_MODE /* The enumerator ID of the derivation function mode */
};

typedef enum __TEE_DK_ObjectAttribute tee_dk_objectattribute;

enum __TEE_OperationMode {
    TEE_MODE_ENCRYPT = 0x0,
    TEE_MODE_DECRYPT,
    TEE_MODE_SIGN,
    TEE_MODE_VERIFY,
    TEE_MODE_MAC,
    TEE_MODE_DIGEST,
    TEE_MODE_DERIVE
};

enum tee_operation_state {
    TEE_OPERATION_STATE_INITIAL = 0x00000000,
    TEE_OPERATION_STATE_ACTIVE  = 0x00000001,
};

typedef uint32_t TEE_OperationMode;

typedef struct {
    uint8_t algorithm_id[TEE_DH_MAX_SIZE_OF_OTHER_INFO];   /* object ID(OID) */
    uint32_t size_of_algorithm_id;                         /* length of AlgorithmID */
    uint8_t party_u_info[TEE_DH_MAX_SIZE_OF_OTHER_INFO];   /* public info of sender */
    uint32_t size_of_party_u_info;                         /* length of PartyUInfo */
    uint8_t party_v_info[TEE_DH_MAX_SIZE_OF_OTHER_INFO];   /* public info of receiver */
    uint32_t size_of_party_v_info;                         /* length of PartyVInfo */
    uint8_t supp_priv_info[TEE_DH_MAX_SIZE_OF_OTHER_INFO]; /* shared private info */
    uint32_t size_of_supp_priv_info;                       /* length of SuppPrivInfo */
    uint8_t supp_pub_info[TEE_DH_MAX_SIZE_OF_OTHER_INFO];  /* shared public info */
    uint32_t size_of_supp_pub_info;                        /* length of SuppPubInfo */
} TEE_DH_OtherInfo;

struct __TEE_OperationInfo {
    uint32_t algorithm;        /* #__TEE_CRYPTO_ALGORITHM_ID */
    uint32_t operationClass;   /* #__TEE_Operation_Constants */
    uint32_t mode;             /* #__TEE_OperationMode */
    uint32_t digestLength;
    uint32_t maxKeySize;
    uint32_t keySize;
    uint32_t requiredKeyUsage;
    uint32_t handleState;
    void *keyValue;
};

typedef struct __TEE_OperationInfo TEE_OperationInfo;

typedef struct {
    uint32_t keySize;
    uint32_t requiredKeyUsage;
} TEE_OperationInfoKey;

typedef struct {
    uint32_t algorithm;
    uint32_t operationClass;
    uint32_t mode;
    uint32_t digestLength;
    uint32_t maxKeySize;
    uint32_t handleState;
    uint32_t operationState;
    uint32_t numberOfKeys;
    TEE_OperationInfoKey keyInformation[];
} TEE_OperationInfoMultiple;

struct __TEE_OperationHandle {
    uint32_t algorithm;        /* #__TEE_CRYPTO_ALGORITHM_ID */
    uint32_t operationClass;   /* #__TEE_Operation_Constants */
    uint32_t mode;             /* #__TEE_OperationMode */
    uint32_t digestLength;
    uint32_t maxKeySize;
    uint32_t keySize;
    uint32_t keySize2;
    uint32_t requiredKeyUsage;
    uint32_t handleState;
    void *keyValue;
    void *keyValue2;
    void *crypto_ctxt;
    void *hmac_rest_ctext;
    void *IV;
    void *publicKey;
    uint32_t publicKeyLen;
    void *privateKey;
    uint32_t privateKeyLen;
    uint32_t IVLen;
    /* start of DH */
    TEE_DH_OtherInfo *dh_otherinfo; /* #TEE_DH_OtherInfo */
    uint32_t dh_hash_mode;          /* #TEE_DH_HASH_Mode */
    uint32_t dh_derive_func;        /* #TEE_DH_DerivFuncMode */
    uint32_t dh_op_mode;            /* #TEE_DH_OpMode_t */
    void *dh_prime;
    uint32_t dh_prime_size;
    /* end of DH */
    pthread_mutex_t operation_lock;
    void *hal_info;
};
typedef struct {
    uint32_t src;
    uint32_t dest;
} crypto_uint2uint;

#define RSA_PUBKEY_MAXSIZE sizeof(CRYS_RSAUserPubKey_t)

#define RSA_PRIVKEY_MAXSIZE sizeof(CRYS_RSAUserPrivKey_t)

typedef struct {
    void *src_data;
    size_t src_len;
    void *dest_data;
    size_t *dest_len;
} operation_src_dest;

typedef struct {
    void *nonce;
    size_t nonce_len;
    uint32_t tag_len;
    size_t aad_len;
    size_t payload_len;
} operation_ae_init;

typedef struct __TEE_OperationHandle *TEE_OperationHandle;

typedef struct __TEE_OperationHandle TEE_OperationHandleVar;

typedef struct __TEE_ObjectHandle TEE_ObjectHandleVar;

/*
 * alloc operation handle
 *
 * @param operation [IN/OUT]  #TEE_OperationHandle
 * @param algorithm [IN]  #TEE_CRYPTO_ALGORITHM_ID
 * @param mode [IN]  #TEE_OperationMode
 * @param max_key_size [IN]  The max key size
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_OUT_OF_MEMORY #TEE_OperationHandle malloc failed
 * @return TEE_ERROR_NOT_SUPPORTE #TEE_CRYPTO_ALGORITHM_ID not support or max key size is out of range
 * @return TEE_ERROR_GENERIC other failed
 */
TEE_Result TEE_AllocateOperation(TEE_OperationHandle *operation, uint32_t algorithm, uint32_t mode,
                                 uint32_t max_key_size);
/*
 * free Operation handle
 *
 * @param operation [IN/OUT] operation handle
 *
 * @return void
 */
void TEE_FreeOperation(TEE_OperationHandle operation);

/*
 * get Operation Info
 *
 * @param operation [IN]  #TEE_OperationHandle
 * @param operationInfo [IN/OUT]  #TEE_OperationInfo
 *
 * @return void
 */
void TEE_GetOperationInfo(const TEE_OperationHandle operation, TEE_OperationInfo *operationInfo);

/*
 * reset operation handle
 *
 * @param operation [IN/OUT]  #TEE_OperationHandle
 *
 * @return void
 */
void TEE_ResetOperation(TEE_OperationHandle operation);

/*
 * set operation key
 *
 * @param operation [IN/OUT]  #TEE_OperationHandle
 * @param key [IN]  #TEE_ObjectHandle
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_BAD_PARAMETERS the params is invalid
 * @return TEE_ERROR_OUT_OF_MEMORY  malloc key buffer failed
 */
TEE_Result TEE_SetOperationKey(TEE_OperationHandle operation, const TEE_ObjectHandle key);

/*
 * set operation key1 and key2
 *
 * @param operation [IN/OUT]  #TEE_OperationHandle
 * @param key1 [IN]  #TEE_ObjectHandle
 * @param key2 [IN]  #TEE_ObjectHandle
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_BAD_PARAMETERS the params is invalid
 */
TEE_Result TEE_SetOperationKey2(TEE_OperationHandle operation, const TEE_ObjectHandle key1,
                                const TEE_ObjectHandle key2);
/*
 * copy src operation to dest operation
 *
 * @param dst_operation [IN/OUT]  #TEE_OperationHandle
 * @param src_operation [IN]  #TEE_OperationHandle
 *
 * @return void
 */
void TEE_CopyOperation(TEE_OperationHandle dst_operation, const TEE_OperationHandle src_operation);

/*
 * init cipher context
 *
 * @param operation [IN/OUT]  #TEE_OperationHandle
 * @param IV [IN]  the iv buffer, set NULL if not use
 * @param IVLen [IN]  the length of iv buffer
 *
 * @return void
 */
void TEE_CipherInit(TEE_OperationHandle operation, const void *IV, size_t IVLen);

/*
 * do cipher update
 *
 * @param operation [IN/OUT]  #TEE_OperationHandle
 * @param srcData [IN]  the src data
 * @param srcLen [IN]  the length of src data
 * @param destData [OUT] the dest data
 * @param destLen [OUT] the length of dest data
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_BAD_PARAMETERS the params is invalid
 * @return TEE_ERROR_GENERIC other error
 */
TEE_Result TEE_CipherUpdate(TEE_OperationHandle operation, const void *srcData, size_t srcLen, void *destData,
                            size_t *destLen);
/*
 * do cipher finish
 *
 * @param operation [IN/OUT]  #TEE_OperationHandle
 * @param srcData [IN]  the src data
 * @param srcLen [IN]  the length of src data
 * @param destData [OUT] the dest data
 * @param destLen [OUT] the length of dest data
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_BAD_PARAMETERS the params is invalid
 * @return TEE_ERROR_GENERIC other error
 */
TEE_Result TEE_CipherDoFinal(TEE_OperationHandle operation, const void *srcData, size_t srcLen, void *destData,
                             size_t *destLen);
/*
 * do digest update
 *
 * @param operation [IN/OUT] #TEE_OperationHandle
 * @param chunk [IN] the chunk buffer
 * @param chunkSize [IN] the length of chunk buffer
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_GENERIC other error
 */
#ifndef GP_SUPPORT

#if defined(API_LEVEL) && (API_LEVEL >= 2)
void TEE_DigestUpdate(TEE_OperationHandle operation, const void *chunk, size_t chunkSize);
#else
TEE_Result TEE_DigestUpdate(TEE_OperationHandle operation, const void *chunk, size_t chunkSize);
#endif /* API_LEVEL */

#else
void TEE_DigestUpdate(TEE_OperationHandle operation, const void *chunk, size_t chunkSize);
#endif

/*
 * do digest finish
 *
 * @param operation [IN/OUT] #TEE_OperationHandle
 * @param chunk [IN] the chunk buffer
 * @param chunkSize [IN] the length of chunk buffer
 * @param hash [OUT]  the hash buffer
 * @param hashLen [OUT]  the length of hash buffer
 *
 * @return #TEE_SUCCESS success
 * @return #TEE_ERROR_GENERIC other error
 */
TEE_Result TEE_DigestDoFinal(TEE_OperationHandle operation, const void *chunk, size_t chunkLen, void *hash,
                             size_t *hashLen);

/*
 * do mac init
 *
 * @param operation [IN/OUT] #TEE_OperationHandle
 * @param IV [IN]  the iv buffer, set NULL if not use
 * @param IVLen [IN]  the length of iv buffer
 *
 * @return void
 */
void TEE_MACInit(TEE_OperationHandle operation, void *IV, size_t IVLen);

/*
 * do mac update
 *
 * @param operation [IN/OUT]   #TEE_OperationHandle
 * @param chunk [IN] the chunk buffer
 * @param chunkSize [IN] the length of chunk buffer
 *
 * @return void
 */
void TEE_MACUpdate(TEE_OperationHandle operation, const void *chunk, size_t chunkSize);

/*
 * do mac finish
 *
 * @param operation [IN/OUT]   #TEE_OperationHandle
 * @param message [IN] the message buffer
 * @param messageLen [IN] the length of message buffer
 * @param mac [OUT]  the mac buffer
 * @param macLen [OUT]  the length of mac buffer
 *
 * @return #TEE_SUCCESS success
 * @return #TEE_ERROR_GENERIC other error
 */
TEE_Result TEE_MACComputeFinal(TEE_OperationHandle operation, const void *message, size_t messageLen, void *mac,
                               size_t *macLen);
/*
 * do mac finish and compare
 *
 * @param operation [IN/OUT]  #TEE_OperationHandle
 * @param message [IN] the message buffer
 * @param messageLen [IN] the length of message buffer
 * @param mac [OUT]  the mac buffer
 * @param macLen [OUT]  the length of mac buffer
 *
 * @return #TEE_SUCCESS success
 * @return #TEE_ERROR_GENERIC other error
 * @return #TEE_ERROR_MAC_INVALID compare failed
 */
#ifndef GP_SUPPORT

#if defined(API_LEVEL) && (API_LEVEL >= 2)
TEE_Result TEE_MACCompareFinal(TEE_OperationHandle operation, const void *message, size_t messageLen, const void *mac,
                               const size_t macLen);
#else
TEE_Result TEE_MACCompareFinal(TEE_OperationHandle operation, const void *message, size_t messageLen, const void *mac,
                               const size_t *macLen);
#endif /* API_LEVEL */

#else
TEE_Result TEE_MACCompareFinal(TEE_OperationHandle operation, const void *message, size_t messageLen, const void *mac,
                               const size_t macLen);
#endif

/*
 * do derive key
 *
 * @param operation [IN/OUT] #TEE_OperationHandle
 * @param params [IN] #TEE_Attribute
 * @param paramCount [IN] the count of param
 * @param derivedKey [OUT] #TEE_ObjectHandle
 *
 * @return void
 */
void TEE_DeriveKey(TEE_OperationHandle operation, const TEE_Attribute *params, uint32_t paramCount,
                   TEE_ObjectHandle derivedKey);
/*
 * generate random data
 *
 * @param randomBuffer [IN/OUT]  the random buffer
 * @param randomBufferLen [IN]  the length of random buffer
 *
 * @return void
 */
void TEE_GenerateRandom(void *randomBuffer, size_t randomBufferLen);

/*
 * do ae init
 *
 * @param operation [IN/OUT]   #TEE_OperationHandle
 * @param nonce [IN] the nounce buffer
 * @param nonceLen [IN] the length of nounce
 * @param tagLen [IN] the length of tag
 * @param AADLen [IN] the length of aad
 * @param payloadLen [IN] the length of payload
 *
 * @return #TEE_SUCCESS success
 * @return #TEE_ERROR_GENERIC other error
 */
TEE_Result TEE_AEInit(TEE_OperationHandle operation, void *nonce, size_t nonceLen, uint32_t tagLen, size_t AADLen,
                      size_t payloadLen);
/*
 * update ae aad
 *
 * @param operation [IN/OUT] #TEE_OperationHandle
 * @param AADdata [IN] the aad buffer
 * @param AADdataLen [IN] the length of aad buffer
 *
 * @return void
 */
void TEE_AEUpdateAAD(TEE_OperationHandle operation, const void *AADdata, size_t AADdataLen);

/*
 * do ae update
 *
 * @param operation [IN/OUT]   #TEE_OperationHandle
 * @param srcData [IN]  the src data
 * @param srcLen [IN]  the length of src data
 * @param destData [OUT] the dest data
 * @param destLen [OUT] the length of dest data
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_GENERIC other error
 */
TEE_Result TEE_AEUpdate(TEE_OperationHandle operation, void *srcData, size_t srcLen, void *destData, size_t *destLen);

/*
 * do ae encrypt
 *
 * @param operation [IN/OUT]   #TEE_OperationHandle
 * @param srcData [IN]  the src data
 * @param srcLen [IN]  the length of src data
 * @param destData [OUT] the dest data
 * @param destLen [OUT] the length of dest data
 * @param tag [OUT] the tag buffer
 * @param tagLen [OUT] the length of tag buffer
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_GENERIC other error
 */
TEE_Result TEE_AEEncryptFinal(TEE_OperationHandle operation, void *srcData, size_t srcLen, void *destData,
                              size_t *destLen, void *tag, size_t *tagLen);
/*
 * do ae decrypt
 *
 * @param operation [IN/OUT]   #TEE_OperationHandle
 * @param srcData [IN]  the src data
 * @param srcLen [IN]  the length of src data
 * @param destData [OUT] the dest data
 * @param destLen [OUT] the length of dest data
 * @param tag [IN] the tag buffer
 * @param tagLen [IN] the length of tag buffer
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_MAC_INVALID the tag is invalid
 */
TEE_Result TEE_AEDecryptFinal(TEE_OperationHandle operation, void *srcData, size_t srcLen, void *destData,
                              size_t *destLen, void *tag, size_t tagLen);

/*
 * do asymmetric encrypt
 *
 * @param operation [IN/OUT]   #TEE_OperationHandle
 * @param params [IN]  #TEE_Attribute
 * @param paramCount [IN] the count of params
 * @param srcData [IN]  the src data
 * @param srcLen [IN]  the length of src data
 * @param destData [OUT] the dest data
 * @param destLen [OUT] the length of dest data
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_BAD_PARAMETERS the params is invalid
 * @return TEE_ERROR_GENERIC other error
 */
TEE_Result TEE_AsymmetricEncrypt(TEE_OperationHandle operation, const TEE_Attribute *params, uint32_t paramCount,
                                 void *srcData, size_t srcLen, void *destData, size_t *destLen);

/*
 * do asymmetric decrypt
 *
 * @param operation [IN/OUT]   #TEE_OperationHandle
 * @param params [IN]  #TEE_Attribute
 * @param paramCount [IN] the count of params
 * @param srcData [IN]  the src data
 * @param srcLen [IN]  the length of src data
 * @param destData [OUT] the dest data
 * @param destLen [OUT] the length of dest data
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_BAD_PARAMETERS the params is invalid
 * @return TEE_ERROR_GENERIC other error
 *
 */
TEE_Result TEE_AsymmetricDecrypt(TEE_OperationHandle operation, const TEE_Attribute *params, uint32_t paramCount,
                                 void *srcData, size_t srcLen, void *destData, size_t *destLen);

/*
 * do asymmetric sign
 *
 * @param operation [IN/OUT] #TEE_OperationHandle
 * @param params [IN]  #TEE_Attribute
 * @param paramCount [IN] the count of params
 * @param digest [IN]  the digest data
 * @param digestLen [IN]  the length of digest data
 * @param signature [OUT] the signature data
 * @param signatureLen [OUT] the length of signature data
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_BAD_PARAMETERS the params is invalid
 * @return TEE_ERROR_GENERIC other error
 */
TEE_Result TEE_AsymmetricSignDigest(TEE_OperationHandle operation, const TEE_Attribute *params, uint32_t paramCount,
                                    void *digest, size_t digestLen, void *signature, size_t *signatureLen);

/*
 * do asymmetric verify
 *
 * @param operation [IN/OUT] #TEE_OperationHandle
 * @param params [IN]  #TEE_Attribute
 * @param paramCount [IN] the count of params
 * @param digest [IN]  the digest data
 * @param digestLen [IN]  the length of digest data
 * @param signature [IN] the signature data
 * @param signatureLen [IN] the length of signature data
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_BAD_PARAMETERS the params is invalid
 * @return TEE_ERROR_GENERIC other error
 */
TEE_Result TEE_AsymmetricVerifyDigest(TEE_OperationHandle operation, const TEE_Attribute *params, uint32_t paramCount,
                                      void *digest, size_t digestLen, void *signature, size_t signatureLen);

#if defined(API_LEVEL) && (API_LEVEL >= 2)

/*
 * Get Operation Info multiple
 *
 * @param operation [IN/OUT] #TEE_OperationHandle
 * @param operationInfoMultiple [IN/OUT] #TEE_OperationInfoMultiple
 * @param operationSize [IN] the size of operation handle
 *
 * @return TEE_SUCCESS succss
 * @return TEE_ERROR_BAD_PARAMETERS the params is invalid
 * @return TEE_ERROR_SHORT_BUFFER the buffer is not large enough
 */
TEE_Result TEE_GetOperationInfoMultiple(TEE_OperationHandle operation, TEE_OperationInfoMultiple *operationInfoMultiple,
                                        const size_t *operationSize);
#endif /* API_LEVEL */

#if defined(API_LEVEL) && (API_LEVEL >= 2)

/*
 * check whether the algorithm is sopported
 *
 * @param algId [IN]  the algorithm
 * @param element [IN] the element
 *
 * @return TEE_SUCCESS support
 * @return TEE_ERROR_NOT_SUPPORTED not support
 */
TEE_Result TEE_IsAlgorithmSupported(uint32_t algId, uint32_t element);

TEE_Result TEE_IsHardWareSupportAlgorithm(uint32_t alg_type);
#endif /* API_LEVEL */

#endif
