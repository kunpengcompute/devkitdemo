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
 * Description: main.c
 * Create: 2022-07-25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "aes.h"

BLOCK_CIPHER_MODE g_cipher_mode;

void PrintUsage(char *name)
{
    printf("Usage: %s [encrypt |decrypt] [options]\n", name);
    printf("Options:\n");
    printf("-k, --key <key>       key\n");
    printf("-m, --mode <mode>     mode\n");
    printf("-i, --input <path>    input file path\n");
    printf("-o, --output <path>   output file path\n");
    printf("-K, --KAE             use KAEdriver\n");
    printf("-h, --help            Print Help (this message) and exit\n");
}

int GetOptions(int argc, char *argv[], Param *param)
{
    int option;
    int option_index = 0;
    struct option long_options[] = {
        {"key", required_argument, NULL, 'k'},
        {"mode", required_argument, NULL, 'm'},
        {"input", required_argument, NULL, 'i'},
        {"output", required_argument, NULL, 'o'},
        {"KAE", no_argument, NULL, 'K'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };
    if (argc == 1) {
        PrintUsage(argv[0]);
        exit(0);
    }
    if (strcmp(argv[1], "encrypt") == 0) {
        param->enc = 1;
    } else if (strcmp(argv[1], "decrypt") == 0) {
        param->enc = 0;
    } else {
        PrintUsage(argv[0]);
        exit(0);
    }
    while (1) {
        option = getopt_long(argc, argv, "k:m:i:o:Kh", long_options, &option_index);
        if (option == -1) {
            break;
        }
        switch (option) {
            case 'k':
                param->key = optarg;
                param->keyLen = strlen(param->key);
                break;
            case 'm':
                param->mode = optarg;
                param->modeLen = strlen(param->mode);
                break;
            case 'i':
                param->inputFilePath = optarg;
                param->inputFilePathLen = strlen(param->inputFilePath);
                break;
            case 'o':
                param->outputFilePath = optarg;
                param->outputFilePathLen = strlen(param->outputFilePath);
                break;
            case 'K':
                param->kae = 1;
                break;
            case 'h':
                PrintUsage(argv[0]);
                exit(0);
                break;
        }
    }
}

int ValidateParam(Param *param)
{
    if (param->keyLen != 16 && param->keyLen != 24 && param->keyLen != 32) {
        printf("SM4 supports only 128-bit, 192-bit or 256-bit key.\n");
        return 1;
    }
    if (param->modeLen != 3) {
        return 1;
    }
    if (strcmp(param->mode, "ecb") == 0) {
        g_cipher_mode  = ECB_MODE;
    } else if (strcmp(param->mode, "cbc") == 0) {
        g_cipher_mode  = CBC_MODE;
    } else if (strcmp(param->mode, "cfb") == 0) {
        g_cipher_mode  = CFB_MODE;
    } else if (strcmp(param->mode, "ofb") == 0) {
        g_cipher_mode  = OFB_MODE;
    } else {
        return 1;
    }
    if (param->inputFilePathLen == 0 || param->inputFilePathLen > 255) {
        return 1;
    }
    if (param->outputFilePathLen == 0 || param->outputFilePathLen > 255) {
        return 1;
    }
    return 0;
}

int GetFileSize(char *filePath)
{
    struct stat statbuf;
    stat(filePath, & statbuf);
    int size = statbuf.st_size;
    return size;
}

int ReadAll(char *filePath, char *buf, size_t fileSize)
{
    FILE *fp = fopen(filePath, "rb");
    if (fp == NULL) {
        return 1;
    }
    if (fread(buf, 1, fileSize, fp) != fileSize) {
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}

int WriteAll(char *filePath, char *buf, size_t fileSize)
{
    FILE *fp = fopen(filePath, "wb");
    if (fp == NULL) {
        return 1;
    }
    if(fwrite(buf, 1, fileSize, fp) != fileSize) {
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    int padding = 0;
    char paddingSize = 0;
    int i;
    Param param;
    memset(&param, 0 , sizeof(Param));
    GetOptions(argc, argv, &param);
    ret = ValidateParam(&param);
    if (ret != 0) {
        printf("Invalid parameter.\n");
        PrintUsage(argv[0]);
        return ret;
    }
    int inputFileSize = 0;
    inputFileSize = GetFileSize(param.inputFilePath);
    printf("inputFileSize: %d\n", inputFileSize);

    int bufSize;
    bufSize = inputFileSize;
    if (strcmp(param.mode, "ecb") == 0 || strcmp(param.mode, "cbc") == 0) {
        padding = 1;  // 需要填充或去除填充
    }

    // 加密时填充
    if (param.enc && padding ==1) {
        bufSize = (inputFileSize / 16 + 1) * 16;  // 填充后的大小
    }
    printf("bufSize: %d\n", bufSize);
    char *in = calloc(1, bufSize);
    if (in == NULL) {
        printf("Failed to calloc memory.\n");
        return 1;
    }
    char *out = calloc(1, bufSize);
    if (out == NULL) {
        printf("Failed to calloc memory.\n");
        free(in);
        return 1;
    }
    if (ReadAll(param.inputFilePath, in, inputFileSize) != 0) {
        printf("Failed to read input file.\n");
        free(in);
        free(out);
        return 1;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);
    if (param.enc && padding == 1) {
        paddingSize = 16 - (inputFileSize % 16);
        i = 0;
        for (i = inputFileSize; i < bufSize; i++) {
            in[i] = paddingSize;
        }
    } else {
        if (inputFileSize & 16 != 0) {
            printf("Invalid input data.\n");
            free(in);
            free(out);
            return 1;
        }
    }

    if (param.kae) {
        if (g_cipher_mode != ECB_MODE && g_cipher_mode != CBC_MODE) {
            printf("WD supports only ECB and CBC mode.\n");
            free(in);
            free(out);
            return 1;
        }
        printf("KAEdriver AES-%d-%s\n", param.keyLen * 8, param.mode);
        if (WDEncrypt(&param, in, bufSize, out, g_cipher_mode) != 0) {
            free(in);
            free(out);
            return 1;
        }
    } else {
        printf("openssl armv8 ASM AES-%d-%s\n", param.keyLen * 8, param.mode);
        opensslEncrypt(&param, in, bufSize, out, g_cipher_mode);
    }
    free(in);

    // 解密时去除填充
    if (!param.enc && padding == 1) {
        paddingSize = out[bufSize - 1];
        for (i = 0; i < paddingSize; i++) {
            if (out[bufSize -1 - i] != paddingSize) {
                printf("Failed to decrypt.\n");
                free(out);
                return 1;
            }
        }
    } else {
        paddingSize = 0;
    }
    gettimeofday(&end, NULL);
    long timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("cost time: %f s\n", timeuse / 1000000.0);
    WriteAll(param.outputFilePath, out, bufSize - paddingSize);
    free(out);
    return 0;
}