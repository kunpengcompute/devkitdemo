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
 * Description: Reference of TEE object api definitions
 */

#ifndef __TEE_OBJECT_API_H
#define __TEE_OBJECT_API_H

#include "tee_defines.h"

/*
 * Definition of HANDLE_NULL, invalid object handle
 */
#define TEE_HANDLE_NULL 0x00000000

/*
 * The keyUsage of TEE_ObjectHandle determines the usage of the object key
 */
enum Usage_Constants {
    TEE_USAGE_EXTRACTABLE = 0x00000001, /* The key of the object can be extracted */
    TEE_USAGE_ENCRYPT     = 0x00000002, /* The key of the object can be used for encryption */
    TEE_USAGE_DECRYPT     = 0x00000004, /* The key of the object can be used for decryption */
    TEE_USAGE_MAC         = 0x00000008, /* The key of the object can be used for hash */
    TEE_USAGE_SIGN        = 0x00000010, /* The key of the object can be used for signing */
    TEE_USAGE_VERIFY      = 0x00000020, /* The key of the object can be used for verification */
    TEE_USAGE_DERIVE      = 0x00000040, /* The key of the object can be used to derive */
    TEE_USAGE_DEFAULT     = 0xFFFFFFFF, /* object initialization, all permissions are assigned by default */
};

/*
 * The handleFlags of TEE_ObjectHandle indicate some information of the object,
 * whether it is a permanent object, whether it is initialized, etc.
 */
enum Handle_Flag_Constants {
    TEE_HANDLE_FLAG_PERSISTENT      = 0x00010000, /* Persistent object */
    TEE_HANDLE_FLAG_INITIALIZED     = 0x00020000, /* Object has been initialized */
    TEE_HANDLE_FLAG_KEY_SET         = 0x00040000, /* Unused */
    TEE_HANDLE_FLAG_EXPECT_TWO_KEYS = 0x00080000, /* Unused */
};

/*
 * List of attribute identifier flags
 */
#define TEE_ATTR_FLAG_VALUE  0x20000000
#define TEE_ATTR_FLAG_PUBLIC 0x10000000

#define TEE_ATTR_IS_BUFFER(attribute_id) ((((attribute_id) << 2) >> 31) == 0)
#define TEE_ATTR_IS_VALUE(attribute_id)  ((((attribute_id) << 2) >> 31) == 1)

#define TEE_ATTR_IS_PROTECTED(attribute_id) ((((attribute_id) << 3) >> 31) == 0)
#define TEE_ATTR_IS_PUBLIC(attribute_id)    ((((attribute_id) << 3) >> 31) == 1)

/*
 * Obtain the TEE_ObjectInfo of the object and copy it to the space pointed
 * to by the parameter objectInfo, which is pre-allocated by the user
 *
 * @param object [IN] Source TEE_ObjectHandle
 * @param objectInfo [OUT] Pointer to the structure used to store the TEE_ObjectInfo
 *
 * @return void
 */
void TEE_GetObjectInfo(TEE_ObjectHandle object, TEE_ObjectInfo *objectInfo);

/*
 * Limit the objectUsage bit of the object. This bit determines the usage of the key in the object.
 * The value range is Usage_Constants. For the flag bit of the parameter objectUsage:
 *    If this bit is set to 1, the use flag of object will not be changed
 *    If this bit is set to 0, the corresponding object usage flag of the object is cleared
 *
 * @attention The newly created object will contain all Usage_Constants,
 * and the usage flag can only be cleared, not set
 * @param object [IN] Need to restrict TEE_ObjectHandle
 * @param objectUsage [IN] ObjectUsage users want to change
 *
 * @return void
 */
void TEE_RestrictObjectUsage(TEE_ObjectHandle object, uint32_t objectUsage);

/*
 * Get the buffer content of the union in the TEE_Attribute structure of the object pointed
 * to by TEE_ObjectHandle, and the union member must be ref
 *
 * @attention The union member in the TEE_Attribute structure must be ref. If the TEE_Attribute is private,
 * the Usage_Constants of the object must include TEE_USAGE_EXTRACTABLE
 * @param object [IN]  The source TEE_ObjectHandle
 * @param attributeID [IN]  The Attribute ID you want to obtain, such as TEE_ObjectAttribute, can also be customized
 * @param buffer [OUT]  Pointer, the buffer pointed to is used to store the contents of the obtained buffer
 * @param size [INOUT]  Pointer, storing content byte length
 *
 * @return TEE_SUCCESS Indicates that the function was executed successfully
 * @return TEE_ERROR_ITEM_NOT_FOUND The TEE_Attribute you are looking for is not found in the object,
 * or the object is not initialized
 * @return TEE_ERROR_SHORT_BUFFER The provided buffer is too small to store the acquired content
 */
