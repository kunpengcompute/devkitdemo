/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: Functions in this header file are deprecated. Do not use
 */

#ifndef __TEE_HW_EXT_API_LEGACY_H__
#define __TEE_HW_EXT_API_LEGACY_H__

#include "tee_defines.h"
#include "tee_crypto_api.h"

/*
 * check wheather target TA(uuid) has permission to invoke target command
 * this feature is only supported by TA with certificate
 *
 * @param [IN] uuid of caller TA
 * @param [IN] cmd id of request
 *
 * @return TEE_SUCCESS target TA has the permission to invoke target command
 * @return TEE_ERROR_ACCESS_DENIED target TA don't has permission to invoke target command
 */
TEE_Result TEE_EXT_CheckInvokePermission(const TEE_UUID *uuid, uint32_t cmd);

/*
 * derive key from device rootkey and UUID of the current task
 *
 * @param salt          [IN]            data for salt
 * @param size          [IN]            salt length
 * @param key           [OUT]           pointer where key is saved
 * @param key_size      [IN]            key_size must be integer times of 16
 *
 * @return TEE_SUCCESS                 OK
 * @return TEE_ERROR_BAD_PARAMETERS    illegal parameters
 * @return TEE_ERROR_GENERIC           internal error
 */
TEE_Result TEE_EXT_DeriveTARootKey(const uint8_t *salt, uint32_t size, uint8_t *key, uint32_t key_size);

/*
 * get device unique id in TEE
 *
 * @param device_unique_id [OUT] buffer to store the result
 * @param length           [IN/OUT] buffer length of device ID
 *
 * return TEE_SUCCESS operation success
 * return others operation failed
 */
TEE_Result TEE_EXT_GetDeviceUniqueId(uint8_t *device_unique_id, uint32_t *length);

/*
 * @ingroup  derive key for keymaster
 * @brief  using root key to derive key for keymaster
 *
 * @par
 * @param buffer [IN] input secret
 * @param size   [IN] input secret size
 * @param buffer [OUT] derived key
 * @param size   [OUT] derived key size
 *
 * @retval NA
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
 */
TEE_Result TEE_EXT_ROOT_DeriveKey2(const uint8_t *secret, uint32_t secret_len, uint8_t *key, uint32_t key_len);

/*
 * @ingroup  ROOT_UuidDeriveKey
 * @brief  derive key from device rootkey and UUID of the current task
 *
 * @attention
 * @param salt          [IN]            data for salt
 * @param size          [IN]            salt length
 * @param key_size      [OUT]           size of generated key, fix-size 32 bytes
 * @param key           [OUT]           pointer where key is saved
 *
 * @retval #TEE_SUCCESS                 OK
 * @retval #TEE_ERROR_BAD_PARAMETERS    illegal parameters
 * @retval #TEE_ERROR_GENERIC           internal error
 *
 * @par dependence:
 * @li tee_ext_api.h
 * @see
 * @since V100R007C00
 */
TEE_Result TEE_EXT_ROOT_UuidDeriveKey(const uint8_t *salt, uint32_t size, uint8_t *key, uint32_t *key_size);

/*
 * @ingroup crypto
 * @brief AES encryption and decryption integration mode.
 *
 * @attention: Now this api only support algorithm: AES_CBC_NOPAD/ AES_ECB_PKCS5,
 *             and input key can derive by root key.
 * @param db        [IN]        Whether to derive the key with the input key and root key.
 * @param iv        [IN]        input iv
 * @param mode      [IN]        0:encrypt; 1:decrypt
 * @param alg       [IN]        AES algorithm: TEE_CRYPTO_ALGORITHM_ID
 * @param key       [IN]        input key
 * @param key_len   [IN]        input key len
 * @param cyp_src   [IN]        input data
 * @param dst       [OUT]       ouput data
 * @param src_len   [IN]        input data len
 * @param dst_len   [OUT]       ouput data len
 *
 * @retval #TEE_SUCCESS                 success
 * @retval #TEE_ERROR_BAD_PARAMETERS    illegal parameters
 * @retval #TEE_ERROR_OUT_OF_MEMORY     alloc memory fail
 *
 * @par dependence:
 * @li crypto
 * @li tee_ext_api.h
 * @see TEE_CipherInit | TEE_CipherUpdate | TEE_CipherDoFinal
 * @since V100R005C00
 */
TEE_Result TEE_EXT_AES_CRYPTO(bool db, const uint8_t *iv, bool mode, uint32_t alg, const uint8_t *key, uint32_t key_len,
                              const uint8_t *cyp_src, uint8_t *dst, uint32_t src_len, uint32_t *dst_len);

/*
 * @ingroup  crypto
 * @brief calculate hash.
 *
 * @attention
 * @param algorithm     [IN]            hash algorithm: like SHA1, SHA256
 * @param ptr           [IN]            input data
 * @param size          [IN]            input data length
 * @param hash_result   [OUT]           hash result: SHA1 20bytes; SHA256 32bytes
 *
 * @retval #TEE_SUCCESS                 success
 * @retval #TEE_ERROR_BAD_PARAMETERS    illegal parameters
 * @retval #TEE_ERROR_GENERIC           error
 *
 * @par dependence:
 * @li crypto
 * @li tee_ext_api.h
 * @since V100R005C00
 */
TEE_Result TEE_EXT_HASH(tee_crypto_algorithm_id algorithm, const uint8_t *ptr, uint32_t size, uint8_t *hash_result);

/*
 * @ingroup  TEE_HWI_IPC_MESSAGE
 * @brief Wait for registered messages from IRQs
 *
 * @param hwi [IN] IRQ number to register
 */
TEE_Result TEE_EXT_HwiMsgRegister(uint32_t hwi);

/*
 * @ingroup  TEE_HWI_IPC_MESSAGE
 * @brief Unregister to receive message for HWI number
 *
 * @param hwi     [IN]        IRQ number to unregister for
 */
TEE_Result TEE_EXT_HwiMsgDeregister(uint32_t hwi);

/*
 * @ingroup  TEE_HWI_IPC_MESSAGE
 * @brief Wait for registered messages from IRQs
 *
 * @param NULL
 *
 * @retval received message id
 */
uint32_t TEE_EXT_HwiMsgWait(void);

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

struct ta_caller_info;

/*
 * Get caller info of current session, refer caller_info struct for more details
 *
 * @param caller_info_data [OUT] caller info to be returned
 * @param length           [IN] sizeof struct caller_info
 *
 * return TEE_SUCCESS operation success
 * return others failed to get caller info
 */
TEE_Result TEE_EXT_GetCallerInfo(struct ta_caller_info *caller_info_data, uint32_t length);

/*
 * get cruurent session type
 *
 * @return session type of current session
 */
uint32_t TEE_GetSessionType(void);

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
TEE_Result TEE_EXT_DeriveTAPlatfromKeys(TEE_ObjectHandle object, uint32_t key_size, const TEE_Attribute *params,
                                        uint32_t param_count, const uint8_t *exinfo, uint32_t exinfo_size);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif
