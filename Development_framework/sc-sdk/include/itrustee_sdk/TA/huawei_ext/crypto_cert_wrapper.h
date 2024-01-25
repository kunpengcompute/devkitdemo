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
 * Description: soft aes engine
 */
#ifndef __CRYPTO_CERT_WRAPPER_H__
#define __CRYPTO_CERT_WRAPPER_H__

#include <stdint.h>
#include <tee_defines.h>
#include "crypto_x509_wrapper.h"

/*
 * Create attestation certificate with input params.
 *
 * @param cert                   [OUT] The certificate buffer
 * @param cert_len               [IN]  The length of certificate buffer
 * @param valid                  [IN]  The valid date buffer
 * @param issuer_tlv             [IN]  The issuer buffer
 * @param issuer_tlv_len         [IN]  The length of issuer buffer
 * @param subject_public_key     [IN]  The subject public key buffer
 * @param subject_public_key_len [IN]  The length of subject public key buffer
 * @param attestation_ext        [IN]  The attestation extrol info buffer
 * @param attestation_ext_len    [IN]  The length of attestation extrol info buffer
 * @param priv_sign              [IN]  The private key buffer
 * @param key_usage_sign_bit     [IN]  The usage sign flag
 * @param key_usage_encrypt_bit  [IN]  The usage encrypt flag
 * @param keytype                [IN]  The keytype of private key
 * @param hash                   [IN]  The hash func of digest
 *
 * @return -1: Create attestation certificate failed
 * @return others: The real size of certificate
 */
int32_t create_attestation_cert(uint8_t *cert, uint32_t cert_len, const validity_period_t *valid,
                                const uint8_t *issuer_tlv, uint32_t issuer_tlv_len,
                                const uint8_t *subject_public_key, uint32_t subject_public_key_len,
                                const uint8_t *attestation_ext, uint32_t attestation_ext_len, void *priv_sign,
                                uint32_t key_usage_sign_bit, uint32_t key_usage_encrypt_bit, uint32_t key_type,
                                uint32_t hash);

/*
 * Get element number from certificate.
 *
 * @param elem     [OUT] The element with elem_id
 * @param elem_id  [IN]  The index of element
 * @param cert     [IN]  The certificate buffer
 * @param cert_len [IN]  The length of certificate buffer
 *
 * @return -1: Get element failed
 * @return  others: The length of element
 */
int32_t get_tbs_element(uint8_t **elem, uint32_t elem_id, const uint8_t *cert, uint32_t cert_len);

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
TEE_Result tee_verify_dev_cert(uint8_t *cert, uint32_t cert_len, uint8_t *parent_key, uint32_t parent_key_len);

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
TEE_Result tee_create_cert_req(uint8_t *buf, size_t len, uint32_t key_type, uint8_t *file_name);
#endif
