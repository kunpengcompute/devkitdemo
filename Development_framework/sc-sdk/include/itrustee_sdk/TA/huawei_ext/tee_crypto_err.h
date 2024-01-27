/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: tee crypto err definitions
 */

#ifndef TEE_CRYPTO_ERR_H
#define TEE_CRYPTO_ERR_H

#include <tee_openssl_err.h>

#define MAX_COMMON_CRYPTO_ENGINE_ERR   99
/*
 * notes: all extension tee error rule is TEE_EXT_ERROR_BASE | CRYPTO_MODULE_ERR_ID | ALG_ERR_ID | base_error_value,
 * For example, the crypto module extension error codes are prefixed with 0x8002,
 * and base_error_value is opensoure lib err.
 */
enum soft_algthorm_err_id {
    SM2_LIB_ERR_ID         = 0x0000, /* GMSSL common err and ec err */
    SM4_LIB_ERR_ID         = 0x1000, /* GMSSL evp err */
    /* as below: for openssl opensource lib */
    COMM_LIB_ERR_ID        = 0x2000,
    BN_LIB_ERR_ID          = 0x3000,
    RSA_LIB_ERR_ID         = 0x4000,
    EVP_LIB_ERR_ID         = 0x5000,
    PEM_LIB_ERR_ID         = 0x6000,
    X509_LIB_ERR_ID        = 0x7000,
    ASN1_LIB_ERR_ID        = 0x8000,
    CRYPTO_LIB_ERR_ID      = 0x9000,
    EC_LIB_ERR_ID          = 0xa000,
    PKCS7_LIB_ERR_ID       = 0xb000,
    OTHER_LIB_ERR_ID       = 0xc000,
};

#endif
