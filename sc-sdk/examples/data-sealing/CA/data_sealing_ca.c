/*
 * Copyright 2020 Huawei Technologies Co., Ltd
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
 * Description: data-sealing-ca.c
 * Create: 2022-04-15
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <limits.h>
#include "data_sealing_ca.h"
#include "tee_client_api.h"
#include "securec.h"

enum DataFlagConstants
{
    TEE_DATA_FLAG_ACCESS_READ = 0x00000001,
    TEE_DATA_FLAG_ACCESS_WRITE = 0x00000002,
    TEE_DATA_FLAG_CREATE = 0x00000200,
    TEE_DATA_FLAG_EXCLUSIVE = 0x00000400,
};

enum OperationType
{
    ENCRYPT = CHAR_MAX + 1,
    DECRYPT
};

typedef struct
{
    enum OperationType operationType;
    char *key;
    char *filePath;
    char *output;
} ParameterValueT;

static struct option const longOptions[] = 
{
    {"encrypt",no_argument,NULL,ENCRYPT},
    {"decrypt",no_argument,NULL,DECRYPT},
    {"key",required_argument,NULL,'k'},
    {"file-path",required_argument,NULL,'f'},
    {"output",required_argument,NULL,'o'},
    {"help",no_argument,NULL,'h'},
    {NULL, 0, NULL, 0}
};

TEEC_Context g_context;
TEEC_Session g_session;

static errno_t HexStrToLong(const char *src, int count, long int *res)
{
    char dest[9];
    char *endptr;
    errno_t err;
    err = strncpy_s(dest, 9, src, count);
    if (err != EOK) {
        return err;
    }
    *res = strtol(dest, &endptr, 16);
    return EOK;
}

static errno_t GetDataSealingUUID(TEEC_UUID *dataSealingUUID)
{
    errno_t err;
    long int res;
    int i;
    char *src = DATA_SEALING_TA_UUID;
    err = HexStrToLong(src, 8, &res);
    if (err != EOK)
    {
        return err;
    }
    dataSealingUUID->timeLow = (uint32_t) res;
    err = HexStrToLong(src + 9, 4, &res);
    if (err != EOK)
    {
        return err;
    }
    dataSealingUUID->timeMid = (uint16_t) res;
    err = HexStrToLong(src + 14, 4, &res);
    if (err != EOK)
    {
        return err;
    }
    dataSealingUUID->timeHiAndVersion = (uint16_t) res;
    for (i = 0; i < 2; i++)
    {
        err = HexStrToLong(src + 19 + i * 2, 2, &res);
        if (err != EOK)
        {
        return err;
        }
        dataSealingUUID->clockSeqAndNode[i] = (uint8_t) res;
    }
    for (i = 0; i < 6; i++)
    {
        err = HexStrToLong(src + 24 + i * 2, 2, &res);
        if (err != EOK)
        {
        return err;
        }
        dataSealingUUID->clockSeqAndNode[i + 2] = (uint8_t) res;
    }
    return EOK;
}

/* initialize context and opensession */
static TEEC_Result TeecInit(void)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin = 0;
    result = TEEC_InitializeContext(NULL, &g_context);
    if (result != TEEC_SUCCESS)
    {
        TEEC_Error("teec initial failed.");
        printf("teec initial failed.");
        return result;
    }
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE,
                                            TEEC_NONE,
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_MEMREF_TEMP_INPUT);
    char dataSealingTAPath[PATH_MAX];
    int ret = sprintf(dataSealingTAPath, "%s%s%s", DATA_SEALING_TA_DIR_NAME, DATA_SEALING_TA_UUID, DATA_SEALING_TA_SUFFIX);
    if (ret <= 0)
    {
        printf("Failed to get TA path.\n");
        TEEC_FinalizeContext(&g_context);
        return TEEC_ERROR_GENERIC;
    }
    g_context.ta_path = (uint8_t *) dataSealingTAPath;
    TEEC_UUID dataSealingUUID;
    if (GetDataSealingUUID(&dataSealingUUID) != EOK)
    {
        printf("Failed to get uuid.\n");
        TEEC_FinalizeContext(&g_context);
        return TEEC_ERROR_GENERIC;
    }
    g_context.ta_path = (uint8_t *) dataSealingTAPath;
    result = TEEC_OpenSession(&g_context, &g_session, &dataSealingUUID, TEEC_LOGIN_IDENTIFY, NULL, &operation, &origin);
    if (result != TEEC_SUCCESS)
    {
        printf("open session failed: result:%d, orgin: %d.\n", result, origin);
        TEEC_FinalizeContext(&g_context);
        return result;
    }
    TEEC_Debug("teec initialize context andopen session success, session id: 0x%x, service id: 0x%x, context 0x%x.",
                g_session.session_id, g_session.service_id, g_session.context);
    TEEC_Debug("teec init OK.");
    return result;
}

static void TeecClose(void)
{
    TEEC_CloseSession(&g_session);
    TEEC_FinalizeContext(&g_context);
    TEEC_Debug("teec uninit OK.");
}

