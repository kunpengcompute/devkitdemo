/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: CA for certification management.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <securec.h>
#include <tee_client_api.h>
#include <tee_client_type.h>
#include <limits.h>

#include "cert_file.h"
#include "cert_common.h"

#define TA_PATH "/usr/bin/4acaf7c8-c652-4643-9b7a-cc07e7a3187a.sec"

/* commands */
#define CMD_DESTROY        "destroy"
#define CMD_IMPORT         "import"
#define CMD_IMPORT_CRL     "import_crl"
#define CMD_EXPORT         "export"
/* sub commands of export */
#define CMD_EXPORT_CERT    "cert"
#define CMD_EXPORT_LOG     "log"

/* number of command arguments */
#define CMD_IMPORT_ARGC         3
#define CMD_EXPORT_CERT_ARGC    4
#define CMD_EXPORT_LOG_ARGC     3
#define CMD_DESTROY_ARGC        2
#define CMD_IMPORT_CRL_ARGC     3

/* index of command parameters */
#define CMD_NAME                1
#define CMD_IMPORT_ARG_PATH     2
#define CMD_EXPORT_SUBCMD       2
#define CMD_EXPORT_ARG_PATH     3

enum {
    IPC_IMPORT_CERT  = 1,
    IPC_EXPORT_CERT  = 2,
    IPC_DESTROY_CERT = 3,
    IPC_EXPORT_LOG   = 4,
    IPC_IMPORT_CRL   = 5
};

enum {
    CERTMANGER_CMD_IMPORT_CERT = 1,
    CERTMANGER_CMD_EXPORT      = 2,
    CERTMANGER_CMD_DESTROY     = 3,
    CERTMANGER_CMD_IMPORT_CRL  = 4
};

static TEEC_Result Destroy(TEEC_Session *session)
{
    TEEC_Result result;
    /* invoke ipc command */
    TEEC_Operation operation = { 0 };
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    result = TEEC_InvokeCommand(session, IPC_DESTROY_CERT, &operation, NULL);
    if (result != TEEC_SUCCESS)
        printf_err("ipc failed\n");
    return result;
}

static TEEC_Result ProcessExportResult(TEEC_SharedMemory *sharedMem, uint32_t len, const char *certPath)
{
    TEEC_Result result = TEEC_SUCCESS;
    uint8_t *outbuf = (uint8_t *)malloc(sharedMem->size);
    if (outbuf == NULL) {
        printf_err("memory allocate failed\n");
        result = TEEC_ERROR_OUT_OF_MEMORY;
        goto end;
    }
    if (len == 0)
        printf_err("warning: empty content\n");
    /* replaced with memcpy(outbuf, sharedMem->buffer, len) when memcpy_s is not supported */
    if (memcpy_s(outbuf, sharedMem->size, sharedMem->buffer, len) != EOK) {
        result = TEEC_ERROR_OUT_OF_MEMORY;
        printf_err("memcpy_s failed\n");
        goto end;
    }
    if (certPath != NULL) {
        if (StoreToFs(outbuf, len, certPath) != 0) {
            result = TEEC_ERROR_WRITE_DATA;
            printf_err("write to file failed: %s\n", certPath);
        }
    } else {
        printf("%s\n", outbuf);
    }
end:
    if (outbuf != NULL) {
        free(outbuf);
        outbuf = NULL;
    }
    return result;
}

static TEEC_Result Export(TEEC_Context *context, TEEC_Session *session, const char *cmdLine, const char *certPath)
{
    TEEC_Result result;
    char realPath[PATH_MAX];
    /* 1. parse sub-command */
    uint32_t cmd;
    if (memcmp(cmdLine, CMD_EXPORT_CERT, sizeof(CMD_EXPORT_CERT)) == 0) {
        /* check Legality of certPath */
        if (realpath(certPath, realPath) == NULL) {
            result = errno;
            printf_err("illegal certification path: %s\n", certPath);
            goto end;
        }
        cmd = IPC_EXPORT_CERT;
    } else if (memcmp(cmdLine, CMD_EXPORT_LOG, sizeof(CMD_EXPORT_LOG)) == 0) {
        cmd = IPC_EXPORT_LOG;
    } else {
        result = TEEC_ERROR_INVALID_CMD;
        printf_err("unknown sub-command: %s\n", cmdLine);
        goto end;
    }
    /* 2. allocate shared memory */
    TEEC_SharedMemory sharedMem;
    sharedMem.size = (cmd == IPC_EXPORT_LOG) ? MAX_LOG_BUFFER_LEN : MAX_BUFFER_LEN;
    sharedMem.flags = TEEC_MEM_OUTPUT | TEEC_MEM_INPUT;
    result = TEEC_AllocateSharedMemory(context, &sharedMem);
    if (result != TEEC_SUCCESS) {
        printf_err("allocate shared memory failed\n");
        goto end;
    }
    /* 3. invoke ipc command */
    TEEC_Operation operation = { 0 };
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    operation.params[0].memref.parent = &sharedMem;
    operation.params[0].memref.offset = 0;
    operation.params[0].memref.size = sharedMem.size;
    result = TEEC_InvokeCommand(session, cmd, &operation, NULL);
    if (result != TEEC_SUCCESS) {
        printf_err("ipc failed\n");
        goto free_sharedMem;
    }
    /* 4. process ipc result */
    const char *path = cmd == IPC_EXPORT_CERT ? realPath : NULL;
    result = ProcessExportResult(&sharedMem, operation.params[0].memref.size, path);
free_sharedMem:
    TEEC_ReleaseSharedMemory(&sharedMem);
end:
    return result;
}

