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
 * Description: soft ec engine
 */
#ifndef __CRYPTO_EC_WRAPPER_H__
#define __CRYPTO_EC_WRAPPER_H__

#include <stdint.h>
#include <tee_defines.h>

#define ECC_PRIV_LEN                 66
#define ECC_PUB_LEN                  66

struct ec_pub_info {
    uint8_t *x;
    uint32_t x_len;
    uint8_t *y;
    uint32_t y_len;
};

struct ec_priv_info {
    uint32_t nid;
    uint8_t *r;
    uint32_t r_len;
};

typedef struct {
    uint32_t domain;
    uint8_t x[ECC_PUB_LEN];
    uint32_t x_len;
    uint8_t y[ECC_PUB_LEN];
    uint32_t y_len;
} ecc_pub_key_t;

typedef struct {
    uint32_t domain;
    uint8_t r[ECC_PRIV_LEN];
    uint32_t r_len;
} ecc_priv_key_t;

/*
 * Derive ecc public key from private key.
 *
 * @param priv_info  [IN]  The ecc_priv_key_t structure
 * @param pub_info   [OUT] The ecc_pub_key_t structure
 *
 * @return  0: Derive ecc public key success
 * @return -1: Derive ecc public key failed
 */
int32_t ecc_derive_public_key(ecc_priv_key_t *priv_info, ecc_pub_key_t *pub_info);

/*
 * Derive ecc private key from huk.
 *
 * @param priv     [OUT] The ecc_priv_key_t structure
 * @param secret   [IN]  The huk buffer
 * @param sec_len  [IN]  The length of huk buffer
 *
 * @return  0: Derive ecc private key success
 * @return -1: Derive ecc private key failed
 */
int32_t derive_ecc_private_key_from_huk(ecc_priv_key_t *priv, const uint8_t *secret, uint32_t sec_len);

/*
 * Convert the ecc_pub_key_t structure passed in by the user into ecc public key buffer.
 *
 * @param out    [OUT]    The ecc public key buffer
 * @param outlen [IN] The length of ecc public key buffer
 * @param pub    [IN]     The ecc public key structure
 *
 * @return -1: Export ecc public key failed
 * @return  others: The real size of out buffer
 */
int32_t ecc_export_pub(uint8_t *out, uint32_t out_size, ecc_pub_key_t *pub);

/*
 * Convert the ecc public key passed in by the user into the ecc_pub_key_t structure.
 *
 * @param pub   [OUT] The ecc public key structure
 * @param in    [IN]  The ecc public key buffer
 * @param inlen [IN]  The length of ecc public key buffer
 *
 * @return  1: Import ecc public key success
 * @return -1: Import ecc public key failed
 */
int32_t ecc_import_pub(ecc_pub_key_t *pub, const uint8_t *in, uint32_t inlen);

/*
 * Convert the ecc private key passed in by the user into the ecc_priv_key_t structure.
 *
 * @param priv  [OUT] The ecc private key structure
 * @param in    [IN]  The ecc private key buffer
 * @param inlen [IN]  The length of ecc private key buffer
 *
 * @return -1: Import ecc private key failed
 * @return  others: The width of  ecc private key
 */
int32_t ecc_import_priv(ecc_priv_key_t *priv, const uint8_t *in, uint32_t inlen);

/*
 * Read next TLV (Type-Length-Value) from ASN1 buffer.
 *
 * @param type        [OUT] Type of TLV
 * @param header_len  [OUT] Length of TLV
 * @param buf         [IN]  Input TLV
 * @param buf_len     [IN]  Length of buf in bytes
 *
 * @return -1: Get next TLV failed
 * @return  others: Length of next TLV
 */
int32_t get_next_tlv(uint32_t *type, uint32_t *header_len, const uint8_t *buf, uint32_t buf_len);

/*
 * Use ECC algorithm to sign user data.
 *
 * @param signature  [OUT]    The signature of input data
 * @param sig_siz    [IN]     The length of signature
 * @param in         [IN]     The data to be sign
 * @param in_len     [IN]     The length of input data
 * @param priv       [IN]     The ecc private key structure
 *
 * @return -1: Sign input buffer use ecc failed
 * @return  others: The length of signature
 */
int32_t ecc_sign_digest(uint8_t *signature, uint32_t sig_size, uint8_t *in, uint32_t in_len, ecc_priv_key_t *priv);

/*
 * Verify the data with ECC algorithm.
 *
 * @param signature  [IN]  The signature of input data
 * @param sig_len    [IN]  The length of signature
 * @param in         [IN]  The input data
 * @param in_len     [IN]  The length of input data
 * @param pub        [IN]  The ecc public key structure
 *
 * @return  1: Verify digest success
 * @return -1: Verify digest failed
 */
int32_t ecc_verify_digest(const uint8_t *signature, uint32_t sig_len, uint8_t *in, uint32_t in_len, ecc_pub_key_t *pub);

#endif
