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
 * Description: hello_world_ca.c
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
#include "hello_world_ca.h"

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

// 初始化
static TEEC_Result TeecInit()
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
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);

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

    // 创建TEE会话
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

// InvokeCommand
static TEEC_Result HelloWorld(char* msg, uint32_t msgLen)
{
    TEEC_Operation operation;
    TEEC_Result result;
    uint32_t origin;

    if (msg == NULL || msgLen == 0) {
        TEEC_Error("invalid params.");
        return (TEEC_Result)TEEC_ERROR_GENERIC;
    }

    operation.started = 1;
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_MEMREF_TEMP_INPUT,
        TEEC_MEMREF_TEMP_OUTPUT,
        TEEC_NONE,
        TEEC_NONE);

    operation.params[PARAMS_IDX0].tmpref.buffer = msg;
    operation.params[PARAMS_IDX0].tmpref.size = msgLen;
    char buffer[32] = {0};
    operation.params[PARAMS_IDX1].tmpref.buffer = buffer;
    operation.params[PARAMS_IDX1].tmpref.size = 32;
    result = TEEC_InvokeCommand(&g_session, CMD_HELLO_WORLD, &operation, &origin);
    if (result != TEEC_SUCCESS) {
        TEEC_Error("invoke CMD_HELLO_WORLD failed, codes=0x%x, origin=0x%x.", result, origin);
    } else {
        printf("Recv from TA: %s\n", buffer);
    }
    return result;
}

int main(int argc, char *argv[])
{
    TEEC_Result ret;
    ret = TeecInit();
    if (ret == TEEC_ERROR_ACCESS_DENIED) {
        printf("TEEC_ERROR_ACCESS_DENIED.\n");
        return 1;
    }
    if (TEEC_SUCCESS != ret) {
        printf("TeecInit Failed!\n");
        return 1;
    }
    char *msg = "Message from CA.";
    HelloWorld(msg, strlen(msg));
    TeecClose();
    return 0;
}