static TEEC_Result Import(TEEC_Context *context, TEEC_Session *session, const char *certPath)
{
    TEEC_Result result;
    char realPath[PATH_MAX];
    /* 1. allocate shared memory */
    TEEC_SharedMemory sharedMem;
    sharedMem.size = MAX_BUFFER_LEN;
    sharedMem.flags = TEEC_MEM_OUTPUT | TEEC_MEM_INPUT;
    result = TEEC_AllocateSharedMemory(context, &sharedMem);
    if (result != TEEC_SUCCESS) {
        printf_err("allocate shared memory failed\n");
        goto end;
    }
    /* 2. check certPath legality */
    if (realpath(certPath, realPath) == NULL) {
        printf_err("illegal certification path:%s\n", certPath);
        result = errno;
        goto free_sharedMem;
    }
    if (!IsFileExist(realPath)) {
        printf_err("certification not exist:%s\n", certPath);
        result = TEEC_ERROR_BAD_PARAMETERS;
        goto free_sharedMem;
    }
    /* 3. read cert from filesystem to shared memory */
    size_t fileSize = 0;
    if (LoadFromFs(sharedMem.buffer, sharedMem.size, certPath, &fileSize) != 0) {
        result = TEEC_ERROR_READ_DATA;
        printf_err("load certification failed\n");
        goto free_sharedMem;
    }
    /* 4. invoke ipc command */
    TEEC_Operation operation = { 0 };
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    operation.params[0].memref.parent = &sharedMem;
    operation.params[0].memref.offset = 0;
    operation.params[0].memref.size = fileSize;
    result = TEEC_InvokeCommand(session, IPC_IMPORT_CERT, &operation, NULL);
    if (result != TEEC_SUCCESS)
        printf_err("ipc failed\n");
free_sharedMem:
    TEEC_ReleaseSharedMemory(&sharedMem);
end:
    return result;
}

static TEEC_Result ImportCrl(TEEC_Context *context, TEEC_Session *session, const char *certPath)
{
    TEEC_Result result;
    char realPath[PATH_MAX];
    /* 1. allocate shared memory */
    TEEC_SharedMemory sharedMem;
    sharedMem.size = MAX_BUFFER_LEN;
    sharedMem.flags = TEEC_MEM_OUTPUT | TEEC_MEM_INPUT;
    result = TEEC_AllocateSharedMemory(context, &sharedMem);
    if (result != TEEC_SUCCESS) {
        printf_err("allocate crl shared memory failed\n");
        goto end;
    }
    /* 2. check certPath legality */
    if (realpath(certPath, realPath) == NULL) {
        printf_err("illegal certification path:%s\n", certPath);
        result = errno;
        goto free_sharedMem;
    }
    if (!IsFileExist(realPath)) {
        printf_err("certification not exist:%s\n", certPath);
        result = TEEC_ERROR_BAD_PARAMETERS;
        goto free_sharedMem;
    }
    /* 3. read cert from filesystem to shared memory */
    size_t fileSize = 0;
    if (LoadFromFs(sharedMem.buffer, sharedMem.size, certPath, &fileSize) != 0) {
        result = TEEC_ERROR_READ_DATA;
        printf_err("load crl failed\n");
        goto free_sharedMem;
    }
    /* 4. invoke ipc command */
    TEEC_Operation operation = { 0 };
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INPUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    operation.params[0].memref.parent = &sharedMem;
    operation.params[0].memref.offset = 0;
    operation.params[0].memref.size = fileSize;
    result = TEEC_InvokeCommand(session, IPC_IMPORT_CRL, &operation, NULL);
    if (result != TEEC_SUCCESS)
        printf_err("ipc failed\n");
free_sharedMem:
    TEEC_ReleaseSharedMemory(&sharedMem);
end:
    return result;
}

