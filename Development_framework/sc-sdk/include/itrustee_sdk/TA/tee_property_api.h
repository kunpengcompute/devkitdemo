/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: TEE property implementation header file
 */

#ifndef TEE_PROPERTY_API_H
#define TEE_PROPERTY_API_H

#include "tee_defines.h"

/*
 * below definitions are defined by Global Platform or Platform SDK released previously
 * for compatibility:
 * don't make any change to the content below
 */
typedef enum {
    TEE_PROPSET_UNKNOW             = 0,
    TEE_PROPSET_TEE_IMPLEMENTATION = 0xFFFFFFFD,
    TEE_PROPSET_CURRENT_CLIENT     = 0xFFFFFFFE,
    TEE_PROPSET_CURRENT_TA         = 0xFFFFFFFF,
} Pseudo_PropSetHandle;

typedef uint32_t TEE_PropSetHandle;

/*
 * performs a lookup in a property set to retrieve an individual
 * property and convert its value into a printable string
 *
 * @param propsetOrEnumerator [IN] One of the TEE_PROPSET_XXX pseudo-handles or a handle on a property enumerator
 * @param name                [IN] Pointer to the zero-terminated string containing name of the property to retrieve
 * @param valueBuffer         [OUT] Output buffer for the property value
 * @param valueBufferLen      [IN/OUT] Output buffer length
 *
 * @return TEE_SUCCESS operation success
 * @return TEE_ERROR_ITEM_NOT_FOUND cannot find target property
 * @return TEE_ERROR_SHORT_BUFFER the value buffer is not large enough to hold the whole property value
 */
TEE_Result TEE_GetPropertyAsString(TEE_PropSetHandle propsetOrEnumerator, const char *name, char *valueBuffer,
                                   size_t *valueBufferLen);

/*
 * retrieves a single property in a property set and converts its value to a Boolean
 *
 * @param propsetOrEnumerator [IN] One of the TEE_PROPSET_XXX pseudo-handles or a handle on a property enumerator
 * @param name                [IN] Pointer to the zero-terminated string containing name of the property to retrieve
 * @param value               [OUT] A pointer to the variable that will contain the value of the property
 *
 * @return TEE_SUCCESS operation success
 * @return TEE_ERROR_ITEM_NOT_FOUND cannot find target property
 */
TEE_Result TEE_GetPropertyAsBool(TEE_PropSetHandle propsetOrEnumerator, const char *name, bool *value);

/*
 * retrieves a single property in a property set and converts its value to a 32-bit unsigned integer
 *
 * @param propsetOrEnumerator [IN] One of the TEE_PROPSET_XXX pseudo-handles or a handle on a property enumerator
 * @param name                [IN] Pointer to the zero-terminated string containing name of the property to retrieve
 * @param value               [OUT] A pointer to the variable that will contain the value of the property
 *
 * @return TEE_SUCCESS operation success
 * @return TEE_ERROR_ITEM_NOT_FOUND cannot find target property
 */
TEE_Result TEE_GetPropertyAsU32(TEE_PropSetHandle propsetOrEnumerator, const char *name, uint32_t *value);

/*
 * retrieves a single property in a property set and converts its value to a 64-bit unsigned integer
 *
 * @param propsetOrEnumerator [IN] One of the TEE_PROPSET_XXX pseudo-handles or a handle on a property enumerator
 * @param name                [IN] Pointer to the zero-terminated string containing name of the property to retrieve
 * @param value               [IN/OUT] A pointer to the variable that will contain the value of the property
 *
 * @return TEE_SUCCESS operation success
 * @return TEE_ERROR_ITEM_NOT_FOUND cannot find target property
 */
TEE_Result TEE_GetPropertyAsU64(TEE_PropSetHandle propsetOrEnumerator, const char *name, uint64_t *value);

/*
 * retrieves an individual property and converts its value into a binary block
 *
 * @param propsetOrEnumerator [IN] One of the TEE_PROPSET_XXX pseudo-handles or a handle on a property enumerator
 * @param name                [IN] Pointer to the zero-terminated string containing name of the property to retrieve
 * @param valueBuffer         [OUT] Output buffer for the property value
 * @param valueBufferLen      [IN/OUT] Output buffer length
 *
 * @return TEE_SUCCESS operation success
 * @return TEE_ERROR_ITEM_NOT_FOUND cannot find target property
 * @return TEE_ERROR_SHORT_BUFFER the value buffer is not large enough to hold the whole property value
 */
