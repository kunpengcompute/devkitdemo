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
#ifndef __CRYPTO_EC_X509_WRAPPER_H__
#define __CRYPTO_EC_X509_WRAPPER_H__

#include <stdint.h>
#include <tee_defines.h>
#include "crypto_x509_wrapper.h"

/*
 * Recover the root certificate.
 *
 * @param cert     [OUT]    The certificate buffer
 * @param cert_len [IN]     The length of certificate buffer
 * @param priv     [IN]     The private key structure
 * @param keytype  [IN]     The keytype of private key
 *
 * @return -1: Recover root certificate failed
 * @return others: Recover root certificate success
 */
int32_t recover_root_cert(uint8_t *cert, uint32_t cert_len, const void *priv, uint32_t keytype);

/*
 * Sign the pkcs10 certificate.
 *
 * @param cert          [OUT]    The certificate buffer
 * @param cert_len      [IN]     The length of certificate buffer
 * @param csr           [IN]     The certificate signing request buffer
 * @param csr_len       [IN]     The length of certificate signing request buffer
 * @param valid         [IN]     The valid date buffer
 * @param serial_number [IN]     The serial number buffer
 * @param serial_length [IN]     The length of serial number buffer
 * @param priv          [IN]     The private key structure
 * @param keytype       [IN]     The keytype of private key
 *
 * @return -1: Sign the pkcs10 certificate failed
 * @return others: The real size of certificate
 */
int32_t sign_pkcs10(uint8_t *cert, uint32_t cert_len,
                    const uint8_t *csr, uint32_t csr_len, const validity_period_t *valid,
                    const uint8_t *serial_number, uint32_t serial_length, const void *priv, uint32_t keytype);

#endif
