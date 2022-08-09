#ifndef HEAEDER_OPTION_H
#define HEAEDER_OPTION_H

#include <stdlib.h>

typedef enum {
    ECB_MODE = 0x1,
    CBC_MODE,
    CFB_MODE,
    OFB_MODE
} BLOCK_CIPHER_MODE;

typedef struct Param {
    char key[16];
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

#endif
