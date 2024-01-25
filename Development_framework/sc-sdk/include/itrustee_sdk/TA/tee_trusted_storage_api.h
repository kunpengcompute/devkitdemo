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
 * Description: Reference of TEE trusted storage definitions
 */

#ifndef __TEE_TRUSTED_STORAGE_API_H
#define __TEE_TRUSTED_STORAGE_API_H

#include "tee_defines.h"
#include "tee_object_api.h"

/*
 * Data stream positioning start position option, used in TEE_SeekObjectData function
 */
enum __TEE_Whence {
    TEE_DATA_SEEK_SET = 0, /* Position the starting position as the beginning of the data stream */
    TEE_DATA_SEEK_CUR,     /* Position the starting position as the current data stream position */
    TEE_DATA_SEEK_END      /* Position the starting position at the end of the data stream */
};

struct __TEE_ObjectEnumHandle;
typedef struct __TEE_ObjectEnumHandle *TEE_ObjectEnumHandle;

typedef uint32_t TEE_Whence;

/*
 * Storage ID, which defines the storage space of the corresponding application
 */
enum Object_Storage_Constants {
    TEE_OBJECT_STORAGE_PRIVATE = 0x00000001, /* Separate private storage space for each application */
};

/*
 * System resource constraints, such as the maximum value that the data stream position indicator can take
 */
enum Miscellaneous_Constants {
    TEE_DATA_MAX_POSITION = 0xFFFFFFFF, /* The maximum length that the position indicator of the data stream can take */
    TEE_OBJECT_ID_MAX_LEN = 64,         /* The maximum length of objectID, which actually extends to 128 bytes */
};

/*
 * The maximum number of bytes of data that the data stream can store
 */
enum TEE_DATA_Size {
    TEE_DATA_OBJECT_MAX_SIZE = 0xFFFFFFFF /* The maximum bytes of data that the object data stream can store */
};

/*
 * The handleFlags of TEE_ObjectHandle determines the access authority of
 * the TEE_ObjectHandle to the object data stream
 */
enum Data_Flag_Constants {
    /* Have read permission to the data stream, and can read */
    TEE_DATA_FLAG_ACCESS_READ = 0x00000001,
    /* Have write permission to the data stream, and can write and truncate */
    TEE_DATA_FLAG_ACCESS_WRITE = 0x00000002,
    /* Have WRITE_META permission for data stream, and can delete and rename operation */
    TEE_DATA_FLAG_ACCESS_WRITE_META = 0x00000004,
    /*
     * Have shared read permissions on the data stream, you can open multiple
     * TEE_ObjectHandles for concurrent reading
     */
    TEE_DATA_FLAG_SHARE_READ = 0x00000010,
    /*
     * Have shared write permissions for the data stream, and multiple TEE_ObjectHandles
     * can be opened for concurrent writing
     */
    TEE_DATA_FLAG_SHARE_WRITE = 0x00000020,
    /* Unused */
    TEE_DATA_FLAG_CREATE = 0x00000200,
    /*
     * Protect an existing file with the same name. If the file with the same name does not exist,
     * create a new data file; if the file with the same name exists, an error will be reported.
     * Used in GP v1.1, deprecated in GP v1.2
     */
    TEE_DATA_FLAG_EXCLUSIVE = 0x00000400,
    /*
     * Protect an existing file with the same name. If the file with the same name does not exist,
     * create a new data file; if the file with the same name exists, an error will be reported.
     * Used in GP v1.2
     */
    TEE_DATA_FLAG_OVERWRITE = 0x00000400,
    /*
     * If the bit27 is set to 1, it means deriving the 32-bytes TA root key at one time,
     * if it is 0, it means  deriving two 16-bytes TA root keys and combined them together
     */
    TEE_DATA_FLAG_DERIVE_32BYTES_KEY_ONCE = 0x08000000,
    /* If bit28 is set to 1, it means AES256, if it is 0, it means AES128 */
    TEE_DATA_FLAG_AES256 =  0x10000000,
    /* If bit29 is set to 1, it means that the lower version will be opened first */
    TEE_DATA_FLAG_OPEN_AESC = 0x20000000,
};

/*
 * Create a new persistent object, you can directly initialize the data stream and TEE_Attribute,
 * the user can use the returned handle to access the object's TEE_Attribute and data stream
 *
 * @param storageID [IN]  Corresponding to a separate storage space for each application,
 * the value is Object_Storage_Constants
 * @param objectID [IN]  Object identifier, the name of the object to be created
 * @param objectIDLen [IN]  The length of the object identifier by byte, no more than 128 bytes
 * @param flags [IN]  Flags after object creation, the value can be one or more of Data_Flag_Constants
 * or Handle_Flag_Constants
 * @param attributes [IN]  The TEE_ObjectHandle of the transient object, used to initialize the
 * TEE_Attribute of the object, can be TEE_HANDLE_NULL
 * @param initialData [IN]  Initial data, used to initialize data stream data
 * @param initialDataLen [IN] InitialData length in byte
 * @param object [OUT]  TEE_ObjectHandle returned after the function is successfully executed
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 * @return TEE_ERROR_ITEM_NOT_FOUND:  The storageID does not exist
 * @return TEE_ERROR_ACCESS_CONFLICT  Access conflict
 * @return TEE_ERROR_OUT_OF_MEMORY  Insufficient memory to complete the operation
 * @return TEE_ERROR_STORAGE_NO_SPACE  There is not enough space to create the object
 */