static TEEC_UUID g_taId = {
    0x4acaf7c8, 0xc652, 0x4643,
    { 0x9b, 0x7a, 0xcc, 0x07, 0xe7, 0xa3, 0x18, 0x7a }
};

static TEEC_Result OpenSessionTa(TEEC_Context *context, TEEC_Session *session)
{
    TEEC_UUID *uuidp = &g_taId;
    TEEC_Operation operation = { 0 };
    context->ta_path = (uint8_t *)TA_PATH;
    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_NONE, TEEC_NONE);
    TEEC_Result result = TEEC_OpenSession(context, session, uuidp, TEEC_LOGIN_IDENTIFY, NULL, &operation, NULL);
    if (result != TEEC_SUCCESS) {
        printf_err("teec open session failed,result is 0x%x\n", result);
        return result;
    }
    return TEEC_SUCCESS;
}

static int32_t GetInputCmd(int argc, char *argv[])
{
    if (argc < CMD_DESTROY_ARGC || argc > CMD_EXPORT_CERT_ARGC) {
        printf_err("wrong parameters num \n");
        return -1;
    }
    /* 1. process command line */
    if (memcmp(argv[CMD_NAME], CMD_IMPORT, sizeof(CMD_IMPORT)) == 0 && argc == CMD_IMPORT_ARGC) {
        /* import */
        return CERTMANGER_CMD_IMPORT_CERT;
    } else if (memcmp(argv[CMD_NAME], CMD_IMPORT_CRL, sizeof(CMD_IMPORT_CRL)) == 0 && argc == CMD_IMPORT_CRL_ARGC) {
        /* import crl */
        return CERTMANGER_CMD_IMPORT_CRL;
    } else if (memcmp(argv[CMD_NAME], CMD_EXPORT, sizeof(CMD_EXPORT)) == 0 &&
               (argc == CMD_EXPORT_CERT_ARGC || argc == CMD_EXPORT_LOG_ARGC)) {
        /* export */
        return CERTMANGER_CMD_EXPORT;
    } else if (memcmp(argv[CMD_NAME], CMD_DESTROY, sizeof(CMD_DESTROY)) == 0 && argc == CMD_DESTROY_ARGC) {
        /* destroy */
        return CERTMANGER_CMD_DESTROY;
    } else {
        /* undefined */
        printf("invalid command \n");
        return -1;
    }
}

static TEEC_Result SelectCmd(char *argv[], TEEC_Context *context, TEEC_Session *session, int32_t cmd)
{
    switch (cmd) {
        case CERTMANGER_CMD_IMPORT_CERT:
            return Import(context, session, argv[CMD_IMPORT_ARG_PATH]);
        case CERTMANGER_CMD_IMPORT_CRL:
            return ImportCrl(context, session, argv[CMD_IMPORT_ARG_PATH]);
        case CERTMANGER_CMD_EXPORT:
            return Export(context, session, argv[CMD_EXPORT_SUBCMD], argv[CMD_EXPORT_ARG_PATH]);
        case CERTMANGER_CMD_DESTROY:
            return Destroy(session);
        default:
            printf_err("failed, errno input:%d\n", cmd);
            return TEEC_ERROR_INVALID_CMD;
    }
}

int main(int argc, char *argv[])
{
    int32_t ret = 0;
    TEEC_Result result;
    TEEC_Context context;
    TEEC_Session session;

    /* 1. check input cmd */
    int32_t cmd = GetInputCmd(argc, argv);
    if (cmd <= 0) {
        printf_err("input parameter errno\n");
        return -1;
    }

    /* 2. init context */
    result = TEEC_InitializeContext(NULL, &context);
    if (result != TEEC_SUCCESS) {
        printf_err("teec initialize failed\n");
        return -1;
    }

    /* 3. open session */
    result = OpenSessionTa(&context, &session);
    if (result != TEEC_SUCCESS) {
        printf_err("teec open session failed\n");
        goto final;
    }

    /* 4. select cmd */
    result = SelectCmd(argv, &context, &session, cmd);
    switch (result) {
        case TEEC_SUCCESS:
            printf("success\n");
            break;
        case TEEC_ERROR_ITEM_NOT_FOUND:
            printf_err("ssa log is not exist\n");
            ret = -1;
            goto close;
        default:
            printf_err("certmanger error: %x\n", result);
            ret = -1;
            goto close;
    }
close:
    TEEC_CloseSession(&session);
final:
    TEEC_FinalizeContext(&context);
    return ret;
}
