/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: Tee ext api header file
 */

#ifndef TEE_EXT_API_H
#define TEE_EXT_API_H

#include "tee_defines.h"
#include "tee_hw_ext_api_legacy.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/*
 * below definitions are defined by Platform SDK released previously
 * for compatibility:
 * don't make any change to the content below
 */
#define TEE_INFO_RESERVED       0xFFFFFFFF
#define TEE_RETURN_AGENT_BUFFER 0x99
#define TEE_INVALID_AGENT       0x66
#define TEE_AGENT_LOCK          0x33

#define TEE_GET_REEINFO_SUCCESS 0
#define TEE_GET_REEINFO_FAILED  1

#define TEE_SMC_FROM_USR    0
#define TEE_SMC_FROM_KERNEL 1

#define RESERVED_BUF_SIZE 32
typedef struct ta_caller_info {
    uint32_t session_type;
    union {
        struct {
            TEE_UUID caller_uuid;
            uint32_t group_id;
        };
        uint8_t ca_info[RESERVED_BUF_SIZE];
    } caller_identity;
    uint8_t smc_from_kernel_mode;
    uint8_t reserved[RESERVED_BUF_SIZE - 1];
} caller_info;

/*
 * Get caller info of current session, refer caller_info struct for more details
 *
 * @param caller_info_data [OUT] caller info to be returned
 * @param length           [IN] sizeof struct caller_info
 *
 * return TEE_SUCCESS operation success
 * return others failed to get caller info
 */
TEE_Result tee_ext_get_caller_info(caller_info *caller_info_data, uint32_t length);

/*
 * Get group info of current service
 *
 * @param group_id [OUT] group info to be returned
 *
 * return TEE_SUCCESS operation success
 * return others failed to get group info
 */
TEE_Result tee_ext_get_group_id(uint32_t *group_id);

/*
 * Get nsid of current service
 *
 * @param nsid [OUT] nsid to be returned
 *
 * return TEE_SUCCESS operation success
 * return others failed to get nsid
 */
TEE_Result tee_ext_get_nsid(uint32_t *nsid);

/*
 * verify TA's caller's identify
 * TA can call this API to add caller's info,
 * which is allowed to call this TA.
 * this API is for CA in form of JAR or binary-excuteble file.
 *
 * @param ca_name       [IN]        CA caller's process name
 * @param user_name     [IN]        CA caller's username
 *
 * return TEE_SUCCESS operation
 * return others failed to add caller info for target CA
 */
TEE_Result addcaller_ca_exec(const char *ca_name, const char *user_name);

/*
 * verify TA's caller's identify
 * TA can call this API to add caller's info,
 * which is allowed to call this TA.
 * this API is for CA in form of JAR or binary-excuteble file.
 *
 * @param ca_name       [IN]        CA caller's process name
 * @param user_name     [IN]        CA caller's username
 *
 * return TEE_SUCCESS operation
 * return others failed to add caller info for target CA
 */

TEE_Result AddCaller_CA_user(const char *ca_name, const char *user_name);

/*
 * TA can call this API to add caller's info,
 * which is allowed to call this CA.
 * this API is for CA in form of native ca and APK.
 *
 * @param cainfo_hash [IN] CA callerinfo's sha256 value
 *
 * return TEE_SUCCESS operation
 */
TEE_Result AddCaller_CA(const uint8_t *cainfo_hash, uint32_t length);

/*
 * TA call this API allow others TA open session with itself
 *
 * return TEE_SUCCESS operation success
 * result others operation failed
 */
TEE_Result AddCaller_TA_all(void);

#define SESSION_FROM_CA      0
#define SESSION_FROM_TA      1
#define SESSION_FROM_UNKNOWN 0xFF
/*
 * get cruurent session type
 *
 * @return session type of current session
 */
uint32_t tee_get_session_type(void);

/*
 * Check CA params during CA Authentication
 *
 * @param param_types             [IN] CA caller's param types
 * @param params[TEE_PARAMS_NUM]  [IN] CA caller's params
 *
 * return TEE_SUCCESS
 */
TEE_Result TEE_EXT_CheckClientPerm(uint32_t param_types, const TEE_Param params[TEE_PARAMS_NUM]);

/*
 * derive key from platform key
 *
 * @param object      [IN/OUT] input data in ObjectInfo->keytype, output keys in Attributes.
 * @param keySize     [IN]     key size in bits, it desides the ecc curve type too.
 * @param params      [IN]     unused
 * @param paramCount  [IN]     unused
 * @param exinfo      [IN]     user info as derive slat.
 * @param exinfo_size [IN]     size of user info, Max is 64bytes, must bigger than 0.
 *
 * @return TEE_SUCCESS means success, others means failed.
 */
TEE_Result tee_ext_derive_ta_platfrom_keys(TEE_ObjectHandle object, uint32_t key_size, const TEE_Attribute *params,
    uint32_t param_count, const uint8_t *exinfo, uint32_t exinfo_size);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