TEE_Result TEE_CreatePersistentObject(uint32_t storageID, const void *objectID, size_t objectIDLen, uint32_t flags,
                                      TEE_ObjectHandle attributes, const void *initialData, size_t initialDataLen,
                                      TEE_ObjectHandle *object);

/*
 * Open an existing permanent object, the returned handle can be used by the user to access
 * the object's TEE_Attribute and data stream
 *
 * @param storageID [IN]  orresponding to a separate storage space for each application,
 * the value is Object_Storage_Constants
 * @param objectID [IN]  object identifier, the name of the object to be opened
 * @param objectIDLen [IN]  The length of the object identifier by byte, no more than 128 bytes
 * @param flags [IN]  Flags after object opened, the value can be one or more of
 * Data_Flag_Constants or Handle_Flag_Constants
 * @param object [OUT]  TEE_ObjectHandle returned after the function is successfully executed
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 * @return TEE_ERROR_ITEM_NOT_FOUND:  The storageID does not exist or cannot find object identifier
 * @return TEE_ERROR_ACCESS_CONFLICT  Access conflict
 * @return TEE_ERROR_OUT_OF_MEMORY  Insufficient memory to complete the operation
 */
TEE_Result TEE_OpenPersistentObject(uint32_t storageID, const void *objectID, size_t objectIDLen, uint32_t flags,
                                    TEE_ObjectHandle *object);

/*
 * Read size bytes of data from the object's data stream to the buffer,
 * the TEE_ObjectHandle must have been opened with TEE_DATA_FLAG_ACCESS_READ permission
 *
 * @param objbect [IN]  The TEE_ObjectHandle to be read
 * @param buffer [OUT]  Buffer for storing read data
 * @param size [IN]  Size of data to be read by byte
 * @param count [OUT]  Size of data actually read by byte
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 * @return TEE_ERROR_OUT_OF_MEMORY  Insufficient memory to complete the operation
 */
TEE_Result TEE_ReadObjectData(TEE_ObjectHandle object, void *buffer, size_t size, uint32_t *count);

/*
 * Write size bytes of data from the buffer to the data stream of the object.
 * TEE_ObjectHandle must have been opened with TEE_DATA_FLAG_ACCESS_WRITE permission
 *
 * @param ojbect [IN]  The TEE_ObjectHandle to be write
 * @param buffer [IN]  Store the data to be written
 * @param size [IN]  The length of the data to be written, the size does not exceed 4096 bytes
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 * @return TEE_ERROR_OUT_OF_MEMORY  Insufficient memory to complete the operation
 * @return TEE_ERROR_STORAGE_NO_SPACE  There is not enough space to perform the operation
 */
TEE_Result TEE_WriteObjectData(TEE_ObjectHandle object, const void *buffer, size_t size);

/*
 * This function changes the size of the data stream. If the size is smaller than the size of
 * the current data stream, delete all excess bytes. If size is greater than the size of the
 * current data stream, use '0' to expand
 * TEE_ObjectHandle must be opened with TEE_DATA_FLAG_ACCESS_WRITE permission
 *
 * @param object [IN]  TEE_ObjectHandle to be truncated
 * @param size [IN]  The new length of the data stream, the size does not exceed 4096 bytes
 *
 * @return TEE_SUCCESS Indicates that the function was executed successfully
 * @return TEE_ERROR_STORAGE_NO_SPACE There is not enough space to perform the operation
 */
TEE_Result TEE_TruncateObjectData(TEE_ObjectHandle object, size_t size);

/*
 * Set the data stream position pointed to by TEE_ObjectHandle, and set the data stream position to:
 * start position + offset
 * The parameter whence controls the starting position of the offset, the value can choose in TEE_Whence,
 * and the meaning is as follows:
 *     TEE_DATA_SEEK_SET, the starting position of the data stream offset is the file header, which is 0
 *     TEE_DATA_SEEK_CUR, the starting position of the data stream offset is the current position
 *     TEE_DATA_SEEK_END, the starting position of the data stream offset is the end of the file
 * When the parameter offset is a positive number, it is offset backward, and when it is negative, it is offset forward.
 *
 * @param object [IN]  TEE_ObjectHandle to be set
 * @param offset [IN]  The size of the data stream position movement, the size does not exceed 4096 bytes
 * @param whence [IN]  The initial position of the data stream offset
 *
 * @return TEE_SUCCESS Indicates that the function was executed successfully
 * @return TEE_ERROR_OVERFLOW  The operation causes the value of the position indicator to exceed its
 * system limit TEE_DATA_MAX_POSITION
 */
