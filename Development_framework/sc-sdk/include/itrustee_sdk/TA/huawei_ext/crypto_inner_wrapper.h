/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: soft inner engine
 */
#ifndef __CRYPTO_INNER_WRAPPER_H__
#define __CRYPTO_INNER_WRAPPER_H__

#include <stdint.h>
#include <tee_defines.h>

/*
 * Get common name from certificate.
 *
 * @param name      [OUT]    The common name buffer
 * @param name_size [IN]     The length of common name buffer
 * @param cert      [IN]     The certificate buffer
 * @param cert_len  [IN]     The length of certificate buffer
 *
 * @return -1: Get common name failed
 * @return  others: Get common name success
 */
int32_t get_subject_CN(uint8_t *name, uint32_t name_size, const uint8_t *cert, uint32_t cert_len);

/*
 * Get organization name from certificate.
 *
 * @param name      [OUT]    The organization name buffer
 * @param name_size [IN]     The length of organization name buffer
 * @param cert      [IN]     The certificate buffer
 * @param cert_len  [IN]     The length of certificate buffer
 *
 * @return -1: Get organization name failed
 * @return  others: Get organization name success
 */
int32_t get_subject_OU(uint8_t *name, uint32_t name_size, const uint8_t *cert, uint32_t cert_len);

/*
 * verify Cert in tee
 *
 * @param cert           [IN]           data for salt
 * @param cert_len       [IN]           salt length
 * @param parent_key     [IN]           size of generated key, fix-size 32 bytes
 * @param parent_key_len [IN]           pointer where key is saved
 *
 * @return TEE_SUCCESS                 OK
 * @return TEE_ERROR_BAD_PARAMETERS    illegal parameters
 * @return TEE_ERROR_GENERIC           internal error
 */
TEE_Result TEE_EXT_verify_dev_cert(uint8_t *cert, uint32_t cert_len, uint8_t *parent_key, uint32_t parent_key_len);

/*
 * create cert request in TEE
 *
 * @param buf          [OUT]            cert request output buffer
 * @param len          [OUT]            output buffer size
 * @param key_type      [IN]            key_type RSA 0; ECC 1
 * @param file_name     [IN]            pointer where key is saved
 *
 * @return TEE_SUCCESS                  operation success
 * @return TEE_ERROR_BAD_PARAMETERS     illegal parameters
 */
TEE_Result TEE_EXT_create_cert_req(uint8_t *buf, size_t len, uint32_t key_type, uint8_t *file_name);
#endif
