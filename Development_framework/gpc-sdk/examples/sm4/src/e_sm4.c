
#include "sm4.h"
#include "option.h"

#if !defined(STRICT_ALIGNMENT) && !defined(PEDANTIC)
# define STRICT_ALIGNMENT 0
#endif

#if defined(__GNUC__) && !STRICT_ALIGNMENT
typedef size_t size_t_aX __attribute((__aligned__(1)));
#else
typedef size_t size_t_aX;
#endif

void CRYPTO_cbc128_encrypt(const unsigned char *in, unsigned char *out,
                           size_t len, const void *key,
                           unsigned char ivec[16], block128_f block)
{
    size_t n;
    const unsigned char *iv = ivec;

    if (len == 0)
        return;

#if !defined(OPENSSL_SMALL_FOOTPRINT)
    if (STRICT_ALIGNMENT &&
        ((size_t)in | (size_t)out | (size_t)ivec) % sizeof(size_t) != 0) {
        while (len >= 16) {
            for (n = 0; n < 16; ++n)
                out[n] = in[n] ^ iv[n];
            (*block) (out, out, key);
            iv = out;
            len -= 16;
            in += 16;
            out += 16;
        }
    } else {
        while (len >= 16) {
            for (n = 0; n < 16; n += sizeof(size_t))
                *(size_t_aX *)(out + n) =
                    *(size_t_aX *)(in + n) ^ *(size_t_aX *)(iv + n);
            (*block) (out, out, key);
            iv = out;
            len -= 16;
            in += 16;
            out += 16;
        }
    }
#endif
    while (len) {
        for (n = 0; n < 16 && n < len; ++n)
            out[n] = in[n] ^ iv[n];
        for (; n < 16; ++n)
            out[n] = iv[n];
        (*block) (out, out, key);
        iv = out;
        if (len <= 16)
            break;
        len -= 16;
        in += 16;
        out += 16;
    }
    if (ivec != iv)
        memcpy(ivec, iv, 16);
}