TEE_Result TEE_SeekObjectData(TEE_ObjectHandle object, int32_t offset, TEE_Whence whence);

/*
 * Close the opened TEE_ObjectHandle and delete the object. The object must be a persistent object
 * and must have been opened with TEE_DATA_FLAG_ACCESS_WRITE_META permission
 *
 * @param object [IN]  TEE_ObjectHandle to be closed and deleted
 *
 * @return void
 */
void TEE_CloseAndDeletePersistentObject(TEE_ObjectHandle object);

/*
 * Synchronize the opened TEE_ObjectHandle, and synchronize the corresponding security attribute files to the disk
 *
 * @param object [IN]  TEE_ObjectHandle to be synchronized
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 */
TEE_Result TEE_SyncPersistentObject(TEE_ObjectHandle object);

/*
 * Change the object identifier, the TEE_ObjectHandle must be opened with TEE_DATA_FLAG_ACCESS_WRITE_META permission
 *
 * @param ojbect [IN/OUT]  The object handle to be modified
 * @param newObjectID [IN]  New object identifier
 * @param newObjectIDLen [IN]  New object identifier length
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 */
TEE_Result TEE_RenamePersistentObject(TEE_ObjectHandle object, void *newObjectID, size_t newObjectIDLen);

/*
 * Allocate the handle of an uninitialized object enumerator
 *
 * @param object [OUT]  Pointer to the handle of the newly created object enumerator
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 * @return TEE_ERROR_OUT_OF_MEMORY No enough memory to allocate
 */
TEE_Result TEE_AllocatePersistentObjectEnumerator(TEE_ObjectEnumHandle *obj_enumerator);

/*
 * Release a object enumerator handle that has allocated. The handle becomes invalid after the function is called,
 * and all allocated are released. Use it in pair with TEE_AllocatePersistentObjectEnumerator
 *
 * @param object [IN]  TEE_ObjectEnumHandle to be released
 *
 * @return void
 */
void TEE_FreePersistentObjectEnumerator(TEE_ObjectEnumHandle obj_enumerator);

/*
 * Reset the temporary object enumerator to its initial state, that is, the state just after the allocate
 *
 * @param object [IN]  TEE_ObjectEnumHandle of the object enumerator to be reset
 *
 * @return void
 */
void TEE_ResetPersistentObjectEnumerator(TEE_ObjectEnumHandle obj_enumerator);

/*
 * Start enumerating all objects in a given storage space, the information of the object can be obtained
 * through the TEE_GetNextPersistentObject function
 *
 * @param object [IN]  TEE_ObjectEnumHandle of the allocated object enumerator
 * @param storageID [IN]  Correspond to a separate storage space for each application, the value is
 * Object_Storage_Constants, currently only supports TEE_STORAGE_PRIVATE
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 * @return TEE_ITEM_NOT_FOUND storageID is not TEE_STORAGE_PRIVATE or there is no object in the storage space
 */
TEE_Result TEE_StartPersistentObjectEnumerator(TEE_ObjectEnumHandle obj_enumerator, uint32_t storage_id);

/*
 * Get the next object in the object enumerator, and return the object's TEE_ObjectInfo, objectID,
 * objectIDLen information
 *
 * @param object [IN]  TEE_ObjectEnumHandle of the initialized object enumerator
 * @param objectInfo [OUT]  Pointer to the structure used to store the obtained TEE_ObjectInfo
 * @param objectInfo [OUT]  Pointer to a buffer, used to store the obtained objectID
 * @param objectInfo [OUT]  Used to store the obtained objectIDLen
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 * @return TEE_ITEM_NOT_FOUND  The enumerator has no object or the enumerator has not been initialized
 */
TEE_Result TEE_GetNextPersistentObject(TEE_ObjectEnumHandle obj_enumerator,
    TEE_ObjectInfo *object_info, void *object_id, size_t *object_id_len);

#if defined(API_LEVEL) && (API_LEVEL >= 2)

/*
 * Close the opened TEE_ObjectHandle and delete the object. The object must be a persistent object
 * and must have been opened with TEE_DATA_FLAG_ACCESS_WRITE_META permission
 *
 * @param object [IN]  TEE_ObjectHandle to be closed and deleted
 *
 * @return TEE_SUCCESS  Indicates that the function was executed successfully
 * @return TEE_ERROR_STORAGE_NOT_AVAILABLE Cannot access the storage area where the file is located
 */
TEE_Result TEE_CloseAndDeletePersistentObject1(TEE_ObjectHandle object);

#endif // API_LEVEL
#endif