static TEEC_Result DataSealingReadFile(IN const char *path, IN const uint32_t mode,
                                       OUT char *readBuff, OUT size_t *readSize)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin = 0;
    printf("DATA_SEALING READ FILE is running.\n");
    if (path == NULL)
    {
        TEEC_Error("DATA_SEALING READ FILE failed, path is null.\n");
        return TEEC_ERROR_BAD_STATE;
    }
    /* First, Check Init is ok or not */
    if ((g_context.fd == 0) || (g_session.session_id == 0))
    {
        TEEC_Error("DATA_SEALING READ FILE failed, Please Init Storage Service.\n");
        return TEEC_ERROR_BAD_STATE;
    }
    /* Then, Invoke Command */
    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INOUT,
                                            TEEC_VALUE_OUTPUT);
    operation.params[PARAMS_IDX0].tmpref.buffer = (void *)path;
    operation.params[PARAMS_IDX0].tmpref.size = strlen(path);
    operation.params[PARAMS_IDX1].value.a = mode;
    operation.params[PARAMS_IDX1].value.b = TEEC_VALUE_UNDEF;
    operation.params[PARAMS_IDX2].tmpref.buffer = (void *) readBuff;
    operation.params[PARAMS_IDX2].tmpref.size = (uint32_t) (*readSize);
    operation.params[PARAMS_IDX3].value.a = SECURE_STORAGE_INVALID_FD;
    operation.params[PARAMS_IDX3].value.b = TEEC_VALUE_UNDEF;
    result = TEEC_InvokeCommand(&g_session, CMD_DATA_SEALING_READ_FILE, &operation, &origin);
    if (result != TEEC_SUCCESS) 
    {
        TEEC_Error("ReadFile Failed:0x%x, origin:0x%x.\n", result, origin);
        return result;
    }
    *readSize = operation.params[PARAMS_IDX2].tmpref.size;
    printf("DataSealing read file success: fd=%d, read count=%zu.\n", operation.params[PARAMS_IDX3].value.a, *readSize);
    return TEEC_SUCCESS;
}

static TEEC_Result DataSealingWriteFile(IN const char *path, IN const uint32_t mode,
                                        IN const char *writeBuff, IN const size_t writeSize)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin = 0;
    printf("DATA_SEALING WRITE FILE is running.\n");
    if (path == NULL)
    {
        TEEC_Error("DATA_SEALING WRITE FILE failed, path is null.\n");
        return TEEC_ERROR_BAD_STATE;
    }
    if ((mode & TEE_DATA_FLAG_CREATE)  && (mode & TEE_DATA_FLAG_EXCLUSIVE))
    {
        TEEC_Error("DATA_SEALING WRITE FILE failed, mode error, CREATE&EXCLUSIVE canot be concurrence.\n");
        return TEEC_ERROR_BAD_STATE;
    }
    /* First, Check Init is ok or not */
    if ((g_context.fd == 0) || (g_session.session_id == 0)) 
    {
        TEEC_Error("DATA_SEALING WRITE FILE failed, Please Init Storage Service.\n");
        return  TEEC_ERROR_BAD_STATE;
    }

    /* Then, Invoke Command */
    operation.started = 1;
    operation.cancel_flag = 0;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_VALUE_INPUT,
                                            TEEC_MEMREF_TEMP_INPUT,
                                            TEEC_VALUE_OUTPUT);
    operation.params[PARAMS_IDX0].tmpref.buffer = (void *)path;
    operation.params[PARAMS_IDX0].tmpref.size = strlen(path);
    operation.params[PARAMS_IDX1].value.a = mode;
    operation.params[PARAMS_IDX1].value.b = TEEC_VALUE_UNDEF;
    operation.params[PARAMS_IDX2].tmpref.buffer = (void *) writeBuff;
    operation.params[PARAMS_IDX2].tmpref.size = (uint32_t) writeSize;
    operation.params[PARAMS_IDX3].value.a = SECURE_STORAGE_INVALID_FD;
    operation.params[PARAMS_IDX3].value.b = TEEC_VALUE_UNDEF;
    result = TEEC_InvokeCommand(&g_session, CMD_DATA_SEALING_WRITE_FILE, &operation, &origin);
    if (result != TEEC_SUCCESS) 
    {
        TEEC_Error("WriteFile Failed:0x%x, origin:0x%x.\n", result, origin);
        return result;
    }
    printf("DataSealing write file success: fd=%d.\n", operation.params[PARAMS_IDX3].value.a);
    return TEEC_SUCCESS;
}

static int ReadFile (const char *filePath, char *fileContent)
{
    FILE *pFile = fopen (filePath, "r");
    if (pFile == NULL)
    {
        return 0;
    }
    int readSize = fread (fileContent, 1, FILE_SIZE_MAX + 1, pFile);
    if (readSize > FILE_SIZE_MAX || readSize == 0)
    {
        printf ("The maximum file size is 4096B.\n");
        fclose (pFile);
        return 0;
    }
    fclose(pFile);
    return readSize;
}

