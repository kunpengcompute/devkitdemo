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
 * Description: cert_assign_ca.c
 * Create: 2022-08-15
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "securec.h"
#include "tee_client_api.h"
#include "cert_assign_ca.h"

TEEC_Context g_context;
TEEC_Session g_session;

// 16进制字符串转数字
static errno_t HexStrToLong(const char* src, int count, long int* res)
{
    char dest[9];
    char* endptr;
    errno_t err;
    err = strncpy_s(dest, 9, src, count);
    if (err != EOK) {
        return err;
    }
    *res = strtol(dest, &endptr, 16);
    return EOK;
}

// 将字符串的UUID转成TEE的UUID结构体
static errno_t GetUUID(TEEC_UUID* taUUID, char *src)
{
    errno_t err;
    long int res;
    int i;
    err = HexStrToLong(src, 8, &res);
    if (err != EOK) {
        return err;
    }
    taUUID->timeLow = (uint32_t)res;

    err = HexStrToLong(src + 9, 4, &res);
    if (err != EOK) {
        return err;
    }
    taUUID->timeMid = (uint16_t)res;

    err = HexStrToLong(src + 14, 4, &res);
    if (err != EOK) {
        return err;
    }
    taUUID->timeHiAndVersion = (uint16_t)res;
    for (i = 0; i < 2; i++) {
        err = HexStrToLong(src + 19 + i * 2, 2, &res);
        if (err != EOK) {
            return err;
        }
        taUUID->clockSeqAndNode[i] = (uint8_t)res;
    }

    for (i = 0; i < 6; i++) {
        err = HexStrToLong(src + 24 + i * 2, 2, &res);
        if (err != EOK) {
            return err;
        }
        taUUID->clockSeqAndNode[i + 2] = (uint8_t)res;
    }
    return EOK;
}

// 判断字符是否在字符串中
static bool CharInStr(const char targetChar, const char* baseStr)
{
	int i;
	for (i = 0; i < strlen(baseStr); i++) {
		if (baseStr[i] == targetChar) {
			return true;
		}
	}
	return false;
}

// 验证字符串是否合法
static bool ValidateString(const char* str, const char* baseStr)
{
	int i;
	for (i = 0; i < strlen(str); i++) {
		if (!CharInStr(str[i], baseStr)) {
			return false;
		}
	}
	return true;
}

// 初始化，验证用户名密码
static TEEC_Result TeecInit(char *username, char *password)
{
    TEEC_Operation operation;
    TEEC_Result result;
    int ret;
    uint32_t origin;

    result = TEEC_InitializeContext(NULL, &g_context);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("TEEC initial failed.");
        return result;
    }

    /* pass TA's FULLPATH to TEE, then OpenSession. */
    /* CA MUST use the TEEC_LOGIN_IDENTIFY mode to login. */
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_NONE,
        TEEC_NONE);
    operation.params[PARAMS_IDX0].tmpref.buffer = username;
    operation.params[PARAMS_IDX0].tmpref.size = strlen(username);
    operation.params[PARAMS_IDX1].tmpref.buffer = password;
    operation.params[PARAMS_IDX1].tmpref.size = strlen(password);

    char taPath[PATH_LEN] = {0};
    ret = sprintf(taPath, "%s%s%s", TA_DIR_NAME, TA_UUID, TA_SUFFIX);
    if (ret <= 0) {
        printf("Failed to get TA path.\n");
        TEEC_FinalizeContext(&g_context);
        return TEEC_ERROR_GENERIC;
    }
    g_context.ta_path = (uint8_t*)taPath;

    /* CERT ASSIGN uuid */
    TEEC_UUID taUUID;
    if (GetUUID(&taUUID, TA_UUID) != EOK) {
        printf("Failed to get UUID.\n");
        TEEC_FinalizeContext(&g_context);
        return TEEC_ERROR_GENERIC;
    }

    // 创建TEE会话，验证用户名密码
    result = TEEC_OpenSession(&g_context, &g_session, &taUUID, TEEC_LOGIN_IDENTIFY, NULL, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        printf("Open session failed: result: %d, orgin: %d.\n", result, origin);
        TEEC_FinalizeContext(&g_context);
        return result;
    }
    TEEC_Debug("TEEC initialize context and open session success, session id: 0x%x, service id: 0x%x, context: 0x%x.",
        g_session.session_id, g_session.service_id, g_session.context);
    TEEC_Debug("teex init OK.");
    return result;
}