TEE_Result TEE_GetPropertyAsBinaryBlock(TEE_PropSetHandle propsetOrEnumerator, const char *name, void *valueBuffer,
                                        size_t *valueBufferLen);

/*
 * retrieves a single property in a property set and converts its value to TEE_UUID struct
 *
 * @param propsetOrEnumerator [IN] One of the TEE_PROPSET_XXX pseudo-handles or a handle on a property enumerator
 * @param name                [IN] Pointer to the zero-terminated string containing name of the property to retrieve
 * @param value               [OUT] A pointer to the variable that will contain the value of the property
 *
 * @return TEE_SUCCESS operation success
 * @return TEE_ERROR_ITEM_NOT_FOUND cannot find target property
 */
TEE_Result TEE_GetPropertyAsUUID(TEE_PropSetHandle propsetOrEnumerator, const char *name, TEE_UUID *value);

/*
 * retrieves a single property in a property set and converts its value to TEE_Identity struct
 *
 * @param propsetOrEnumerator [IN] One of the TEE_PROPSET_XXX pseudo-handles or a handle on a property enumerator
 * @param name                [IN] Pointer to the zero-terminated string containing name of the property to retrieve
 * @param value               [OUT] A pointer to the variable that will contain the value of the property
 *
 * @return TEE_SUCCESS operation success
 * @return TEE_ERROR_ITEM_NOT_FOUND cannot find target property
 */
TEE_Result TEE_GetPropertyAsIdentity(TEE_PropSetHandle propsetOrEnumerator, const char *name, TEE_Identity *identity);

/*
 * allocates a property enumerator object
 *
 * @param enumerator [OUT] A pointer filled with an opaque handle on the property enumerator
 *
 * @return TEE_SUCCESS operation success
 * @return TEE_ERROR_OUT_OF_MEMORY not enough resources to allocate the property enumerator
 */
TEE_Result TEE_AllocatePropertyEnumerator(TEE_PropSetHandle *enumerator);

/*
 * deallocates a property enumerator object
 *
 * @param enumerator [IN] A handle on the enumerator to free
 *
 * @return void
 */
void TEE_FreePropertyEnumerator(TEE_PropSetHandle enumerator);

/*
 * starts to enumerate the properties in an enumerator
 *
 * @param enumerator [IN] A handle on the enumerator
 * @param propSet    [IN] A pseudo-handle on the property set to enumerate
 *
 * @return void
 */
void TEE_StartPropertyEnumerator(TEE_PropSetHandle enumerator, TEE_PropSetHandle propSet);

/*
 * resets a property enumerator to its state immediately after allocation
 *
 * @param enumerator A handle on the enumerator to reset
 *
 * @return void
 */
void TEE_ResetPropertyEnumerator(TEE_PropSetHandle enumerator);

/*
 * gets the name of the current property in an enumerator
 *
 * @param enumerator    [IN]     A handle on the enumerator
 * @param nameBuffer    [OUT]    The buffer to be filled with the name
 * @param nameBufferLen [IN/OUT] The length of buffer to be filled
 *
 * @return TEE_SUCCESS operation success
 * @return TEE_ERROR_ITEM_NOT_FOUND no current property either because the enumerator has not started
 * or because it has reached the end of the property set
 * @return TEE_ERROR_SHORT_BUFFER If the name buffer is not large enough to contain the property name
 */
TEE_Result TEE_GetPropertyName(TEE_PropSetHandle enumerator, void *nameBuffer, size_t *nameBufferLen);

/*
 * advances the enumerator to the next property
 *
 * @param enumerator [IN] A handle on the enumerator
 *
 * @return TEE_SUCCESS operation success
 * @return TEE_ERROR_ITEM_NOT_FOUND  enumerator has reached the end of the property set or if it has not started
 */
TEE_Result TEE_GetNextProperty(TEE_PropSetHandle enumerator);
#endif