void CRYPTO_cbc128_decrypt(const unsigned char *in, unsigned char *out,
                           size_t len, const void *key,
                           unsigned char ivec[16], block128_f block)
{
    size_t n;
    union {
        size_t t[16 / sizeof(size_t)];
        unsigned char c[16];
    } tmp;

    if (len == 0)
        return;

#if !defined(OPENSSL_SMALL_FOOTPRINT)
    if (in != out) {
        const unsigned char *iv = ivec;

        if (STRICT_ALIGNMENT &&
            ((size_t)in | (size_t)out | (size_t)ivec) % sizeof(size_t) != 0) {
            while (len >= 16) {
                (*block) (in, out, key);
                for (n = 0; n < 16; ++n)
                    out[n] ^= iv[n];
                iv = in;
                len -= 16;
                in += 16;
                out += 16;
            }
        } else if (16 % sizeof(size_t) == 0) { /* always true */
            while (len >= 16) {
                size_t_aX *out_t = (size_t_aX *)out;
                size_t_aX *iv_t = (size_t_aX *)iv;

                (*block) (in, out, key);
                for (n = 0; n < 16 / sizeof(size_t); n++)
                    out_t[n] ^= iv_t[n];
                iv = in;
                len -= 16;
                in += 16;
                out += 16;
            }
        }
        if (ivec != iv)
            memcpy(ivec, iv, 16);
    } else {
        if (STRICT_ALIGNMENT &&
            ((size_t)in | (size_t)out | (size_t)ivec) % sizeof(size_t) != 0) {
            unsigned char c;
            while (len >= 16) {
                (*block) (in, tmp.c, key);
                for (n = 0; n < 16; ++n) {
                    c = in[n];
                    out[n] = tmp.c[n] ^ ivec[n];
                    ivec[n] = c;
                }
                len -= 16;
                in += 16;
                out += 16;
            }
        } else if (16 % sizeof(size_t) == 0) { /* always true */
            while (len >= 16) {
                size_t c;
                size_t_aX *out_t = (size_t_aX *)out;
                size_t_aX *ivec_t = (size_t_aX *)ivec;
                const size_t_aX *in_t = (const size_t_aX *)in;

                (*block) (in, tmp.c, key);
                for (n = 0; n < 16 / sizeof(size_t); n++) {
                    c = in_t[n];
                    out_t[n] = tmp.t[n] ^ ivec_t[n];
                    ivec_t[n] = c;
                }
                len -= 16;
                in += 16;
                out += 16;
            }
        }
    }
#endif
    while (len) {
        unsigned char c;
        (*block) (in, tmp.c, key);
        for (n = 0; n < 16 && n < len; ++n) {
            c = in[n];
            out[n] = tmp.c[n] ^ ivec[n];
            ivec[n] = c;
        }
        if (len <= 16) {
            for (; n < 16; ++n)
                ivec[n] = in[n];
            break;
        }
        len -= 16;
        in += 16;
        out += 16;
    }
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
                        *(size_t_aX *)(out + n) =
                            *(size_t_aX *)(ivec + n)
                                ^= *(size_t_aX *)(in + n);
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
                        size_t t = *(size_t_aX *)(in + n);
                        *(size_t_aX *)(out + n)
                            = *(size_t_aX *)(ivec + n) ^ t;
                        *(size_t_aX *)(ivec + n) = t;
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
                    *(size_t_aX *)(out + n) =
                        *(size_t_aX *)(in + n)
                        ^ *(size_t_aX *)(ivec + n);
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

void sm4_ecb_encrypt(const unsigned char *in, unsigned char *out,
                     size_t len, const SM4_KEY *key, const int enc)
{
    unsigned int i = 0;
    for (; i < len; i += 16) {
        if (enc) {
            SM4_encrypt(in, out, key);
        } else {
            SM4_decrypt(in, out, key);
        }
        in += 16;
        out += 16;
    }
}

void sm4_cbc_encrypt(const unsigned char *in, unsigned char *out,
                            size_t len, const SM4_KEY *key,
                            unsigned char *ivec, const int enc)
{
    if (enc) {
        CRYPTO_cbc128_encrypt(in, out, len, key, ivec,
                              (block128_f)SM4_encrypt);
    } else {
        CRYPTO_cbc128_decrypt(in, out, len, key, ivec, 
                              (block128_f)SM4_decrypt);
    }
}

void sm4_cfb128_encrypt(const unsigned char *in, unsigned char *out,
                               size_t length, const SM4_KEY *key,
                               unsigned char *ivec, int *num, const int enc)
{
    CRYPTO_cfb128_encrypt(in, out, length, key, ivec, num, enc,
                          (block128_f)SM4_encrypt);
}

void sm4_ofb128_encrypt(const unsigned char *in, unsigned char *out,
                               size_t length, const SM4_KEY *key,
                               unsigned char *ivec, int *num)
{
    CRYPTO_ofb128_encrypt(in, out, length, key, ivec, num,
                          (block128_f)SM4_encrypt);
}

int opensslEncrypt(Param *param, char *in, size_t bufSize, char *out, BLOCK_CIPHER_MODE mode)
{
    int num = 0;
    char iv[] = "1234567812345678";
    SM4_KEY *ks = (SM4_KEY *)calloc(1, sizeof(SM4_KEY));
    SM4_set_key(param->key, ks);
    switch (mode)
    {
        case ECB_MODE:
            sm4_ecb_encrypt(in, out, bufSize, ks, param->enc);
            break;
        case CBC_MODE:
            sm4_cbc_encrypt(in, out, bufSize, ks, iv, param->enc);
            break;
        case CFB_MODE:
            sm4_cfb128_encrypt(in, out, bufSize, ks, iv, &num, param->enc);
            break;
        case OFB_MODE:
            sm4_ofb128_encrypt(in, out, bufSize, ks, iv, &num);
            break;
    }
    free(ks);
    return 0;
}
