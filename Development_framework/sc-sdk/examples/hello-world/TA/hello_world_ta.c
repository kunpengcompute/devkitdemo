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
 * Description: hello_world_ta.c
 * Create: 2022-08-15
 */
#include <string.h>
#include <sys/types.h>
#include "tee_core_api.h"
#include "tee_defines.h"
#include "tee_log.h"
#include "tee_crypto_api.h"
#include "tee_object_api.h"
#include "hello_world_ta.h"

TEE_Result TA_CreateEntryPoint(void)
{
    SLogTrace("----- TA_CreateEntryPoint ----- ");
    SLogTrace("TA version: %s ", TA_VERSION);

    // 验证CA程序路径和运行用户
    if (addcaller_ca_exec(CLIENT_APPLICATION_NAME, "root") != TEE_SUCCESS) {
        SLogError("TA_CreateEntryPoint: addcaller_ca_exec failed.");
        return TEE_ERROR_GENERIC;
    }
    SLogTrace("TA_CreateEntryPoint: addcaller_ca_exec ok.");
    return TEE_SUCCESS;
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE], void **sessionContext)
{
    TEE_Result ret = TEE_SUCCESS;
    SLogTrace("---- TA_OpenSessionEntryPoint -------- ");
    (void)paramTypes;
    (void)params;
    (void)sessionContext;

    return ret;
}

TEE_Result HelloWorld(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE])
{
    if (TEE_PARAM_TYPE_MEMREF_INPUT != TEE_PARAM_TYPE_GET(paramTypes, 0) && 
        TEE_PARAM_TYPE_MEMREF_OUTPUT != TEE_PARAM_TYPE_GET(paramTypes, 1)) {
        SLogError("Bad expected parameter types.");
        return TEE_ERROR_BAD_PARAMETERS;
    }
    SLogTrace("Recv from CA, msg: %s", params[PARAMS_IDX0].memref.buffer);

    char *msgHelloWorld = "Hello World!";
    TEE_MemMove(params[PARAMS_IDX1].memref.buffer, msgHelloWorld, strlen(msgHelloWorld));
    return TEE_SUCCESS;
}

TEE_Result TA_InvokeCommandEntryPoint(void *sessionContext, uint32_t cmdId,
                                      uint32_t paramTypes, TEE_Param params[PARAMS_SIZE])
{
    (void)sessionContext;
    TEE_Result ret;

    SLogTrace("---- TA_InvokeCommandEntryPoint ----cmdid: %d------- ", cmdId);
    switch (cmdId) {
        case CMD_HELLO_WORLD:
            ret = HelloWorld(paramTypes, params);
            break;

        default:
            SLogError("Unknown CMD ID: %d", cmdId);
            ret = TEE_FAIL;
    }

    return ret;
}

void TA_CloseSessionEntryPoint(void *sessionContext)
{
    TEE_Free(sessionContext);
    SLogTrace("---- TA_CloseSessionEntryPoint ----- ");
}

void TA_DestroyEntryPoint(void)
{
    SLogTrace("---- TA_DestroyEntryPoint ---- ");
}
