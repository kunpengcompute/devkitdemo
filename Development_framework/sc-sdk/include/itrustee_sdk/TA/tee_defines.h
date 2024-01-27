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
 * Description: Reference of TEE internal api and internal definitions
 */

#ifndef __TEE_DEFINES_H
#define __TEE_DEFINES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <tee_uuid.h>

#ifndef TA_EXPORT
#define TA_EXPORT
#endif

typedef int *tee_mutex_handle;

#define API_LEVEL1_0   1
#define API_LEVEL1_1_1 2
#define API_LEVEL1_2   3

#define TEE_PARAMS_NUM 4

#define PARAM_NOT_USED(val) ((void)(val))

typedef union {
    struct {
        void *buffer;
        size_t size;
    } memref;
    struct {
        unsigned int a;
        unsigned int b;
    } value;
} TEE_Param;

#define TEE_PARAM_TYPES(param0Type, param1Type, param2Type, param3Type) \
    (((param3Type) << 12) | ((param2Type) << 8) | ((param1Type) << 4) | (param0Type))

#define TEE_PARAM_TYPE_GET(paramTypes, index) (((paramTypes) >> (4U * (index))) & 0x0F)

/*
 * check validation of parameter types
 *
 * @param param_to_check [IN] expected parameter values
 * @param valid0 [IN] first parameter type
 * @param valid1 [IN] second parameter type
 * @param valid2 [IN] third parameter type
 * @param valid3 [IN] fourth parameter type
 *
 * @retval true parameter types are correct
 * @retval false parameter types are incorrect
 */
static inline bool check_param_type(uint32_t param_to_check, uint32_t valid0, uint32_t valid1, uint32_t valid2,
                                    uint32_t valid3)
{
    return (TEE_PARAM_TYPES(valid0, valid1, valid2, valid3) == param_to_check);
}

enum TEE_ParamType {
    TEE_PARAM_TYPE_NONE             = 0x0,
    TEE_PARAM_TYPE_VALUE_INPUT      = 0x1,
    TEE_PARAM_TYPE_VALUE_OUTPUT     = 0x2,
    TEE_PARAM_TYPE_VALUE_INOUT      = 0x3,
    TEE_PARAM_TYPE_MEMREF_INPUT     = 0x5,
    TEE_PARAM_TYPE_MEMREF_OUTPUT    = 0x6,
    TEE_PARAM_TYPE_MEMREF_INOUT     = 0x7,
    TEE_PARAM_TYPE_ION_INPUT        = 0x8,
    TEE_PARAM_TYPE_ION_SGLIST_INPUT = 0x9,
    TEE_PARAM_TYPE_MEMREF_SHARED_INOUT = 0xa,
    TEE_PARAM_TYPE_RESMEM_INPUT        = 0xc,
    TEE_PARAM_TYPE_RESMEM_OUTPUT       = 0xd,
    TEE_PARAM_TYPE_RESMEM_INOUT        = 0xe,
};

#define S_VAR_NOT_USED(variable) \
    do {                         \
        (void)(variable);        \
    } while (0);

typedef struct {
    uint32_t objectType;
    uint32_t objectSize;
    uint32_t maxObjectSize;
    uint32_t objectUsage;
    uint32_t dataSize;
    uint32_t dataPosition;
    uint32_t handleFlags;
} TEE_ObjectInfo;

typedef struct {
    uint32_t attributeID;
    union {
        struct {
            void *buffer;
            size_t length;
        } ref;
        struct {
            uint32_t a;
            uint32_t b;
        } value;
    } content;
} TEE_Attribute;

