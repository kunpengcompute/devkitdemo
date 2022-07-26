
#ifndef HEADER_SM4_H
#define HEADER_SM4_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

# define SM4_ENCRYPT     1
# define SM4_DECRYPT     0

# define SM4_BLOCK_SIZE    16
# define SM4_KEY_SCHEDULE  32

typedef struct SM4_KEY_st {
    uint32_t rk[SM4_KEY_SCHEDULE];
} SM4_KEY;

int SM4_set_key(const uint8_t *key, SM4_KEY *ks);
void SM4_encrypt(const uint8_t *in, uint8_t *out, const SM4_KEY *ks);
void SM4_decrypt(const uint8_t *in, uint8_t *out, const SM4_KEY *ks);

void sm4_ecb_encrypt(const unsigned char *in, unsigned char *out, 
                     size_t len, const SM4_KEY *key, const int enc);

void sm4_cbc_encrypt(const unsigned char *in, unsigned char *out,
                     size_t len, const SM4_KEY *key,
                     unsigned char *ivec, const int enc);

void sm4_cfb128_encrypt(const unsigned char *in, unsigned char *out,
                        size_t length, const SM4_KEY *key,
                        unsigned char *ivec, int *num, const int enc);

void sm4_ofb128_encrypt(const unsigned char *in, unsigned char *out,
                        size_t length, const SM4_KEY *key,
                        unsigned char *ivec, int *num);

typedef void (*block128_f) (const unsigned char in[16],
                            unsigned char out[16], const void *key);

#endif