TEE_Result TEE_GetObjectBufferAttribute(TEE_ObjectHandle object, uint32_t attributeID, void *buffer, size_t *size);

/*
 * Get the value of the union in the TEE_Attribute in the object, and the union member must be the value
 *
 * @attention The member of the union in the TEE_Attribute structure must be value. If the TEE_Attribute is private,
 * the Usage_Constants of the object must include TEE_USAGE_EXTRACTABLE
 * @param object [IN]  The source TEE_ObjectHandle
 * @param attributeID [IN]  The Attribute ID you want to obtain, such as TEE_ObjectAttribute, can also be customized
 * @param a [OUT]  Pointer, the space pointed to is used to store a
 * @param b [OUT]  Pointer, the space pointed to is used to store b
 *
 * @return TEE_SUCCESS Indicates that the function was executed successfully
 * @return TEE_ERROR_ITEM_NOT_FOUND The TEE_Attribute you are looking for is not found in the object,
 * or the object is not initialized
 * @return TEE_ERROR_ACCESS_DENIED Attempt to obtain a private TEE_Attribute but did not set TEE_USAGE_EXTRACTABLE
 */
TEE_Result TEE_GetObjectValueAttribute(TEE_ObjectHandle object, uint32_t attributeID, uint32_t *a, uint32_t *b);

/*
 * Close the opened TEE_ObjectHandle, the object can be a persistent object or a transient object
 *
 * @param object [IN]  TEE_ObjectHandle to be closed
 *
 * @return void
 */
void TEE_CloseObject(TEE_ObjectHandle object);

/*
 * Allocate an uninitialized object to store the key, in which objectType and maxObjectSize
 * must be specified to pre-allocate
 *
 * @param objectType [IN]  The type of the object to be created, the value can be TEE_ObjectType
 * @param maxObjectSize [IN]  Maximum bytes of object
 * @param object [OUT]  Pointer to the handle of the newly created object
 *
 * @return TEE_SUCCESS Indicates that the function was executed successfully
 * @return TEE_ERROR_OUT_OF_MEMORY Not enough  to allocate
 * @return TEE_ERROR_NOT_SUPPORTED The bytes provided by the object is not supported
 */
TEE_Result TEE_AllocateTransientObject(uint32_t objectType, uint32_t maxObjectSize, TEE_ObjectHandle *object);

/*
 * Release a transient object that has allocated. After the function is called, the handle becomes invalid,
 * and all allocated  are released. Paired with TEE_AllocateTransientObject
 *
 * @param object [IN]  TEE_ObjectHandle to be released
 *
 * @return void
 */
void TEE_FreeTransientObject(TEE_ObjectHandle object);

/*
 * Reset the transient object to initial state, that is, the state just after the allocate.
 * The uninitialized object that has allocated but has not stored the key can be reused to store the key
 *
 * @param object [IN]  TEE_ObjectHandle to be reset
 *
 * @return void
 */
void TEE_ResetTransientObject(TEE_ObjectHandle object);

/*
 * This function assigns the attributes in the parameter attrs to an uninitialized transient object,
 * and the parameter attrs is provided by the trusted application (TA)
 *
 * @param object [IN/OUT]  TEE_ObjectHandle created but not initialized
 * @param attrs [IN]  object attribute array, can be one or more TEE_Attribute
 * @param attrCount [IN]  Number of array members
 *
 * @return TEE_SUCCESS Indicates that the function was executed successfully
 * @return TEE_ERROR_BAD_PARAMETERS The attribute is incorrect or inconsistent.
 * Ensure that the object is still uninitialized
 */
TEE_Result TEE_PopulateTransientObject(TEE_ObjectHandle object, TEE_Attribute *attrs, uint32_t attrCount);

/*
 * Initialize a buffer type TEE_Attribute, that is, the member of union in the TEE_Attribute structure must be ref
 *
 * @param attr [OUT]  TEE_Attribute to be initialized
 * @param attributeID [IN]  ID assigned to TEE_Attribute
 * @param buffer [IN]  The buffer stores the content to be assigned
 * @param length [IN]  The byte length of the assignment content
 *
 * @return void
 */
void TEE_InitRefAttribute(TEE_Attribute *attr, uint32_t attributeID, void *buffer, size_t length);

