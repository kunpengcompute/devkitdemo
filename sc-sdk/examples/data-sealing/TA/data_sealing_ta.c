/*
 * Copyright 2020 Huawei Technologies Co., Ltd
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
 * Description: data-sealing-ta.c
 * Create: 2022-04-15
 */

#include <string.h>
#include "data_sealing_ta.h"
#include "tee_ext_api.h"
#include "tee_log.h"
#include "tee_mem_mgmt_api.h"
#include "tee_object_api.h"

struct session_identity
{
	uint32_t len;
	char val[1];
};

TEE_Result TA_CreateEntryPoint(void)
{
	SLogTrace("TA_CreateEntryPoint");
	const char* caPath = "/vendor/bin/data-sealing";
	if (addcaller_ca_exec(caPath, "root") != TEE_SUCCESS)
	{
		SLogError("TA_CreateEntryPoint: add caller for ca exec failed.");
		return TEE_ERROR_GENERIC;
	}
	SLogTrace("TA_CreateEntryPoint: AddCaller_CA_exec ok.");
	return TEE_SUCCESS;
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t paramTypes, TEE_Param params[PARAMS_SIZE], void** sessionContext)
{
	struct session_identity* identity = NULL;
	uint32_t pkgNameLen;
	SLogTrace("DataSealing open session entry point.");
	if (!(TEE_PARAM_TYPE_MEMREF_INPUT == TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX3)
		  || TEE_PARAM_TYPE_MEMREF_OUTPUT == TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX3)
		  || TEE_PARAM_TYPE_MEMREF_INOUT == TEE_PARAM_TYPE_GET(paramTypes, PARAMS_IDX3)))
	{
		SLogError("Bad parameters (paramTypes, 3)");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	pkgNameLen = params[PARAMS_IDX3].memref.size;
	if ((pkgNameLen == 0) || (pkgNameLen >= FILE_NAME_LEN_MAX))
	{
		SLogError("Invalid size of package name len login info!");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	identity = (struct session_identity*)TEE_Malloc((sizeof(struct session_identity) + pkgNameLen), 0);
	if (identity == NULL)
	{
		SLogError("Failed to allocate mem for session_identity.");
		return TEE_ERROR_GENERIC;
	}
	identity->len = pkgNameLen;
	TEE_MemMove((char*)(identity->val), params[PARAMS_IDX3].memref.buffer, params[PARAMS_IDX3].memref.size);
	*sessionContext = (void*)identity;
	SLogTrace("DataSealing open session entry point ok.");

	return TEE_SUCCESS;
}

static TEE_Result
TA_DataSealingReadFile(IN uint32_t paramTypes, IN TEE_Param params[PARAMS_SIZE], IN struct session_identity* identity)
{
	SLogTrace("TA DataSealing read file is running.");
	if (identity == NULL)
	{
		SLogError("bad parameters.");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	if (!check_param_type(paramTypes, TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_VALUE_INPUT,
		TEE_PARAM_TYPE_MEMREF_INOUT, TEE_PARAM_TYPE_VALUE_OUTPUT))
	{
		SLogError("Bad expected parameters types.");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	char* path = NULL;
	char* readBuff = NULL;
	uint32_t pathLen, flags, readLen, realReadLen;
	TEE_ObjectHandle object = NULL;
	TEE_Result ret;

	path = params[PARAMS_IDX0].memref.buffer;
	pathLen = params[PARAMS_IDX0].memref.size;
	readBuff = params[PARAMS_IDX2].memref.buffer;
	readLen = params[PARAMS_IDX2].memref.size;
	flags = params[PARAMS_IDX1].value.a;
	flags = flags & (~TEE_DATA_FLAG_AES256);

	ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, path, pathLen, flags, &object);
	if (ret != TEE_SUCCESS)
	{
		SLogError("open failed.");
		return ret;
	}

	ret = TEE_ReadObjectData(object, (void*)readBuff, readLen, &realReadLen);
	if (ret != TEE_SUCCESS)
	{
		SLogError("read failed.");
		goto clean;
	}

	params[PARAMS_IDX2].memref.size = realReadLen;
	params[PARAMS_IDX3].value.a = (uint64_t)object->dataPtr & UNSIGN_INT_MASK;
	SLogTrace("TA DataSealing read file success, fd is: 0x%x.", object->dataPtr);

clean:
	TEE_CloseObject(object);
	return ret;
}

static TEE_Result
TA_DataSealingWriteFile(uint32_t paramTypes, IN TEE_Param params[PARAMS_SIZE], IN struct session_identity* identity)
{
	SLogTrace("TA DataSealing write file is running.");
	if (identity == NULL)
	{
		SLogError("bad parameters.");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	if (!check_param_type(paramTypes, TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_VALUE_INPUT,
		TEE_PARAM_TYPE_MEMREF_INPUT, TEE_PARAM_TYPE_VALUE_OUTPUT))
	{
		SLogError("Bad expected parameters types.");
		return TEE_ERROR_BAD_PARAMETERS;
	}
	char* path = NULL;
	uint32_t pathLen, flags, writeLen;
	char* writeBuff = NULL;
	TEE_ObjectHandle object = NULL;
	TEE_Result ret;
	TEE_ObjectInfo objectinfo;
	path = params[PARAMS_IDX0].memref.buffer;
	pathLen = params[PARAMS_IDX0].memref.size;
	writeBuff = params[PARAMS_IDX2].memref.buffer;
	writeLen = params[PARAMS_IDX2].memref.size;
	flags = params[PARAMS_IDX1].value.a;
	flags = (flags & (~TEE_DATA_FLAG_AES256));
	if (flags & TEE_DATA_FLAG_EXCLUSIVE)
	{
		ret = TEE_CreatePersistentObject(TEE_OBJECT_STORAGE_PRIVATE, path, pathLen,
			flags, TEE_HANDLE_NULL, NULL, 0, &object);
	}
	else if (flags & TEE_DATA_FLAG_CREATE)
	{
		ret = TEE_CreatePersistentObject(TEE_OBJECT_STORAGE_PRIVATE, path, pathLen,
			flags & (~TEE_DATA_FLAG_CREATE), TEE_HANDLE_NULL, NULL, 0, &object);
	}
	else
	{
		ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, path, pathLen, flags, &object);
	}

	if (ret != TEE_SUCCESS)
	{
		SLogError("create or open failed");
		return ret;
	}
	ret = TEE_WriteObjectData(object, (void*)writeBuff, writeLen);
	if (ret != TEE_SUCCESS)
	{
		SLogError("write failed.");
		goto clean;
	}
	params[PARAMS_IDX3].value.a = (uint64_t)object->dataPtr & UNSIGN_INT_MASK;
	SLogTrace("TA DataSealing write file, fd is:0x%x.", object->dataPtr);
	TEE_GetObjectInfo(object, &objectinfo);
	if ((objectinfo.dataSize != writeLen) || (objectinfo.dataPosition != writeLen))
	{
		SLogError("after write, info file: len=%d, pos=%d\n", objectinfo.dataSize, objectinfo.dataPosition);
		goto clean;
	}
	SLogTrace("TA DataSealing check object ok.");
clean:
	TEE_CloseObject(object);
	return ret;
}

TEE_Result TA_InvokeCommandEntryPoint(void* context, uint32_t cmdId, uint32_t paramTypes, TEE_Param params[4])
{
	struct session_identity* identity = (struct session_identity*)context;
	TEE_Result ret;
	SLogTrace("DataSealing ta invoke command entry point");
	switch (cmdId)
	{
	case CMD_DATA_SEALING_READ_FILE:
		ret = TA_DataSealingReadFile(paramTypes, params, identity);
		break;
	case CMD_DATA_SEALING_WRITE_FILE:
		ret = TA_DataSealingWriteFile(paramTypes, params, identity);
		break;
	default:
		SLogError("Unknow CMD ID: %d", cmdId);
		ret = TEE_FAIL;
		break;
	}
	if (ret != TEE_SUCCESS)
	{
		SLogError("DataSealing ta invoke command entry point failed 0x%x. cmd_id is %d.", ret, cmdId);
	}
	return ret;
}

void TA_CloseSessionEntryPoint(void* context)
{
	SLogTrace("TA_CloseSessionEntryPoint");
	if (context != NULL)
	{
		TEE_Free(context);
	}
}

void TA_DestoryEntryPoint(void)
{
	SLogTrace("TA_DestoryEntryPoint");
}
