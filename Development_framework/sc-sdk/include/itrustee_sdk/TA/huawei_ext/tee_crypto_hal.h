/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: crypto hal info function
 */

#ifndef TEE_CRYPTO_HAL_H
#define TEE_CRYPTO_HAL_H
#include "tee_crypto_api.h"
enum CRYPTO_ENGINE {
    DX_CRYPTO = 0,
    EPS_CRYPTO = 1,
    SOFT_CRYPTO = 2,
    SEC_CRYPTO = 3,
    CRYPTO_ENGINE_MAX = 1024,
};

/*
 * Set the encryption and decryption engine to operation
 *
 * @param operation [IN/OUT] The handle of operation
 * @param crypto    [IN]The engine to be set
 *
 * @retval TEE_SUCCESS: Set crypto engine success
 * @retval TEE_ERROR_BAD_PARAMETERS: Operation is NULLi or crypto is invalid
 */
TEE_Result TEE_SetCryptoFlag(TEE_OperationHandle operation, uint32_t crypto);

/*
 * Set the encryption and decryption engine to object
 *
 * @param operation [IN/OUT] The handle of object
 * @param crypto    [IN]The engine to be set
 *
 * @retval TEE_SUCCESS: Set crypto engine success
 * @retval TEE_ERROR_BAD_PARAMETERS: Operation is NULLi or crypto is invalid
 */
TEE_Result TEE_SetObjectFlag(TEE_ObjectHandle object, uint32_t crypto);

#endif