enum TEE_ObjectAttribute {
    TEE_ATTR_SECRET_VALUE          = 0xC0000000,
    TEE_ATTR_RSA_MODULUS           = 0xD0000130,
    TEE_ATTR_RSA_PUBLIC_EXPONENT   = 0xD0000230,
    TEE_ATTR_RSA_PRIVATE_EXPONENT  = 0xC0000330,
    TEE_ATTR_RSA_PRIME1            = 0xC0000430,
    TEE_ATTR_RSA_PRIME2            = 0xC0000530,
    TEE_ATTR_RSA_EXPONENT1         = 0xC0000630,
    TEE_ATTR_RSA_EXPONENT2         = 0xC0000730,
    TEE_ATTR_RSA_COEFFICIENT       = 0xC0000830,
    TEE_ATTR_RSA_MGF1_HASH         = 0xF0000830,
    TEE_ATTR_DSA_PRIME             = 0xD0001031,
    TEE_ATTR_DSA_SUBPRIME          = 0xD0001131,
    TEE_ATTR_DSA_BASE              = 0xD0001231,
    TEE_ATTR_DSA_PUBLIC_VALUE      = 0xD0000131,
    TEE_ATTR_DSA_PRIVATE_VALUE     = 0xC0000231,
    TEE_ATTR_DH_PRIME              = 0xD0001032,
    TEE_ATTR_DH_SUBPRIME           = 0xD0001132,
    TEE_ATTR_DH_BASE               = 0xD0001232,
    TEE_ATTR_DH_X_BITS             = 0xF0001332,
    TEE_ATTR_DH_PUBLIC_VALUE       = 0xD0000132,
    TEE_ATTR_DH_PRIVATE_VALUE      = 0xC0000232,
    TEE_ATTR_RSA_OAEP_LABEL        = 0xD0000930,
    TEE_ATTR_RSA_PSS_SALT_LENGTH   = 0xF0000A30,
    TEE_ATTR_ECC_PUBLIC_VALUE_X    = 0xD0000141,
    TEE_ATTR_ECC_PUBLIC_VALUE_Y    = 0xD0000241,
    TEE_ATTR_ECC_PRIVATE_VALUE     = 0xC0000341,
    TEE_ATTR_ECC_CURVE             = 0xF0000441,
    TEE_ATTR_ED25519_CTX           = 0xD0000643,
    TEE_ATTR_ED25519_PUBLIC_VALUE  = 0xD0000743,
    TEE_ATTR_ED25519_PRIVATE_VALUE = 0xC0000843,
    TEE_ATTR_ED25519_PH            = 0xF0000543,
    TEE_ATTR_X25519_PUBLIC_VALUE   = 0xD0000944,
    TEE_ATTR_X25519_PRIVATE_VALUE  = 0xC0000A44,
    TEE_ATTR_PBKDF2_HMAC_PASSWORD  = 0xD0000133,
    TEE_ATTR_PBKDF2_HMAC_SALT      = 0xD0000134,
    TEE_ATTR_PBKDF2_HMAC_DIGEST    = 0xF0000135,
    TEE_ATTR_PRF_LABEL             = 0xD0000136,
    TEE_ATTR_PRF_SEED              = 0xD0000137,
    TEE_ATTR_PRF_HASH_ALGORITHM    = 0xF0000138,
    TEE_ATTR_HKDF_SALT             = 0xD0000946,
    TEE_ATTR_HKDF_INFO             = 0xD0000A46,
    TEE_ATTR_HKDF_HASH_ALGORITHM   = 0xF0000B46,
    TEE_ATTR_KDF_KEY_SIZE          = 0xF0000C46,
};

enum TEE_ObjectType {
    TEE_TYPE_AES                = 0xA0000010,
    TEE_TYPE_DES                = 0xA0000011,
    TEE_TYPE_DES3               = 0xA0000013,
    TEE_TYPE_HMAC_MD5           = 0xA0000001,
    TEE_TYPE_HMAC_SHA1          = 0xA0000002,
    TEE_TYPE_HMAC_SHA224        = 0xA0000003,
    TEE_TYPE_HMAC_SHA256        = 0xA0000004,
    TEE_TYPE_HMAC_SHA384        = 0xA0000005,
    TEE_TYPE_HMAC_SHA512        = 0xA0000006,
    TEE_TYPE_RSA_PUBLIC_KEY     = 0xA0000030,
    TEE_TYPE_RSA_KEYPAIR        = 0xA1000030,
    TEE_TYPE_DSA_PUBLIC_KEY     = 0xA0000031,
    TEE_TYPE_DSA_KEYPAIR        = 0xA1000031,
    TEE_TYPE_DH_KEYPAIR         = 0xA1000032,
    TEE_TYPE_GENERIC_SECRET     = 0xA0000000,
    TEE_TYPE_DATA               = 0xA1000033,
    TEE_TYPE_DATA_GP1_1         = 0xA00000BF,
    TEE_TYPE_ECDSA_PUBLIC_KEY   = 0xA0000041,
    TEE_TYPE_ECDSA_KEYPAIR      = 0xA1000041,
    TEE_TYPE_ECDH_PUBLIC_KEY    = 0xA0000042,
    TEE_TYPE_ECDH_KEYPAIR       = 0xA1000042,
    TEE_TYPE_ED25519_PUBLIC_KEY = 0xA0000043,
    TEE_TYPE_ED25519_KEYPAIR    = 0xA1000043,
    TEE_TYPE_X25519_PUBLIC_KEY  = 0xA0000044,
    TEE_TYPE_X25519_KEYPAIR     = 0xA1000044,
    TEE_TYPE_SM2_DSA_PUBLIC_KEY = 0xA0000045,
    TEE_TYPE_SM2_DSA_KEYPAIR    = 0xA1000045,
    TEE_TYPE_SM2_KEP_PUBLIC_KEY = 0xA0000046,
    TEE_TYPE_SM2_KEP_KEYPAIR    = 0xA1000046,
    TEE_TYPE_SM2_PKE_PUBLIC_KEY = 0xA0000047,
    TEE_TYPE_SM2_PKE_KEYPAIR    = 0xA1000047,
    TEE_TYPE_HMAC_SM3           = 0xA0000007,
    TEE_TYPE_SM4                = 0xA0000014,
    TEE_TYPE_SIP_HASH           = 0xF0000002,
    TEE_TYPE_PBKDF2_HMAC        = 0xF0000004,
    TEE_TYPE_HKDF               = 0xA000004A,
    TEE_TYPE_PRF                = 0xF0000005,