// 关闭会话，销毁context
static void TeecClose(void)
{
    TEEC_CloseSession(&g_session);
    TEEC_FinalizeContext(&g_context);

    TEEC_Debug("TEEC uninit OK.");
}

// 获取文件大小
int GetFileSize(char *filePath, size_t *fileSize)
{
    struct stat statbuf;
    int ret = stat(filePath, &statbuf);
    if (ret != 0) {
        return ret;
    }
    *fileSize = statbuf.st_size;
    return 0;
}

// 读取文件
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

// 写入文件
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

// 调用TA查询根证书是否已创建
static int GetRootCertState() 
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin = 0;
    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE
    );
    result = TEEC_InvokeCommand(&g_session, CMD_GET_ROOT_CERT_STATE, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        printf("Invoke CMD_GET_ROOT_CERT_STATE failed, codes=0x%x, origin=0x%x.\n", result, origin);
    }
    return result;
}

// 调用TA创建根证书
static TEEC_Result CreateRootCert(char *commonName, char *cipher, char *certContent, size_t *certContentLen) 
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin = 0;
    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_NONE
    );
    operation.params[PARAMS_IDX0].tmpref.buffer = commonName;
    operation.params[PARAMS_IDX0].tmpref.size = strlen(commonName);
    operation.params[PARAMS_IDX1].tmpref.buffer = cipher;
    operation.params[PARAMS_IDX1].tmpref.size = strlen(cipher);
    operation.params[PARAMS_IDX2].tmpref.buffer = certContent;
    operation.params[PARAMS_IDX2].tmpref.size = *certContentLen;
    result = TEEC_InvokeCommand(&g_session, CMD_CREAT_ROOT_CERT, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        printf("Invoke CMD_CREAT_ROOT_CERT failed, codes=0x%x, origin=0x%x.\n", result, origin);
    } else {
        *certContentLen = operation.params[PARAMS_IDX2].tmpref.size;
    }
    return result;
}

// 调用TA签发X509证书
static int X509SignCert(char *reqPath, char *certPath) 
{
    size_t certBufferLen = MAX_BUFFER_LEN;
    char certBuffer[MAX_BUFFER_LEN + 1] = {0};
    size_t reqBufferLen = MAX_BUFFER_LEN;
    char reqBuffer[MAX_BUFFER_LEN + 1] = {0};
    size_t fileSize = 0;
    GetFileSize(reqPath, &fileSize);
    if (fileSize > MAX_BUFFER_LEN) {
        printf("The certificate signing request file size is too large.\n");
        return 1;
    }
    if (ReadAll(reqPath, reqBuffer, fileSize) != 0) {
        printf("Failed to read the certificate signing request file.\n");
        return 1;
    }
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin = 0;
    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_NONE,
        TEEC_NONE
    );
    operation.params[PARAMS_IDX0].tmpref.buffer = reqBuffer;
    operation.params[PARAMS_IDX0].tmpref.size = fileSize;
    operation.params[PARAMS_IDX1].tmpref.buffer = certBuffer;
    operation.params[PARAMS_IDX1].tmpref.size = certBufferLen;
    result = TEEC_InvokeCommand(&g_session, CMD_SIGN_X509_CERT, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        printf("Invoke CMD_SIGN_X509_CERT failed, codes=0x%x, origin=0x%x.\n", result, origin);
        return 1;
    }
    certBufferLen = operation.params[PARAMS_IDX1].tmpref.size;
    if (WriteAll(certPath, certBuffer, certBufferLen) != 0) {
        printf("Failed to write certificate.\n");
        return 1;
    }
    printf("The certificate has been saved in %s.\n", certPath);
    return result;
}

// 读取用户输入
static int ReadLine(char *buffer, size_t bufferLen, const char *prompt) {
    printf("%s", prompt);
    if (!fgets(buffer, bufferLen + 1, stdin)) {
        printf("Failed to read stdin.\n");
        return 1;
    }
    char end = buffer[bufferLen - 1];
    if (end != '\0' && end != '\n') {
        printf("Input too long.\n");
        return 1;
    }
    // 去除换行符
    buffer[strlen(buffer) - 1] = '\0';
    return 0;
}

