/*
 * Copyright 2022 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description: openssl.c
 * Create: 2022-07-25
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aesv8-armx.h"
#include "aes.h"


/*
 * The input and output encrypted as though 128bit ofb mode is being used.
 * The extra state information to record how much of the 128bit block we have
 * used is contained in *num;
 */
void CRYPTO_ofb128_encrypt(const unsigned char *in, unsigned char *out,
                           size_t len, const void *key,
                           unsigned char ivec[16], int *num, block128_f block)
{
    unsigned int n;
    size_t l = 0;

    if (*num < 0) {
        /* There is no good way to signal an error return from here */
        *num = -1;
        return;
    }
    n = *num;

#if !defined(OPENSSL_SMALL_FOOTPRINT)
    if (16 % sizeof(size_t) == 0) { /* always true actually */
        do {
            while (n && len) {
                *(out++) = *(in++) ^ ivec[n];
                --len;
                n = (n + 1) % 16;
            }
# if defined(STRICT_ALIGNMENT)
            if (((size_t)in | (size_t)out | (size_t)ivec) % sizeof(size_t) !=
                0)
                break;
# endif
            while (len >= 16) {
                (*block) (ivec, ivec, key);
                for (; n < 16; n += sizeof(size_t))
                    *(size_t *)(out + n) =
                        *(size_t *)(in + n)
                        ^ *(size_t *)(ivec + n);
                len -= 16;
                out += 16;
                in += 16;
                n = 0;
            }
            if (len) {
                (*block) (ivec, ivec, key);
                while (len--) {
                    out[n] = in[n] ^ ivec[n];
                    ++n;
                }
            }
            *num = n;
            return;
        } while (0);
    }
    /* the rest would be commonly eliminated by x86* compiler */
#endif
    while (l < len) {
        if (n == 0) {
            (*block) (ivec, ivec, key);
        }
        out[l] = in[l] ^ ivec[n];
        ++l;
        n = (n + 1) % 16;
    }

    *num = n;
}


/*
 * The input and output encrypted as though 128bit cfb mode is being used.
 * The extra state information to record how much of the 128bit block we have
 * used is contained in *num;
 */
void CRYPTO_cfb128_encrypt(const unsigned char *in, unsigned char *out,
                           size_t len, const void *key,
                           unsigned char ivec[16], int *num,
                           int enc, block128_f block)
{
    unsigned int n;
    size_t l = 0;

    if (*num < 0) {
        /* There is no good way to signal an error return from here */
        *num = -1;
        return;
    }
    n = *num;

    if (enc) {
#if !defined(OPENSSL_SMALL_FOOTPRINT)
        if (16 % sizeof(size_t) == 0) { /* always true actually */
            do {
                while (n && len) {
                    *(out++) = ivec[n] ^= *(in++);
                    --len;
                    n = (n + 1) % 16;
                }
# if defined(STRICT_ALIGNMENT)
                if (((size_t)in | (size_t)out | (size_t)ivec) %
                    sizeof(size_t) != 0)
                    break;
# endif
                while (len >= 16) {
                    (*block) (ivec, ivec, key);
                    for (; n < 16; n += sizeof(size_t)) {
                        *(size_t *)(out + n) =
                            *(size_t *)(ivec + n)
                                ^= *(size_t *)(in + n);
                    }
                    len -= 16;
                    out += 16;
                    in += 16;
                    n = 0;
                }
                if (len) {
                    (*block) (ivec, ivec, key);
                    while (len--) {
                        out[n] = ivec[n] ^= in[n];
                        ++n;
                    }
                }
                *num = n;
                return;
            } while (0);
        }
        /* the rest would be commonly eliminated by x86* compiler */
#endif
        while (l < len) {
            if (n == 0) {
                (*block) (ivec, ivec, key);
            }
            out[l] = ivec[n] ^= in[l];
            ++l;
            n = (n + 1) % 16;
        }
        *num = n;
    } else {
#if !defined(OPENSSL_SMALL_FOOTPRINT)
        if (16 % sizeof(size_t) == 0) { /* always true actually */
            do {
                while (n && len) {
                    unsigned char c;
                    *(out++) = ivec[n] ^ (c = *(in++));
                    ivec[n] = c;
                    --len;
                    n = (n + 1) % 16;
                }
# if defined(STRICT_ALIGNMENT)
                if (((size_t)in | (size_t)out | (size_t)ivec) %
                    sizeof(size_t) != 0)
                    break;
# endif
                while (len >= 16) {
                    (*block) (ivec, ivec, key);
                    for (; n < 16; n += sizeof(size_t)) {
                        size_t t = *(size_t *)(in + n);
                        *(size_t *)(out + n)
                            = *(size_t *)(ivec + n) ^ t;
                        *(size_t *)(ivec + n) = t;
                    }
                    len -= 16;
                    out += 16;
                    in += 16;
                    n = 0;
                }
                if (len) {
                    (*block) (ivec, ivec, key);
                    while (len--) {
                        unsigned char c;
                        out[n] = ivec[n] ^ (c = in[n]);
                        ivec[n] = c;
                        ++n;
                    }
                }
                *num = n;
                return;
            } while (0);
        }
        /* the rest would be commonly eliminated by x86* compiler */
#endif
        while (l < len) {
            unsigned char c;
            if (n == 0) {
                (*block) (ivec, ivec, key);
            }
            out[l] = ivec[n] ^ (c = in[l]);
            ivec[n] = c;
            ++l;
            n = (n + 1) % 16;
        }
        *num = n;
    }
}

