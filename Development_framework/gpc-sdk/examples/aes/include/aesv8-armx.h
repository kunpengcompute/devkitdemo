#ifndef __AESV8_ARMX_H
#define __AESV8_ARMX_H

#include <stdlib.h>

#define AES_MAXNR 14
struct aes_key_st {
    unsigned int rd_key[4 * (AES_MAXNR + 1)];
    int rounds;
};
typedef struct aes_key_st AES_KEY;

int aes_v8_set_encrypt_key(const unsigned char *userKey, const int bits,
                           AES_KEY *key);

int aes_v8_set_decrypt_key(const unsigned char *userKey, const int bits,
                           AES_KEY *key);

void aes_v8_cbc_encrypt(const unsigned char *in, unsigned char *out,
                        size_t length, const AES_KEY *key,
                        unsigned char *ivec, const int enc);

void aes_v8_ecb_encrypt(const unsigned char *in, unsigned char *out,
                        size_t length, const AES_KEY *key,
                        const int enc);

void aes_v8_encrypt(const unsigned char *in, unsigned char *out,
                    const void *key);

typedef void (*block128_f) (const unsigned char in[16],
                            unsigned char out[16], const void *key);

#endif