    TEE_TYPE_CORRUPTED_OBJECT = 0xA00000BE,
};

#define OBJECT_NAME_LEN_MAX 255

struct __TEE_ObjectHandle {
    void *dataPtr;
    uint32_t dataLen;
    uint8_t dataName[OBJECT_NAME_LEN_MAX];
    TEE_ObjectInfo *ObjectInfo;
    TEE_Attribute *Attribute;
    uint32_t attributesLen;
    uint32_t CRTMode;
    void *infoattrfd;
    uint32_t generate_flag;
    uint32_t storage_id;
};
typedef struct __TEE_ObjectHandle *TEE_ObjectHandle;

enum TEE_Result_Value {
    TEE_SUCCESS                             = 0x00000000, /* success                                                */
    TEE_ERROR_INVALID_CMD                   = 0x00000001, /* command is invalid                                     */
    TEE_ERROR_SERVICE_NOT_EXIST             = 0x00000002, /* service is not exist                                   */
    TEE_ERROR_SESSION_NOT_EXIST             = 0x00000003, /* session is not exist                                   */
    TEE_ERROR_SESSION_MAXIMUM               = 0x00000004, /* exceeds max session count                              */
    TEE_ERROR_REGISTER_EXIST_SERVICE        = 0x00000005, /* service already registered                             */
    TEE_ERROR_TARGET_DEAD_FATAL             = 0x00000006, /* internal error occurs                                  */
    TEE_ERROR_READ_DATA                     = 0x00000007, /* read data failed                                       */
    TEE_ERROR_WRITE_DATA                    = 0x00000008, /* write data failed                                      */
    TEE_ERROR_TRUNCATE_OBJECT               = 0x00000009, /* truncate data failed                                   */
    TEE_ERROR_SEEK_DATA                     = 0x0000000A, /* seek data failed                                       */
    TEE_ERROR_SYNC_DATA                     = 0x0000000B, /* sync data failed                                       */
    TEE_ERROR_RENAME_OBJECT                 = 0x0000000C, /* rename file failed                                     */
    TEE_ERROR_TRUSTED_APP_LOAD_ERROR        = 0x0000000D, /* error occurs when loading TA                           */
    TEE_ERROR_OTRP_LOAD_NOT_MATCHED         = 0x80000100, /* TA type is inconsistent with the loading mode.         */
    TEE_ERROR_OTRP_LOAD_EXCEED              = 0x80000101, /* the not open session's otrp service num exceeds        */
    TEE_ERROR_OTRP_ACCESS_DENIED            = 0x80000102, /* uuid of load cmd is not inconsistent with the sec file */
    TEE_ERROR_OTRP_SERVICE_AGED             = 0x80000103, /* otrp service is aged                                   */
    TEE_ERROR_STORAGE_EIO                   = 0x80001001, /* I/O error occurs in storage operation                  */
    TEE_ERROR_STORAGE_EAGAIN                = 0x80001002, /* storage section is unavailable                         */
    TEE_ERROR_STORAGE_ENOTDIR               = 0x80001003, /* operation target is not directory                      */
    TEE_ERROR_STORAGE_EISDIR                = 0x80001004, /* cannot do this operation on directory                  */
    TEE_ERROR_STORAGE_ENFILE                = 0x80001005, /* opened files exceed max count in system                */
    TEE_ERROR_STORAGE_EMFILE                = 0x80001006, /* opened files exceed max count for this process         */
    TEE_ERROR_STORAGE_EROFS                 = 0x80001007, /* stroage section is read only                           */
    TEE_ERROR_STORAGE_PATH_WRONG            = 0x8000100A, /* File path error                                        */
    TEE_ERROR_MSG_QUEUE_OVERFLOW            = 0x8000100B, /* sevice msg queue overflow                              */
    TEE_ERROR_SUBTHREAD_ACCESS              = 0x8000100C, /* The subthread created by TA cannot access the service  */
    TEE_ERROR_ORIGIN_PARTITION_INACTIVE     = 0x8000100D, /* Enable backup feature, original partition is inactive  */
    TEE_ERROR_BACKUP_PARTITION_INACTIVE     = 0x8000100E, /* Enable backup feature, backup partition is inactive    */
    TEE_ERROR_CORRUPT_OBJECT                = 0xF0100001, /* file object has been damaged                           */
    TEE_ERROR_STORAGE_NOT_AVAILABLE         = 0xF0100003, /* storage section is unavailable                         */
    TEE_ERROR_CIPHERTEXT_INVALID            = 0xF0100006, /* cipher text is incorrect                               */
    TEE_ISOCKET_ERROR_PROTOCOL              = 0xF1007001, /* protocol error in socket connection                    */
    TEE_ISOCKET_ERROR_REMOTE_CLOSED         = 0xF1007002, /* socket is closed by remote                             */
    TEE_ISOCKET_ERROR_TIMEOUT               = 0xF1007003, /* socket connection is timeout                           */
    TEE_ISOCKET_ERROR_OUT_OF_RESOURCES      = 0xF1007004, /* no resource avaliable for socket connection            */
    TEE_ISOCKET_ERROR_LARGE_BUFFER          = 0xF1007005, /* buffer is too large in socket connection               */
    TEE_ISOCKET_WARNING_PROTOCOL            = 0xF1007006, /* warnning occurs in socket connection                   */
    TEE_ERROR_GENERIC                       = 0xFFFF0000, /* generic error                                          */
    TEE_ERROR_ACCESS_DENIED                 = 0xFFFF0001, /* access is denied                                       */
    TEE_ERROR_CANCEL                        = 0xFFFF0002, /* operation has been canceled                            */
    TEE_ERROR_ACCESS_CONFLICT               = 0xFFFF0003, /* conflict access error occurs                           */
    TEE_ERROR_EXCESS_DATA                   = 0xFFFF0004, /* exceeds max data size                                  */
    TEE_ERROR_BAD_FORMAT                    = 0xFFFF0005, /* incorrect data format                                  */
    TEE_ERROR_BAD_PARAMETERS                = 0xFFFF0006, /* incorrect parameters                                   */
    TEE_ERROR_BAD_STATE                     = 0xFFFF0007, /* operation is not allowed in current state              */
    TEE_ERROR_ITEM_NOT_FOUND                = 0xFFFF0008, /* cannot find target item                                */
    TEE_ERROR_NOT_IMPLEMENTED               = 0xFFFF0009, /* api is not implemented                                 */
    TEE_ERROR_NOT_SUPPORTED                 = 0xFFFF000A, /* api is not supported                                   */
    TEE_ERROR_NO_DATA                       = 0xFFFF000B, /* no data avaliable for this operation                   */
    TEE_ERROR_OUT_OF_MEMORY                 = 0xFFFF000C, /* not memory avaliable for this operation                */
    TEE_ERROR_BUSY                          = 0xFFFF000D, /* system busy to handle this operation                   */
    TEE_ERROR_COMMUNICATION                 = 0xFFFF000E, /* communication error with target                        */
    TEE_ERROR_SECURITY                      = 0xFFFF000F, /* security error occurs                                  */
    TEE_ERROR_SHORT_BUFFER                  = 0xFFFF0010, /* buffer is too short for this operation                 */
    TEE_ERROR_EXTERNAL_CANCEL               = 0xFFFF0011, /* operation is canceled                                  */
    TEE_PENDING                             = 0xFFFF2000, /* service is in pending state(in asynchronous state)     */
    TEE_PENDING2                            = 0xFFFF2001, /* service is in pending state()                          */
    TEE_PENDING3                            = 0xFFFF2002, /* reserved error definition                              */
    TEE_ERROR_TIMEOUT                       = 0xFFFF3001, /* operation is timeout                                   */
    TEE_ERROR_OVERFLOW                      = 0xFFFF300f, /* operation overflow                                     */
    TEE_ERROR_TARGET_DEAD                   = 0xFFFF3024, /* TA is crashed                                          */
    TEE_ERROR_STORAGE_NO_SPACE              = 0xFFFF3041, /* no enough space to store data                          */
    TEE_ERROR_MAC_INVALID                   = 0xFFFF3071, /* MAC operation failed                                   */
    TEE_ERROR_SIGNATURE_INVALID             = 0xFFFF3072, /* signature check failed                                 */
    TEE_CLIENT_INTR                         = 0xFFFF4000, /* Interrupted by CFC. Broken control flow is detected.   */
    TEE_ERROR_TIME_NOT_SET                  = 0xFFFF5000, /* time is not set                                        */
    TEE_ERROR_TIME_NEEDS_RESET              = 0xFFFF5001, /* time need to be reset                                  */
    TEE_FAIL                                = 0xFFFF5002, /* system error                                           */
    TEE_ERROR_TIMER                         = 0xFFFF6000, /* base value of timer error codes                        */
    TEE_ERROR_TIMER_CREATE_FAILED           = 0xFFFF6001, /* failed to create timer                                 */
    TEE_ERROR_TIMER_DESTROY_FAILED          = 0xFFFF6002, /* failed to destroy timer                                */
    TEE_ERROR_TIMER_NOT_FOUND               = 0xFFFF6003, /* timer not found                                        */
    TEE_ERROR_RPMB_BASE                     = 0xFFFF7000, /* base value of RPMB error codes                         */
    TEE_ERROR_RPMB_GENERIC                  = 0xFFFF7001, /* generic error of RPMB operations                       */
    TEE_ERROR_RPMB_MAC_FAIL                 = 0xFFFF7002, /* verify MAC failed in RPMB operations                   */
    TEE_ERROR_RPMB_COUNTER_FAIL             = 0xFFFF7003, /* invalid counter in RPMB operations                     */
    TEE_ERROR_RPMB_ADDR_FAIL                = 0xFFFF7004, /* addresss check failed in RPMB operations               */
    TEE_ERROR_RPMB_WRITE_FAIL               = 0xFFFF7005, /* failed to write data to RPMB                           */
    TEE_ERROR_RPMB_READ_FAIL                = 0xFFFF7006, /* failed to read data in RPMB                            */
    TEE_ERROR_RPMB_KEY_NOT_PROGRAM          = 0xFFFF7007, /* key is not provisioned in RPMB                         */
    TEE_ERROR_RPMB_RESP_UNEXPECT_MSGTYPE    = 0xFFFF7100, /* incorrect message type in RPMB response                */
    TEE_ERROR_RPMB_RESP_UNEXPECT_BLKCNT     = 0xFFFF7101, /* incorrect message data block count in RPMB response    */
    TEE_ERROR_RPMB_RESP_UNEXPECT_BLKIDX     = 0xFFFF7102, /* incorrect message data block index in RPMB response    */
    TEE_ERROR_RPMB_RESP_UNEXPECT_WRCNT      = 0xFFFF7103, /* incorrect message data counter in RPMB response        */
    TEE_ERROR_RPMB_RESP_UNEXPECT_NONCE      = 0xFFFF7104, /* incorrect message data nonce in RPMB response          */
    TEE_ERROR_RPMB_RESP_UNEXPECT_MAC        = 0xFFFF7105, /* incorrect message data MAC in RPMB response            */
    TEE_ERROR_RPMB_FILE_NOT_FOUND           = 0xFFFF7106, /* file not found in RPMB                                 */
    TEE_ERROR_RPMB_NOSPC                    = 0xFFFF7107, /* not space left for RPMB operations                     */
    TEE_ERROR_RPMB_SPC_CONFLICT             = 0xFFFF7108, /* exceeds max space of RPMB for this TA                  */
    TEE_ERROR_RPMB_NOT_AVAILABLE            = 0xFFFF7109, /* RPMB service not ready                                 */
    TEE_ERROR_RPMB_DAMAGED                  = 0xFFFF710A, /* RPMB partition is damaged                              */
    TEE_ERROR_TUI_IN_USE                    = 0xFFFF7110, /* TUI is being used                                      */
    TEE_ERROR_TUI_SWITCH_CHANNAL            = 0xFFFF7111, /* incorrect message switch channal in TUI response       */
    TEE_ERROR_TUI_CFG_DRIVER                = 0xFFFF7112, /* incorrect message configurator driver in TUI response  */
    TEE_ERROR_TUI_INVALID_EVENT             = 0xFFFF7113, /* invalid TUI event                                      */
    TEE_ERROR_TUI_POLL_EVENT                = 0xFFFF7114, /* incorrect message polling events in TUI response       */
    TEE_ERROR_TUI_CANCELED                  = 0xFFFF7115, /* TUI is cancelled                                       */
    TEE_ERROR_TUI_EXIT                      = 0xFFFF7116, /* TUI is exited                                          */
    TEE_ERROR_TUI_NOT_AVAILABLE             = 0xFFFF7117, /* TUI unavailable                                        */
    TEE_ERROR_SEC_FLASH_NOT_AVAILABLE       = 0xFFFF7118, /* sec flash is not available                             */
    TEE_ERROR_SESRV_NOT_AVAILABLE           = 0xFFFF7119, /* SE service has crashed or not enabled                  */
    TEE_ERROR_BIOSRV_NOT_AVAILABLE          = 0xFFFF711A, /* BIO service is not available                           */
    TEE_ERROR_ROTSRV_NOT_AVAILABLE          = 0xFFFF711B, /* ROT service is not available                           */
    TEE_ERROR_ARTSRV_NOT_AVAILABLE          = 0xFFFF711C, /* ART service is not available                           */
    TEE_ERROR_HSMSRV_NOT_AVAILABLE          = 0xFFFF711D, /* HSM service is not available                           */
    TEE_ERROR_ANTIROOT_RSP_FAIL             = 0xFFFF9110, /* AntiRoot Response verify failed                        */
    TEE_ERROR_ANTIROOT_INVOKE_ERROR         = 0xFFFF9111, /* AntiRoot ERROR during invokecmd                        */
    TEE_ERROR_AUDIT_FAIL                    = 0xFFFF9112, /* audit failed                                           */
    TEE_FAIL2                               = 0xFFFF9113, /* unused                                                 */
    TEE_ERROR_IPC_OVERFLOW                  = 0xFFFF9114, /* IPC Channel overflow error                             */
    TEE_ERROR_APM                           = 0xFFFF9115, /* APM error                                              */
};

