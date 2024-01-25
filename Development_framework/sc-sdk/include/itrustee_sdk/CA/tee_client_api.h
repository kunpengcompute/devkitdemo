/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2023. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: TEE client API definitions
 */

#ifndef _TEE_CLIENT_API_H_
#define _TEE_CLIENT_API_H_

#ifdef LOG_NDEBUG
#undef LOG_NDEBUG
#endif
#define LOG_NDEBUG 0

#include <string.h>
#include "tee_client_type.h"
#include "tee_client_log.h"

#ifdef __cplusplus
extern "C" {
#endif

#define S_VAR_NOT_USED(variable)  do { (void)(variable); } while (0)

#define TEEC_PARAM_TYPES(param0Type, param1Type, param2Type, param3Type) \
    ((param3Type) << 12 | (param2Type) << 8 | (param1Type) << 4 | (param0Type))

#define TEEC_PARAM_TYPE_GET(paramTypes, index) \
    (((paramTypes) >> (4*(index))) & 0x0F)

#define TEEC_VALUE_UNDEF 0xFFFFFFFF

/*
 *  initializes a new TEE Context, forming a connection between this Client Application and the TEE
 *
 * @param name [IN] TEE name (unused)
 * @param context [OUT] pointer to TEEC_Context to be initialized
 *
 * @return TEEC_SUCCESS operation success
 * @return TEEC_ERROR_BAD_PARAMETERS invalid parameter
 * @return TEEC_ERROR_GENERIC system error unhandled
 */
TEEC_Result TEEC_InitializeContext(
    const char *name,
    TEEC_Context *context);

/*
 * finalizes an initialized TEE Context, closing the connection between the Client Application and the TEE
 *
 * @param context [IN/OUT] pointer to TEEC_Context initialized by TEEC_InitializeContext
 *
 * @return void
 */
void TEEC_FinalizeContext(
    TEEC_Context *context);

/*
 * opens a new Session between the Client Application and the specified Trusted Application
 *
 * @param context [IN/OUT] a pointer to an initialized TEE Context
 * @param session [OUT]  a pointer to a Session structure to be opened
 * @param destination [IN] a pointer to a structure containing the UUID of the destination Trusted Application
 * @param connectionMethod [IN] the method of connection to use
 * @param connectionData [IN] any necessary data required to support the connection method
 * @param operation [IN/OUT] a pointer to an Operation containing a set of Parameters to exchange with the
 * Trusted Application
 * @param returnOrigin [OUT]  a pointer to a variable which will contain the return origin, This field may be NULL
 * if the return origin is not needed
 *
 * @return TEEC_SUCCESS operation success
 * @return TEEC_ERROR_BAD_PARAMETERS invalid parameter, context or session or destination is NULL
 * @return TEEC_ERROR_ACCESS_DENIED client Application's connection request is denied
 * @return TEEC_ERROR_OUT_OF_MEMORY system resource is out of use
 * @return TEEC_ERROR_TRUSTED_APP_LOAD_ERROR load Trusted Application failed
 * @return others refer TEEC_ReturnCode
 */
TEEC_Result TEEC_OpenSession(
    TEEC_Context *context,
    TEEC_Session *session,
    const TEEC_UUID *destination,
    uint32_t connectionMethod,
    const void *connectionData,
    TEEC_Operation *operation,
    uint32_t *returnOrigin);

/*
 * closes a Session which has been opened with a Trusted Application
 *
 * @param session [IN/OUT] pointer to a session to be closed
 *
 * @return void
 */
void TEEC_CloseSession(
    TEEC_Session *session);

/*
 * invokes a Command within the specified Session
 *
 * @param session [IN/OUT] the open Session in which the command will be invoked
 * @param commandID [IN] the identifier of the Command within the Trusted Application to invoke
 * @param operation [IN/OUT] a pointer to a Client Application initialized TEEC_Operation structure
 * @param returnOrigin [OUT] a pointer to a variable which will contain the return origin
 *
 * @return TEEC_SUCCESS operation success
 * @return TEEC_ERROR_BAD_PARAMETERS invalid parameter, session is NULL or operation data invalid
 * @return TEEC_ERROR_ACCESS_DENIED invoke command operation is denied
 * @return TEEC_ERROR_OUT_OF_MEMORY system resource is out of use
 * @return others refer TEEC_ReturnCode
 */
TEEC_Result TEEC_InvokeCommand(
    TEEC_Session     *session,
    uint32_t          commandID,
    TEEC_Operation   *operation,
    uint32_t         *returnOrigin);

/*
 * registers a block of existing Client Application memory as a block of Shared Memory within
 * the scope of the specified TEE Context, in accordance with the parameters which have been set by the
 * Client Application inside the sharedMem structure (don't support 0 size data)
 *
 * @param context [IN/OUT] a pointer to an initialized TEE Context
 * @param sharedMem [IN/OUT] a pointer to a Shared Memory structure to register
 *
 * @return TEEC_SUCCESS operation success
 * @return TEEC_ERROR_BAD_PARAMETERS invalid parameter, context or sharedMem is NULL
 */
TEEC_Result TEEC_RegisterSharedMemory(
    TEEC_Context *context,
    TEEC_SharedMemory *sharedMem);

/*
 * allocates a new block of memory as a block of Shared Memory within the scope of the specified TEE Context
 * size of sharedMem should not be 0
 *
 * @param context [IN/OUT] a pointer to an initialized TEE Context
 * @param sharedMem [IN/OUT] a pointer to a Shared Memory structure to allocate
 *
 * @return TEEC_SUCCESS operation success
 * @return TEEC_ERROR_BAD_PARAMETERS invalid parameter, context or sharedMem is NULL
 * @return TEEC_ERROR_OUT_OF_MEMORY system resource is out of use
 */
TEEC_Result TEEC_AllocateSharedMemory(
    TEEC_Context *context,
    TEEC_SharedMemory *sharedMem);

/*
 * deregisters or deallocates a previously initialized block of Shared Memory
 * if memory is allocated by TEEC_AllocateSharedMemory, system will free this memory
 * if memory is registered by TEEC_RegisterSharedMemory, system will not free this memory
 *
 * @param sharedMem [IN/OUT] a pointer to a valid Shared Memory structure
 *
 * @return void
 */
void TEEC_ReleaseSharedMemory(
    TEEC_SharedMemory *sharedMem);

/*
 * requests the cancellation of a pending open Session operation or a Command invocation operation
 * this operation is not supported currently
 *
 * @param operation [IN/OUT] a pointer to a Client Application instantiated Operation structure
 *
 * @return void
 */
void TEEC_RequestCancellation(
    TEEC_Operation *operation);

#ifdef __cplusplus
}
#endif

#endif
