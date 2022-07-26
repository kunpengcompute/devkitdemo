#ifndef __AES_H
#define __AES_H

#include <stdlib.h>

typedef enum {
    ECB_MODE = 0x1,
    CBC_MODE,
    CFB_MODE,
    OFB_MODE
} BLOCK_CIPHER_MODE;

typedef struct Param {
    char *key;
    size_t keyLen;
    char *mode;
    size_t modeLen;
    char *inputFilePath;
    size_t inputFilePathLen;
    char *outputFilePath;
    size_t outputFilePathLen;
    int enc;
    int kae;
} Param;

int opensslEncrypt(Param *param, char *in, size_t bufSize, char *out, BLOCK_CIPHER_MODE mode);
int WDEncrypt(Param *param, char *in, size_t bufSize, char *out, BLOCK_CIPHER_MODE mode);

#endif