// 调用TA查询根证书信息
static int ShowRootCertificate()
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin = 0;
    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE
    );
    size_t bufferLen = MAX_BUFFER_LEN;
    char buffer[MAX_BUFFER_LEN + 1] = {0};
    operation.params[PARAMS_IDX0].tmpref.buffer = buffer;
    operation.params[PARAMS_IDX0].tmpref.size = bufferLen;
    result = TEEC_InvokeCommand(&g_session, CMD_SHOW_ROOT_CERT, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        printf("Invoke CMD_SHOW_ROOT_CERT failed, codes=0x%x, origin=0x%x.\n", result, origin);
    } else {
        printf("root certificate info:\n%s\n", buffer);
    }
    return result;
}

static void PrintHelp() {
    printf("cert-assign>\n");
    printf("\tshow             Show the root certificate info.\n");
    printf("\tsign             Sign a certificate signing request.\n");
    printf("\thelp             Show this help message\n");
    printf("\texit             Exit this program\n");
}

// 开始用户交互，根据用户输入执行相应的功能
static int Interactive(char *username)
{
    int ret;
    ret = GetRootCertState();
    if (ret != 0) {
        // 根证书不存在，创建根证书
        char commonName[INPUT_LEN + 1] = {0};
        char cipher[INPUT_LEN + 1] = {0};
        printf("The root certificate does not exist.\n");
        printf("Create root certificate:\n");
        if (ReadLine(commonName, INPUT_LEN, "Please input common name: ") != 0) {
            return 1;
        }
        if (ReadLine(cipher, INPUT_LEN, "Please input cipher(RSA or SM2): ") != 0) {
            return 1;
        }
        if (strcmp(cipher, "SM2") != 0 && strcmp(cipher, "RSA") != 0) {
            printf("Invalid cipher.\n");
            return 1;
        }
        size_t certBufferLen = MAX_BUFFER_LEN;
        char certBuffer[MAX_BUFFER_LEN + 1] = {0};
        if (CreateRootCert(commonName, cipher, certBuffer, &certBufferLen) != TEEC_SUCCESS) {
            return 1;
        }
        if (WriteAll("root_cert.pem", certBuffer, certBufferLen) != 0) {
            printf("Failed to write root certificate.\n");
            return 1;
        }
        printf("The root certificate has been saved in root_cert.pem.\n");
    }
    char inputBuffer[INPUT_LEN + 1] = {0};
    char reqPath[PATH_LEN + 1] = {0};
    char certPath[PATH_LEN + 1] = {0};
    PrintHelp();
    while (1) {
        memset(inputBuffer, 0, INPUT_LEN + 1);
        if (ReadLine(inputBuffer, INPUT_LEN, "cert-assign>") != 0) {
            continue;
        }
        if (strcmp(inputBuffer, "show") == 0) {
            // 查询根证书信息
            ShowRootCertificate();
        } else if (strcmp(inputBuffer, "sign") == 0) {
            // 签发X509证书
            if (ReadLine(reqPath, PATH_LEN, "Please input certificate signing request file path: ") != 0) {
                continue;
            }
            if (ReadLine(certPath, PATH_LEN, "Please input X509 certificate storage path: ") != 0) {
                continue;
            }
            X509SignCert(reqPath, certPath);
            memset(reqPath, 0, PATH_LEN + 1);
            memset(certPath, 0, PATH_LEN + 1);
        } else if (strcmp(inputBuffer, "exit") == 0) {
            break;
        } else {
            PrintHelp();
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    TEEC_Result ret;
    char username[USERNAME_LEN + 1];
    printf("Welcome to itrestee cert assign demo.\n");
    if (ReadLine(username, USERNAME_LEN, "Please input username: ") != 0) {
        return 1;
    }
    if (!ValidateString(username, BASE_STR)) {
        printf("Incorrect username.\n");
        return 1;
    }
    char *password = getpass("Please input password: ");
    ret = TeecInit(username, password);
    if (ret == TEEC_ERROR_ACCESS_DENIED) {
        printf("Incorrect username or password.\n");
        return 1;
    }
    if (TEEC_SUCCESS != ret) {
        printf("TeecInit Failed!\n");
        return 1;
    }
    Interactive(username);
    TeecClose();
    return 0;
}
