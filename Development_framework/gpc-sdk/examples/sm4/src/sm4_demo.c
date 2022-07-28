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
#include <ctype.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "option.h"
#include "sm4.h"

int OpenSSLEncrypt(Param *param, char *in, size_t bufSize, char *out, BLOCK_CIPHER_MODE mode);
int KAEDriverEncrypt(Param *param, char *in, size_t bufSize, char *out, BLOCK_CIPHER_MODE mode);

BLOCK_CIPHER_MODE g_cipher_mode;

void PrintUsage(char *name)
{
    printf("Usage: %s [encrypt | decrypt] [options]\n", name);
    printf("Options:\n");
    printf("-k, --key <key>        raw key, in hex\n");
    printf("-m, --mode <mode>      block cipher mode\n");
    printf("-i, --input <path>     input file path\n");
    printf("-o, --output <path>    output file path\n");
    printf("-K, --KAE              use KAEdriver\n");
    printf("-h, --help             Print Help (this message) and exit\n");
}


int hexchar2int(unsigned char c)
{
    switch (c) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'a': case 'A':
            return 0x0A;
        case 'b': case 'B':
            return 0x0B;
        case 'c': case 'C':
            return 0x0C;
        case 'd': case 'D':
            return 0x0D;
        case 'e': case 'E':
            return 0x0E;
        case 'f': case 'F':
            return 0x0F;
    }
    return -1;
}

// 转换16进制的key
int SetKey(Param *param, char *optarg, size_t argLen)
{
    if (argLen > 32) {
        printf("key too long.\n");
        return 1;
    }
    param->keyLen = argLen / 2;
    int i = 0;
    int j = 0;
    char *in = optarg;
    for (i = 0; i < argLen; i++) {
        j = (unsigned char)*in++;
        if (!isxdigit(j)) {
            printf("Invalid key: non-hex digit.\n");
            return 1;
        }
        j = (unsigned char)hexchar2int(j);
        if (i & 1)
            param->key[i / 2] |= j;
        else
            param->key[i / 2] = (j << 4);
    }
    return 0;
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
    size_t argLen = 0;
    while (1) {
        option = getopt_long(argc, argv, "k:m:i:o:Kh", long_options, &option_index);
        if (option == -1) {
            break;
        }
        switch (option) {
            case 'k':
                argLen = strlen(optarg);
                if (SetKey(param, optarg, argLen) != 0) {
                    return 1;
                }
                break;
            case 'm':
                param->mode = optarg;
                param->modeLen = strlen(param->mode);
                if (strcmp(param->mode, "ecb") == 0) {
                    g_cipher_mode  = ECB_MODE;
                } else if (strcmp(param->mode, "cbc") == 0) {
                    g_cipher_mode  = CBC_MODE;
                } else if (strcmp(param->mode, "cfb") == 0) {
                    g_cipher_mode  = CFB_MODE;
                } else if (strcmp(param->mode, "ofb") == 0) {
                    g_cipher_mode  = OFB_MODE;
                } else {
                    printf("Invalid mode. Supports only ecb, cbc, cfb and ofb mode.\n");
                    return 1;
                }
                break;
            case 'i':
                param->inputFilePath = optarg;
                param->inputFilePathLen = strlen(param->inputFilePath);
                if (param->inputFilePathLen == 0 || param->inputFilePathLen > 255) {
                    printf("input file path too long.\n");
                    return 1;
                }
                break;
            case 'o':
                param->outputFilePath = optarg;
                param->outputFilePathLen = strlen(param->outputFilePath);
                if (param->outputFilePathLen == 0 || param->outputFilePathLen > 255) {
                    printf("input file path too long.\n");
                    return 1;
                }
                break;
            case 'K':
                param->kae = 1;
                break;
            case 'h':
                PrintUsage(argv[0]);
                exit(0);
                break;
            default:
                break;
        }
    }
    return 0;
}