static int WriteFile (const char *filePath, const char *fileContent, size_t fileContentLen)
{
    FILE *pFile = fopen (filePath, "w");
    if (pFile == NULL)
    {
        return 0;
    }
    size_t ret = fwrite (fileContent, fileContentLen, 1, pFile);
    fclose (pFile);
    return ret;
}

static bool CharInStr (const char targetChar, const char *baseStr)
{
    int i;
    for (i = 0; i < strlen(baseStr); i++)
    {
        if (baseStr[i] == targetChar)
        {
            return true;
        }
    }
    return false;
}

static bool ValidateString (const char *str, const char *baseStr)
{
    int i;
    for (i = 0; i < strlen (str); i++)
    {
       if (!CharInStr (str[i], baseStr))
       {
           return false;
       }
    }
    return true;
}

static void Usage (int status)
{
    if (status != EXIT_SUCCESS)
    {
        printf("Try '/vendor/bin/data-sealing --help' for more information.\n");
    }
    else
    {
        printf("Usage: /vendor/bin/data-sealing [TYPE OPTION] [REQUIRED_ARGUMENT]\n"
                "TYPE OPTION:\n"
                "\t--encrypt\t\tEncrypt data.\n"
                "\t--decrypt\t\tDecrypt data.\n"
                "REQUIRED_ARGUMENT:\n"
                "\t-k, --key\t\tEncrypt data and decrypt data ID [0 < KEY_LENGTH <= 128, letters and numbers only].\n"
                "\t-f, --file-path\t\tEncrypted data file path [0 < FILE_SIZE <= 4096B, strong keys are recommended].\n"
                "\t-o, --oUtput\t\tDecrypted data output path.\n"
                "\t-h, --help\t\tDisplay this help and exit.\n");
    }
    exit (status);
}

static int CheckKey (const char *key)
{
    return (key != NULL && strlen (key) <= KEY_LENGTH_MAX && ValidateString (key, BASE_STR));
}

static void Encrypt (const char *key, const char *filePath)
{
    if (!CheckKey (key))
    {
        printf ("Check if the key is legal!\n");
        Usage (EXIT_FAILURE);
    }
    TEEC_Result result;
    char fileContent[FILE_SIZE_MAX + 1];
    int fileSize = ReadFile (filePath, fileContent);
    if (fileSize > 0)
    {
        result = DataSealingWriteFile (key, (uint32_t) DATA_SEALING_MODE_WRITE, fileContent, fileSize);
        if (result != TEEC_SUCCESS)
        {
            printf ("Encrypt file Failed!\n");
            exit (EXIT_FAILURE);
        }
        else
        {
            exit (EXIT_SUCCESS);
        }
    }
    else
    {
        printf ("Check if the file is legal!\n");
        Usage (EXIT_FAILURE);
    }
}

static void Decrypt (const char *key, const char *outputFilePath)
{
    if (!CheckKey (key))
    {
        printf ("Check if the key is legal!\n");
        Usage (EXIT_FAILURE);
    }
    TEEC_Result result;
    char fileContent[FILE_SIZE_MAX];
    size_t readSize = sizeof (fileContent);
    result = DataSealingReadFile (key,  TEE_DATA_FLAG_ACCESS_READ, fileContent, &readSize);
     if (result != TEEC_SUCCESS)
    {
        printf ("Decrypt file Failed!\n");
        exit (EXIT_FAILURE);
    }
    else
    {
        if (!WriteFile (outputFilePath, fileContent, readSize))
        {
            exit (EXIT_FAILURE);
        }
        exit (EXIT_SUCCESS);
    }
}

static int PerformAction (ParameterValueT parameterValue)
{
    TEEC_Result result = TeecInit ();
    if (result != TEEC_SUCCESS)
    {
        printf ("TeecInit Failed!\n");
        return 1;
    }
    switch (parameterValue.operationType)
    {
    case ENCRYPT:Encrypt (parameterValue.key, parameterValue.filePath);
    break;
    case DECRYPT:Decrypt (parameterValue.key, parameterValue.output);
    break;
    }
    TeecClose ();
    return 0;
}

int main (int argc, char **argv)
{
    ParameterValueT parameterValue;
    while (true)
    {
        int oi = -1;
        int opt = getopt_long (argc, argv, "k:f:o:h", longOptions, &oi);
        if (opt == -1)
        {
            break;
        }
        switch (opt)
        {
        case ENCRYPT:parameterValue.operationType = ENCRYPT;
        break;
        case DECRYPT:parameterValue.operationType = DECRYPT;
        break;
        case 'k':parameterValue.key = optarg;
        break;
        case 'f':parameterValue.filePath = optarg;
        break;
        case 'o':parameterValue.output = optarg;
        break;
        case 'h':Usage (EXIT_SUCCESS);
        default:Usage (EXIT_FAILURE);
        }
    }
    return PerformAction (parameterValue);
}