int ecb_encrypt(AES_KEY *ks, int enc, const unsigned char *in, size_t length, unsigned char *out)
{
    aes_v8_ecb_encrypt(in, out, length, ks, enc);
    return 0;
}

int cbc_encrypt(AES_KEY *ks, int enc, const unsigned char *in, size_t length, unsigned char *out)
{
    char ivec[] = {0xc5, 0xa9, 0x9f, 0x63, 0xec, 0xeb, 0x7f, 0x32,
                   0x92, 0x6b, 0xd6, 0x00, 0x8b, 0xa0, 0x61, 0x87};
    aes_v8_cbc_encrypt(in, out, length, ks, ivec, enc);
    return 0;
}

int cfb_encrypt(AES_KEY *ks, int enc, const unsigned char *in, size_t length, unsigned char *out)
{
    char ivec[] = {0xc5, 0xa9, 0x9f, 0x63, 0xec, 0xeb, 0x7f, 0x32,
                   0x92, 0x6b, 0xd6, 0x00, 0x8b, 0xa0, 0x61, 0x87};
    int num = 0;
    CRYPTO_cfb128_encrypt(in, out, length, ks, ivec, &num, enc, (block128_f)aes_v8_encrypt);
    return 0;
}

int ofb_encrypt(AES_KEY *ks, const unsigned char *in, size_t length, unsigned char *out)
{
    char ivec[] = {0xc5, 0xa9, 0x9f, 0x63, 0xec, 0xeb, 0x7f, 0x32,
                   0x92, 0x6b, 0xd6, 0x00, 0x8b, 0xa0, 0x61, 0x87};
    int num = 0;
    CRYPTO_ofb128_encrypt(in, out, length, ks, ivec, &num, (block128_f)aes_v8_encrypt);
    return 0;
}

int OpenSSLEncrypt(Param *param, char *in, size_t bufSize, char *out, BLOCK_CIPHER_MODE mode)
{
    AES_KEY *ks = (AES_KEY *)calloc(1, sizeof(AES_KEY));
    // CFB OFB 模式加解密都是对IV进行加密操作
    if (mode == CFB_MODE || mode == OFB_MODE) {
        aes_v8_set_encrypt_key(param->key, param->keyLen * 8, ks);
    } else {
        if (param->enc) {
            aes_v8_set_encrypt_key(param->key, param->keyLen * 8, ks);
        } else {
            aes_v8_set_decrypt_key(param->key, param->keyLen * 8, ks);
        }
    }
    switch (mode)
    {
        case ECB_MODE:
            ecb_encrypt(ks, param->enc, in, bufSize, out);
            break;
        case CBC_MODE:
            cbc_encrypt(ks, param->enc, in, bufSize, out);
            break;
        case CFB_MODE:
            cfb_encrypt(ks, param->enc, in, bufSize, out);
            break;
        case OFB_MODE:
            ofb_encrypt(ks, in, bufSize, out);
            break;

        default:
            break;
    }
    free(ks);
    return 0;
}