int ValidateParam(Param *param)
{
    if (param->keyLen != 16) {
        printf("SM4 supports only 128-bit key.\n");
        return 1;
    }
    if (param->modeLen != 3) {
        printf("Invalid mode. Supports only ecb, cbc, cfb and ofb mode.\n");
        return 1;
    }
    if (param->inputFilePathLen == 0 || param->inputFilePathLen > 255) {
        printf("input file path is required or too long.\n");
        return 1;
    }
    if (param->outputFilePathLen == 0 || param->outputFilePathLen > 255) {
        printf("output file path is required or too long.\n");
        return 1;
    }
    return 0;
}

int GetFileSize(char *filePath, int *fileSize)
{
    struct stat statbuf;
    int ret = stat(filePath, &statbuf);
    if (ret != 0) {
        return ret;
    }
    *fileSize = statbuf.st_size;
    return 0;
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
    int ret = 1;
    int padding = 0;
    char paddingSize = 0;
    int i;
    Param param;
    memset(&param, 0 , sizeof(Param));

    ret = GetOptions(argc, argv, &param);
    if (ret != 0) {
        PrintUsage(argv[0]);
        return ret;
    }
    ret = ValidateParam(&param);
    if (ret != 0) {
        PrintUsage(argv[0]);
        return ret;
    }
    int inputFileSize = 0;
    ret = GetFileSize(param.inputFilePath, &inputFileSize);
    if (ret != 0) {
        printf("Failed to read input file.\n");
        return ret;
    }
    printf("inputFileSize: %d\n", inputFileSize);

    int bufSize;
    bufSize = inputFileSize;
    if (strcmp(param.mode, "ecb") == 0 || strcmp(param.mode, "cbc") == 0) {
        padding = 1;
    }

    // 加密时计算PKCS7填充大小
    if (param.enc && padding == 1) {
        bufSize = (inputFileSize / 16 + 1) * 16;
    }
    printf("bufSize: %d\n", bufSize);
    char *in = calloc(1, bufSize);
    if (in == NULL) {
        printf("Failed to calloc memory.\n");
        return ret;
    }
    char *out = calloc(1, bufSize);
    if (out == NULL) {
        printf("Failed to calloc memory.\n");
        free(in);
        return ret;
    }
    if (ReadAll(param.inputFilePath, in, inputFileSize) != 0) {
        printf("Failed to read input file.\n");
        goto finish;
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // 加密时使用PKCS7填充
    if (param.enc && padding == 1) {
        paddingSize = 16 - (inputFileSize % 16);
        i = 0;
        for (i = inputFileSize; i < bufSize; i++) {
            in[i] = paddingSize;
        }
    } else {
        if (inputFileSize & 16 != 0) {
            printf("Invalid input data.\n");
            goto finish;
        }
    }

    char *operation = param.enc ? "encrypt" : "decrypt";
    if (param.kae) {
        if (g_cipher_mode != ECB_MODE && g_cipher_mode != CBC_MODE) {
            printf("KAEdriver supports only ECB and CBC mode.\n");
            goto finish;
        }
        printf("KAEdriver SM4-%s %s\n", param.mode, operation);
        // 调用KAE driver加解密
        if (KAEDriverEncrypt(&param, in, bufSize, out, g_cipher_mode) != 0) {
            goto finish;
        }
    } else {
        printf("OpenSSL SM4-%s %s\n", param.mode, operation);
        // 调用OpenSSL加解密
        OpenSSLEncrypt(&param, in, bufSize, out, g_cipher_mode);
    }

    // 解密时去除PKCS7填充
    if (!param.enc && padding == 1) {
        paddingSize = out[bufSize - 1];
        for (i = 0; i < paddingSize; i++) {
            if (out[bufSize - 1 - i] != paddingSize) {
                printf("Failed to decrypt.\n");
                goto finish;
            }
        }
    } else {
        paddingSize = 0;
    }
    gettimeofday(&end, NULL);
    long timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("cost time: %f s\n", timeuse / 1000000.0);
    ret = WriteAll(param.outputFilePath, out, bufSize - paddingSize);
    if (ret != 0) {
        printf("Failed to write output file.\n");
    }

finish:
    free(in);
    free(out);
    return ret;
}