/*
 * Login type definitions
 */
enum TEE_LoginMethod {
    TEE_LOGIN_PUBLIC = 0x0,
    TEE_LOGIN_USER,
    TEE_LOGIN_GROUP,
    TEE_LOGIN_APPLICATION      = 0x4,
    TEE_LOGIN_USER_APPLICATION = 0x5,
    TEE_LOGIN_GROUP_APPLICATION = 0x6,
    TEE_LOGIN_IDENTIFY = 0x7, /* iTrustee defined Lognin type */
    TEEK_LOGIN_IDENTIFY = 0x80000001, /* iTrustee defined lognin type from linux kernel */
};

typedef struct {
    uint32_t login;
    TEE_UUID uuid;
} TEE_Identity;

typedef uint32_t TEE_Result;
typedef TEE_Result TEEC_Result;

#define TEE_ORIGIN_TEE             0x00000003
#define TEE_ORIGIN_TRUSTED_APP     0x00000004

#ifndef TEE_TA_SESSION_HANDLE
#define TEE_TA_SESSION_HANDLE
typedef uint32_t TEE_TASessionHandle;
#endif

typedef struct __TEE_ObjectHandle *TEE_ObjectHandle;
typedef struct __TEE_ObjectEnumHandle *TEE_ObjectEnumHandle;
typedef struct __TEE_OperationHandle *TEE_OperationHandle;

#define TEE_TIMEOUT_INFINITE (0xFFFFFFFF)

typedef struct {
    uint32_t seconds;
    uint32_t millis;
} TEE_Time;

typedef struct {
    int32_t seconds;
    int32_t millis;
    int32_t min;
    int32_t hour;
    int32_t day;
    int32_t month;
    int32_t year;
} TEE_Date_Time;

typedef struct {
    uint32_t type;
    uint32_t timer_id;
    uint32_t timer_class;
    uint32_t reserved2;
} TEE_timer_property;

#endif
