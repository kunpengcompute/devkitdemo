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
 * Description: snappy demo
 * Create: 2022-07-24
 */
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>    
#include <stdio.h> 
#include <string.h>
#include "snappy/snappy.h"

enum tpye {
    compress = 1,
    decompress = 2,
};

namespace {

size_t GetFileSize(const char *fileName) {
    struct stat statBuf;
    size_t fileSize;
    if (stat(fileName, &statBuf) != 0) {
        perror(fileName);
        exit(1);
    }
    fileSize = statBuf.st_size;
    return static_cast<size_t>(fileSize);
}

void *MallocAndLoadFile(const char *fileName, size_t *bufferSize) {
    *bufferSize = GetFileSize(fileName);
    void *const buffer = malloc(*bufferSize);
    FILE *const inFile = fopen(fileName, "rb");
    size_t const readSize = fread(buffer, 1, *bufferSize, inFile);
    fclose(inFile);
    return buffer;
}

size_t Compress(const char *inFileName) {
    std::string outFileName = std::string(inFileName) + ".snappy";
    size_t fSize;
    void *const fBuff = MallocAndLoadFile(inFileName, &fSize);
    std::string compressed;
    size_t cSize = snappy::Compress((const char *)fBuff, (size_t)fSize, &compressed);
    FILE *const outFile = fopen(outFileName.data(), "wb");
    fputs(compressed.data(), outFile);
    fclose(outFile);
    if (snappy::IsValidCompressedBuffer(compressed.data(), compressed.size())) {
        std::cout << "success" << std::endl;
    } else {
        std::cout << "failed" << std::endl;
    }
    return cSize;
}

std::string Decompress(const char *inFileName) {
    size_t fSize;
    void *const fBuff = MallocAndLoadFile(inFileName, &fSize);
    const char *fContent = (char *)fBuff;
    size_t fContentSize = strlen(fContent);
    std::string uncompressed;
    snappy::Uncompress((const char *)fBuff, (size_t)fSize, &uncompressed);
    return uncompressed;
}

}
int main(int argc, const char **argv) {
    if (argc < 2) {
        std::cout << "wrong arguments" << std::endl
             << "usage:" << std::endl
             << argv[0] << std::endl;
        return 1;
    }
    int flag;
    std::cin >> flag;
    switch (flag) {
    case compress:
        Compress(argv[1]);
        break;
    case decompress:
        std::cout << Decompress(argv[1]) << std::endl;
        break;
    default:
        break;
    }
    return 0;
}
