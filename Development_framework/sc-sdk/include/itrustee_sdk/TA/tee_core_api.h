/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: tee core api definitions
 */
#ifndef __TEE_CORE_API_H
#define __TEE_CORE_API_H

#include "tee_defines.h"
#ifndef TEE_TA_SESSION_HANDLE
#define TEE_TA_SESSION_HANDLE
typedef uint32_t TEE_TASessionHandle;
#endif

/*
 * Raises a Panic in the Trusted Application instance
 *
 * @param panicCode [IN] informative panic code defined by the TA
 *
 * @return void
 */
void TEE_Panic(TEE_Result panicCode);

/*
 * opens a new session with a Trusted Application
 *
 * @param destination [IN]  A pointer to a TEE_UUID structure containing the UUID of the destination Trusted
 * Application
 * @param cancellationRequestTimeout [IN] Timeout in milliseconds or the special value
 * @param paramTypes  [IN]  The types of all parameters passed in the operation
 * @param params      [IN]  The parameters passed in the operation
 * @param session     [OUT] A pointer to a variable that will receive the client session handle
 * @param returnOrigin[OUT] A pointer to a variable which will contain the return origin
 *
 * @return TEE_SUCCESS open session successfully
 * @return TEE_ERROR_ITEM_NOT_FOUND failed to find target TA in TEE
 * @return TEE_ERROR_ACCESS_DENIED access to the destination Trusted Application is denied
 */
TEE_Result TEE_OpenTASession(const TEE_UUID *destination, uint32_t cancellationRequestTimeout, uint32_t paramTypes,
                             TEE_Param params[TEE_PARAMS_NUM], TEE_TASessionHandle *session, uint32_t *returnOrigin);

/*
 * closes a client session opened by TEE_OpenTASession
 *
 * @param session [IN] session handle opened by TEE_OpenTASession
 *
 * @return void
 */
void TEE_CloseTASession(TEE_TASessionHandle session);

/*
 * invokes a command within a session opened between the client Trusted Application instance
 * and a destination Trusted Application instance
 *
 * @param session      [IN] An opened session handle
 * @param cancellationRequestTimeout [IN] Timeout in milliseconds or the special value
 * @param commandID    [IN] The identifier of the Command to invoke
 * @param paramTypes   [IN] The types of all parameters passed in the operation
 * @param params       [IN] The parameters passed in the operation
 * @param returnOrigin [IN] A pointer to a variable which will contain the return origin
 *
 * @return TEE_SUCCESS invoke operation successfully
 * @return TEE_ERROR_ACCESS_DENIED invoke command to target TA is denied
 */
TEE_Result TEE_InvokeTACommand(TEE_TASessionHandle session, uint32_t cancellationRequestTimeout, uint32_t commandID,
                               uint32_t paramTypes, TEE_Param params[TEE_PARAMS_NUM], uint32_t *returnOrigin);

/* not supported */
bool TEE_GetCancellationFlag(void);

/* not supported */
bool TEE_UnmaskCancellation(void);

/* not supported */
bool TEE_MaskCancellation(void);

#endif
