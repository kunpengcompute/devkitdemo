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
 * Description: soft ec x509 engine
 */
#ifndef __CRYPTO_X509_WRAPPER_H__
#define __CRYPTO_X509_WRAPPER_H__

#include <stdint.h>
#include <tee_defines.h>

#define VALIDITY_TIME_SIZE 13
typedef struct {
    uint8_t start[VALIDITY_TIME_SIZE];
    uint8_t end[VALIDITY_TIME_SIZE];
} validity_period_t;

/*
 * Check the certificate revocation list.
 *
 * @param cert            [IN] The crl buffer
 * @param cert_len        [IN] The length of crl buffer
 * @param parent_key      [IN] The public key to verify the crl
 * @param parent_key_len  [IN] The length of public key
 *
 * @return  1: Check the crl success
 * @return  others: Check the crl failed
 */
int32_t x509_crl_validate(uint8_t *cert, uint32_t cert_len, uint8_t *parent_key, uint32_t parent_key_len);

/*
 * Check the x509 certificate.
 *
 * @param cert            [IN] The certificate buffer
 * @param cert_len        [IN] The length of certificate buffer
 * @param parent_key      [IN] The public key to verify the crl
 * @param parent_key_len  [IN] The length of public key
 *
 * @return  1: Check the cert success
 * @return  others: Check the cert failed
 */
int32_t x509_cert_validate(uint8_t *cert, uint32_t cert_len, uint8_t *parent_key, uint32_t parent_key_len);

/*
 * Get public key from certificate.
 *
 * @param pub      [OUT] The public key struct
 * @param in       [IN]  The certificate buffer
 * @param inlen    [IN]  The length of certificate buffer
 *
 * @return  0: Get public key success
 * @return -1: Get public key failed
 */
int32_t import_pub_from_sp(void *pub, const uint8_t *in, uint32_t inlen);

/*
 * Get public key from certificate.
 *
 * @param pub      [OUT] The public key buffer
 * @param cert     [IN]  The certificate buffer
 * @param cert_len [IN]  The length of certificate buffer
 *
 * @return -1: Get public key failed
 * @return  others: The length of public key buffer
 */
int32_t get_subject_public_key(uint8_t *pub, const uint8_t *cert, uint32_t cert_len);

/*
 * Get public key from certificate, with pub buffer size check.
 *
 * @param pub      [OUT]    The public key buffer
 * @param pub_size [IN]     The length of public key buffer
 * @param cert     [IN]     The certificate buffer
 * @param cert_len [IN]     The length of certificate buffer
 *
 * @return -1: Get public key failed
 * @return  others: The length of public key buffer
 */
int32_t get_subject_public_key_new(uint8_t *pub, uint32_t pub_size, const uint8_t *cert, uint32_t cert_len);

/*
 * Get valid date from certificate.
 *
 * @param vd       [OUT] The valid data structure
 * @param cert     [IN]  The certificate buffer
 * @param cert_len [IN]  The length of certificate buffer
 *
 * @return  0: Get valid date success
 * @return -1: Get valid data failed
 */
int32_t get_validity_from_cert(validity_period_t *vd, uint8_t *cert, uint32_t cert_len);

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
int32_t get_subject_x509_cn(uint8_t *name, uint32_t name_size, const uint8_t *cert, uint32_t cert_len);

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
int32_t get_subject_x509_ou(uint8_t *name, uint32_t name_size, const uint8_t *cert, uint32_t cert_len);

/*
 * Get serial number from certificate.
 *
 * @param serial_number      [OUT]    The serial number buffer
 * @param serial_number_size [IN]     The length of serial number buffer
 * @param cert               [IN]     The certificate buffer
 * @param cert_len           [IN]     The length of certificate buffer
 *
 * @return -1: Get serial number failed
 * @return  others: Get serial number success
 */
int32_t get_serial_number_from_cert(uint8_t *serial_number, uint32_t serial_number_size,
    uint8_t *cert, uint32_t cert_len);

/*
 * Get issuer from certificate.
 *
 * @param issuer      [OUT]    The issuer buffer
 * @param issuer_size [IN]     The length of issuer buffer
 * @param cert        [IN]     The certificate buffer
 * @param cert_len    [IN]     The length of certificate buffer
 *
 * @return -1: Get serial number failed
 * @return  others: Get serial number success
 */
int32_t get_issuer_from_cert(uint8_t *issuer, uint32_t issuer_size, uint8_t *crl, uint32_t crl_len);

/*
 * Check cert chain valid.
 *
 * @param root_cert      [IN] root ca cert presave
 * @param root_cert_len [IN] The length of root ca cert
 * @param second_cert        [IN] secondary ca cert buffer
 * @param second_cert_len    [IN] The length of secondary ca cert
 * @param leaf_cert        [IN] leaf ca cert
 * @param leaf_cert_len    [IN] The length of leaf ca cert
 *
 * @return -1: failed
 * @return >0: check success
 */
int x509_cert_chain_validate(const uint8_t *root_cert, uint32_t root_cert_len,
                             const uint8_t *second_cert, uint32_t second_cert_len,
                             const uint8_t *leaf_cert, uint32_t leaf_cert_len);
#endif
