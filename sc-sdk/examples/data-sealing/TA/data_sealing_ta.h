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
 * Description: data-sealing-ta.h
 * Create: 2022-04-15
 */

#ifndef __DATA_SEALING_TA_H
#define __DATA_SEALING_TA_H

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#include "tee_defines.h"

#define FILE_NAME_LEN_MAX      255
#define PARAMS_IDX0            0            // Subscript index 0 of the params parameter.
#define PARAMS_IDX1            1            // Subscript index 1 of the params parameter.
#define PARAMS_IDX2            2            // Subscript index 2 of the params parameter.
#define PARAMS_IDX3            3            // Subscript index 3 of the params parameter.
#define PARAMS_SIZE            4            // Size of the params array.
#define UNSIGN_INT_MASK        0xffffffff

typedef enum
{
	CMD_DATA_SEALING_READ_FILE = 0X1,
	CMD_DATA_SEALING_WRITE_FILE,
} DataSealingCMD;

enum Data_Flag_Constants
{
	TEE_DATA_FLAG_CREATE = 0x00000200,          // Not used.
	TEE_DATA_FLAG_EXCLUSIVE = 0x00000400,       // Protect existing data files with the same name. If no file with the same name exist, create a data file. If a file with the same name exists, an error is reported.
	TEE_DATA_FLAG_AES256 = 0x10000000,          // If 'bit28' is set to '1', the algorithm is AES256. If 'bit28' is set to '0', the algorithm is AES128.
};

enum Object_Storage_Constants
{
	TEE_OBJECT_STORAGE_PRIVATE = 0x00000001,    // Correspond to private storage space of each application.
};

extern TEE_Result
TEE_CreatePersistentObject(uint32_t storageID, const void* objectID, size_t objectIDLen, uint32_t flags,
	TEE_ObjectHandle attributes, const void* initialData, size_t initialDataLen,
	TEE_ObjectHandle* object);

extern TEE_Result TEE_OpenPersistentObject(uint32_t storageID, const void* objectID, size_t objectIDLen, uint32_t flags,
	TEE_ObjectHandle* object);

extern TEE_Result TEE_ReadObjectData(TEE_ObjectHandle object, void* buffer, size_t size, uint32_t* count);

extern TEE_Result TEE_WriteObjectData(TEE_ObjectHandle object, const void* buffer, size_t size);

#endif