/*
 * Initialize a value type TEE_Attribute
 *
 * @param attr [OUT]  TEE_Attribute to be initialized
 * @param attributeID [IN]  ID assigned to TEE_Attribute
 * @param a [IN]  Assign the value to the member value a of the union in the TEE_Attribute
 * @param b [IN]  Assign the value to the member value b of the union in the TEE_Attribute
 *
 * @return void
 */
void TEE_InitValueAttribute(TEE_Attribute *attr, uint32_t attributeID, uint32_t a, uint32_t b);

/*
 * This function uses an initialized object to assign TEE_Attribute to an uninitialized object,
 * which is equivalent to copying the TEE_Attribute of srcobject to destobject
 *
 * @attention The TEE_Attribute type and number of the two objects must match
 * @param destObject [OUT]  The uninitialized TEE_ObjectHandle to be assigned
 * @param srcObject [IN]  The initialized TEE_ObjectHandle is used to assign a value to another object
 *
 * @return void
 */
void TEE_CopyObjectAttributes(TEE_ObjectHandle destObject, TEE_ObjectHandle srcObject);

/*
 * This function generates a random key or key-pair and assigns it to the transient object
 *
 * @param object [IN/OUT]  Transient object, used to store the generated key
 * @param keySize [IN]  The bytes of the required key
 * @param params [IN]  Parameters required for key generation
 * @param paramCount [IN]  The number of parameters required to generate the key
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 * @return TEE_ERROR_BAD_PARAMETERS  The generated key is inconsistent with the key type
 * that the transient object can store
 */
TEE_Result TEE_GenerateKey(TEE_ObjectHandle object, uint32_t keySize, TEE_Attribute *params, uint32_t paramCount);

/*
 * Get the information of the object data part, the total length of the data part and the current
 * position of the data stream
 *
 * @param object [IN]  TEE_ObjectHandle to be obtained
 * @param pos [out]  Data stream position
 * @param len [IN]  Data stream length
 *
 * @return void
 */
TEE_Result TEE_InfoObjectData(TEE_ObjectHandle object, uint32_t *pos, uint32_t *len);

#if defined(API_LEVEL) && (API_LEVEL >= 2)

/*
 * Obtain the TEE_ObjectInfo of the object and copy it to the space pointed
 * to by the parameter objectInfo, which is pre-allocated by the user
 *
 * @param object [IN] Source TEE_ObjectHandle
 * @param objectInfo [OUT] Pointer to the structure used to store the TEE_ObjectInfo
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 * @return TEE_ERROR_CORRUPT_OBJECT  The file is damaged and the file handle will be closed
 * @return TEE_ERROR_STORAGE_NOT_AVAILABLE  Cannot access the storage area where the file is located
 */
TEE_Result TEE_GetObjectInfo1(TEE_ObjectHandle object, TEE_ObjectInfo *objectInfo);

/*
 * This function uses an initialized object to assign TEE_Attribute to an uninitialized object,
 * which is equivalent to copying the TEE_Attribute of srcobject to destobject
 *
 * @attention The TEE_Attribute type and number of the two objects must match
 * @param destObject [IN/OUT]  The uninitialized TEE_ObjectHandle to be assigned
 * @param srcObject [IN]  The initialized TEE_ObjectHandle is used to assign a value to another object
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 * @return TEE_ERROR_CORRUPT_OBJECT  The file is damaged and the file handle will be closed
 * @return TEE_ERROR_STORAGE_NOT_AVAILABLE  Cannot access the storage area where the file is located
 */
TEE_Result TEE_CopyObjectAttributes1(TEE_ObjectHandle destObject, TEE_ObjectHandle srcObject);

/*
 * Limit the objectUsage bit of the object. This bit determines the usage of the key in the object.
 * The value range is Usage_Constants. For the flag bit of the parameter objectUsage:
 *    If this bit is set to 1, the use flag of object will not be changed
 *    If this bit is set to 0, the corresponding object usage flag of the object is cleared
 *
 * @attention The newly created object will contain all Usage_Constants, and the usage flag can
 * only be cleared, not set
 * @param object [IN/OUT] Need to restrict TEE_ObjectHandle
 * @param objectUsage [IN] ObjectUsage users want to change
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 * @return TEE_ERROR_CORRUPT_OBJECT  The file is damaged and the file handle will be closed
 * @return TEE_ERROR_STORAGE_NOT_AVAILABLE  Cannot access the storage area where the file is located
 */
TEE_Result TEE_RestrictObjectUsage1(TEE_ObjectHandle object, uint32_t objectUsage);

#endif /* API_LEVEL */
#endif
