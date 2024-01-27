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
 * Description: soft rsa engine
 */
#ifndef __CRYPTO_RSA_WRAPPER_H__
#define __CRYPTO_RSA_WRAPPER_H__

#include <stdint.h>
#include <tee_defines.h>

#define RSA_PUB_LEN                  1024
#define RSA_PRIV_LEN                 512

typedef struct {
    uint8_t e[RSA_PUB_LEN];
    uint32_t e_len;
    uint8_t n[RSA_PUB_LEN];
    uint32_t n_len;
} rsa_pub_key_t;

typedef struct {
    uint8_t e[RSA_PUB_LEN];
    uint32_t e_len;
    uint8_t n[RSA_PUB_LEN];
    uint32_t n_len;
    uint8_t d[RSA_PUB_LEN];
    uint32_t d_len;
    uint8_t p[RSA_PRIV_LEN];
    uint32_t p_len;
    uint8_t q[RSA_PRIV_LEN];
    uint32_t q_len;
    uint8_t dp[RSA_PRIV_LEN];
    uint32_t dp_len;
    uint8_t dq[RSA_PRIV_LEN];
    uint32_t dq_len;
    uint8_t qinv[RSA_PRIV_LEN];
    uint32_t qinv_len;
} rsa_priv_key_t;

/*
 * Generate rsa key pair.
 * @param priv      [OUT] The rsa private key structure
 * @param pub       [OUT] The rsa public key structure
 * @param e         [IN]  The exponent of rsa key
 * @param key_size  [IN]  The size of rsa key
 *
 * @return  0: Generate rsa keypair success
 * @return -1: Generate rsa keypair failed
 */
int32_t rsa_generate_keypair(rsa_priv_key_t *priv, rsa_pub_key_t *pub, uint32_t e, uint32_t key_size);

/*
 * Do rsa encryption.
 *
 * @param dest_data [OUT]    The dest data buffer
 * @param dest_len  [IN/OUT] The length of dest data
 * @param src_data  [IN]     The src data buffer
 * @param src_len   [IN]     The length of src data
 * @param pub       [IN]     The rsa public key structure
 * @param padding   [IN]     The padding type of encryption
 * @param hash_nid  [IN]     The hash_nid of encryption
 *
 * @return  0: Do rsa encryption success
 * @return -1: Do rsa encryption failed
 */
int32_t rsa_encrypt(uint8_t *dest_data, uint32_t *dest_len, uint8_t *src_data, uint32_t src_len, rsa_pub_key_t *pub,
                    int32_t padding, int32_t hash_nid);

/*
 * Do rsa decryption.
 *
 * @param dest_data [OUT]    The dest data buffer
 * @param dest_len  [IN/OUT] The length of dest data
 * @param src_data  [IN]     The src data buffer
 * @param src_len   [IN]     The length of src data
 * @param priv      [IN]     THE rsa private key structure
 * @param padding   [IN]     The padding type of encryption
 * @param hash_nid  [IN]     The hash_nid of encryption
 *
 * @return  0: Do rsa decryption success
 * @return -1: Do rsa decryption failed
 */
int32_t rsa_decrypt(uint8_t *dest_data, uint32_t *dest_len, uint8_t *src_data, uint32_t src_len, rsa_priv_key_t *priv,
                    uint32_t padding, int32_t hash_nid);

/*
 * Do rsa Sign digest.
 *
 * @param signature  [OUT]    The signature of input data
 * @param sig_size   [IN/OUT] The length of signature
 * @param in         [IN]     The input data
 * @param in_len     [IN]     The length of input data
 * @param priv       [IN]     The rsa  private key structure
 * @param salt_len   [IN]     The length of salt
 * @param hash_nid   [IN]     The hash_nid of encryption
 * @param padding    [IN]     The padding type of encryption
 *
 * @return  0: Do rsa sign digest success
 * @return -1: Do rsa Sign digest failed
 */
int32_t rsa_sign_digest(uint8_t *signature, uint32_t *sig_size, uint8_t *in, uint32_t in_len, rsa_priv_key_t *priv,
                        uint32_t salt_len, int32_t hash_nid, int32_t padding);

/*
 * Do rsa Verify digest.
 *
 * @param signature  [IN]  The signature of input data
 * @param sig_size   [IN]  The length of signature
 * @param in         [IN]  The input data
 * @param in_len     [IN]  The length of input data
 * @param pub        [IN]  The rsa public key structure
 * @param salt_len   [IN]  The length of salt
 * @param hash_nid   [IN]  The hash_nid of encryption
 * @param padding    [IN]  The padding type of encryption
 *
 * @return  0: Do rsa verify success
 * @return -1: Do rsa verify failed
 */
int32_t rsa_verify_digest(uint8_t *signature, uint32_t sig_size, uint8_t *in, uint32_t in_len, const rsa_pub_key_t *pub,
                          uint32_t salt_len, int32_t hash_nid, int32_t padding);

/*
 * Convert the rsa private key passed in by the user into the rsa_priv_key_t structure.
 *
 * @param priv  [OUT] The rsa private key structure
 * @param in    [IN]  The rsa private key buffer
 * @param inlen [IN]  The length of rsa private key buffer
 *
 * @return -1: Import rsa private key failed
 * @return  0: Import rsa private key success
 */
int32_t rsa_import_priv(rsa_priv_key_t *priv, const uint8_t *in, uint32_t in_len);

/*
 * Convert the rsa_pub_key_t structure passed in by the user into rsa public key buffer.
 *
 * @param out    [OUT] The rsa public key buffer
 * @param outlen [IN]  The length of rsa public key buffer
 * @param pub    [IN]  The rsa public key structure
 *
 * @return -1: Export rsa public key failed
 * @return  others: The real size of out buffer
 */
int32_t rsa_export_pub_sp(uint8_t *out, uint32_t out_size, rsa_pub_key_t *pub);

#endif
