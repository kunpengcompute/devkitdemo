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
 * Description: data-sealing-ca.c
 * Create: 2022-04-15
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <limits.h>
#include "tee_client_api.h"
#include "securec.h"
#include "data_sealing_ca.h"

enum DataFlagConstants
{
	TEE_DATA_FLAG_ACCESS_READ = 0x00000001,
	TEE_DATA_FLAG_ACCESS_WRITE = 0x00000002,
	TEE_DATA_FLAG_CREATE = 0x00000200,
	TEE_DATA_FLAG_EXCLUSIVE = 0x00000400,
};

/*************************************************
Description: 操作类型枚举
*************************************************/
enum OperationType
{
	ENCRYPT,    //加密操作
	DECRYPT,    //解密操作
	UNKNOWN     //未知的操作
};

/*************************************************
Description: 输入的参数结构体
*************************************************/
typedef struct
{
	enum OperationType operationType;    // 操作类型
	char* key;                           // encrypt 或 decrypt 时指定的key字符串
	char* filePath;                      // 被加密的文件路径
	char* output;                        // 被解密的文件输出路径
} ParameterT;

TEEC_Context g_context;
TEEC_Session g_session;

static errno_t HexStrToLong(const char* src, int count, long int* res)
{
	char dest[9];
	char* endptr;
	errno_t err;
	err = strncpy_s(dest, 9, src, count);
	if (err != EOK)
	{
		return err;
	}
	*res = strtol(dest, &endptr, 16);
	return EOK;
}

static errno_t GetDataSealingUUID(TEEC_UUID* dataSealingUUID)
{
	errno_t err;
	long int res;
	int i;
	char* src = DATA_SEALING_TA_UUID;
	err = HexStrToLong(src, 8, &res);
	if (err != EOK)
	{
		return err;
	}
	dataSealingUUID->timeLow = (uint32_t)res;
	err = HexStrToLong(src + 9, 4, &res);
	if (err != EOK)
	{
		return err;
	}
	dataSealingUUID->timeMid = (uint16_t)res;
	err = HexStrToLong(src + 14, 4, &res);
	if (err != EOK)
	{
		return err;
	}
	dataSealingUUID->timeHiAndVersion = (uint16_t)res;
	for (i = 0; i < 2; i++)
	{
		err = HexStrToLong(src + 19 + i * 2, 2, &res);
		if (err != EOK)
		{
			return err;
		}
		dataSealingUUID->clockSeqAndNode[i] = (uint8_t)res;
	}
	for (i = 0; i < 6; i++)
	{
		err = HexStrToLong(src + 24 + i * 2, 2, &res);
		if (err != EOK)
		{
			return err;
		}
		dataSealingUUID->clockSeqAndNode[i + 2] = (uint8_t)res;
	}
	return EOK;
}

/* initialize context and opensession */
static TEEC_Result TeecInit(void)
{
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin = 0;
	result = TEEC_InitializeContext(NULL, &g_context);
	if (result != TEEC_SUCCESS)
	{
		TEEC_Error("teec initial failed.");
		return result;
	}
	operation.started = 1;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE,
		TEEC_NONE,
		TEEC_MEMREF_TEMP_INPUT,
		TEEC_MEMREF_TEMP_INPUT);
	char dataSealingTAPath[PATH_MAX];
	int ret = sprintf(dataSealingTAPath, "%s%s%s", DATA_SEALING_TA_DIR_NAME, DATA_SEALING_TA_UUID, DATA_SEALING_TA_SUFFIX);
	if (ret <= 0)
	{
		printf("LOG: Failed to get TA path.\n");
		TEEC_FinalizeContext(&g_context);
		return TEEC_ERROR_GENERIC;
	}
	g_context.ta_path = (uint8_t*)dataSealingTAPath;
	TEEC_UUID dataSealingUUID;
	if (GetDataSealingUUID(&dataSealingUUID) != EOK)
	{
		printf("LOG: Failed to get UUID.\n");
		TEEC_FinalizeContext(&g_context);
		return TEEC_ERROR_GENERIC;
	}
	g_context.ta_path = (uint8_t*)dataSealingTAPath;
	result = TEEC_OpenSession(&g_context, &g_session, &dataSealingUUID, TEEC_LOGIN_IDENTIFY, NULL, &operation, &origin);
	if (result != TEEC_SUCCESS)
	{
		printf("LOG: Open session failed: result:%d, orgin: %d.\n", result, origin);
		TEEC_FinalizeContext(&g_context);
		return result;
	}
	TEEC_Debug("teec initialize context andopen session success, session id: 0x%x, service id: 0x%x, context 0x%x.",
		g_session.session_id, g_session.service_id, g_session.context);
	TEEC_Debug("teec init OK.");
	return result;
}

static void TeecClose(void)
{
	TEEC_CloseSession(&g_session);
	TEEC_FinalizeContext(&g_context);
	TEEC_Debug("teec uninit OK.");
}

static TEEC_Result
DataSealingReadFile(IN const char* path, IN const uint32_t mode, OUT char* readBuff, OUT size_t* readSize)
{
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin = 0;
	printf("LOG: DATA_SEALING READ FILE is running.\n");
	if (path == NULL)
	{
		printf("LOG: DATA_SEALING READ FILE failed, path is null.\n");
		TEEC_Error("DATA_SEALING READ FILE failed, path is null.\n");
		return TEEC_ERROR_BAD_STATE;
	}
	/* First, Check Init is ok or not */
	if ((g_context.fd == 0) || (g_session.session_id == 0))
	{
		printf("LOG: DATA_SEALING READ FILE failed, Please Init Storage Service.\n");
		TEEC_Error("DATA_SEALING READ FILE failed, Please Init Storage Service.\n");
		return TEEC_ERROR_BAD_STATE;
	}
	/* Then, Invoke Command */
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
		TEEC_VALUE_INPUT,
		TEEC_MEMREF_TEMP_INOUT,
		TEEC_VALUE_OUTPUT);
	operation.params[PARAMS_IDX0].tmpref.buffer = (void*)path;
	operation.params[PARAMS_IDX0].tmpref.size = strlen(path);
	operation.params[PARAMS_IDX1].value.a = mode;
	operation.params[PARAMS_IDX1].value.b = TEEC_VALUE_UNDEF;
	operation.params[PARAMS_IDX2].tmpref.buffer = (void*)readBuff;
	operation.params[PARAMS_IDX2].tmpref.size = (uint32_t)(*readSize);
	operation.params[PARAMS_IDX3].value.a = SECURE_STORAGE_INVALID_FD;
	operation.params[PARAMS_IDX3].value.b = TEEC_VALUE_UNDEF;
	result = TEEC_InvokeCommand(&g_session, CMD_DATA_SEALING_READ_FILE, &operation, &origin);
	if (result != TEEC_SUCCESS)
	{
		printf("LOG: ReadFile Failed:0x%x, origin:0x%x.\n", result, origin);
		TEEC_Error("ReadFile Failed:0x%x, origin:0x%x.\n", result, origin);
		return result;
	}
	*readSize = operation.params[PARAMS_IDX2].tmpref.size;
	printf("LOG: DataSealing read file success: fd=%d, read count=%zu.\n", operation.params[PARAMS_IDX3].value
		.a, *readSize);
	return TEEC_SUCCESS;
}

static TEEC_Result
DataSealingWriteFile(IN const char* path, IN const uint32_t mode, IN const char* writeBuff, IN const size_t writeSize)
{
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin = 0;
	printf("LOG: DATA_SEALING WRITE FILE is running.\n");
	if (path == NULL)
	{
		printf("LOG: DATA_SEALING WRITE FILE failed, path is null.\n");
		TEEC_Error("DATA_SEALING WRITE FILE failed, path is null.\n");
		return TEEC_ERROR_BAD_STATE;
	}
	if ((mode & TEE_DATA_FLAG_CREATE) && (mode & TEE_DATA_FLAG_EXCLUSIVE))
	{
		printf("LOG: DATA_SEALING WRITE FILE failed, mode error, CREATE&EXCLUSIVE canot be concurrence.\n");
		TEEC_Error("DATA_SEALING WRITE FILE failed, mode error, CREATE&EXCLUSIVE canot be concurrence.\n");
		return TEEC_ERROR_BAD_STATE;
	}
	/* First, Check Init is ok or not */
	if ((g_context.fd == 0) || (g_session.session_id == 0))
	{
		printf("LOG: DATA_SEALING WRITE FILE failed, Please Init Storage Service.\n");
		TEEC_Error("DATA_SEALING WRITE FILE failed, Please Init Storage Service.\n");
		return TEEC_ERROR_BAD_STATE;
	}
	/* Then, Invoke Command */
	operation.started = 1;
	operation.cancel_flag = 0;
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
		TEEC_VALUE_INPUT,
		TEEC_MEMREF_TEMP_INPUT,
		TEEC_VALUE_OUTPUT);
	operation.params[PARAMS_IDX0].tmpref.buffer = (void*)path;
	operation.params[PARAMS_IDX0].tmpref.size = strlen(path);
	operation.params[PARAMS_IDX1].value.a = mode;
	operation.params[PARAMS_IDX1].value.b = TEEC_VALUE_UNDEF;
	operation.params[PARAMS_IDX2].tmpref.buffer = (void*)writeBuff;
	operation.params[PARAMS_IDX2].tmpref.size = (uint32_t)writeSize;
	operation.params[PARAMS_IDX3].value.a = SECURE_STORAGE_INVALID_FD;
	operation.params[PARAMS_IDX3].value.b = TEEC_VALUE_UNDEF;
	result = TEEC_InvokeCommand(&g_session, CMD_DATA_SEALING_WRITE_FILE, &operation, &origin);
	if (result != TEEC_SUCCESS)
	{
		printf("LOG: WriteFile Failed:0x%x, origin:0x%x.\n", result, origin);
		TEEC_Error("WriteFile Failed:0x%x, origin:0x%x.\n", result, origin);
		return result;
	}
	printf("LOG: DataSealing write file success: fd=%d.\n", operation.params[PARAMS_IDX3].value.a);
	return TEEC_SUCCESS;
}

static bool CharInStr(const char targetChar, const char* baseStr)
{
	int i;
	for (i = 0; i < strlen(baseStr); i++)
	{
		if (baseStr[i] == targetChar)
		{
			return true;
		}
	}
	return false;
}

static bool ValidateString(const char* str, const char* baseStr)
{
	int i;
	for (i = 0; i < strlen(str); i++)
	{
		if (!CharInStr(str[i], baseStr))
		{
			return false;
		}
	}
	return true;
}

/*************************************************
Function: CheckKey
Description: 校验Key的合法性
Calls: ValidateString()
Called By: GetParameter()
Input: key: encrypt 或 decrypt 时指定的key字符串
Output: none
Return: 符合要求返回 true 否则返回 false
Others: 合规的key字符串仅由字母和数字组成,且长度为 0 < KEY_LENGTH <= 128
*************************************************/
static bool CheckKey(char* key)
{
	return (key != NULL && strlen(key) <= KEY_LENGTH_MAX && ValidateString(key, BASE_STR));
}

/*************************************************
Function: ReadFile
Description: 读取文件
Calls: fopen(), fread(), fclose()
Called By: Encrypt()
Input: filePath: 将要被读取的文件路径
Output: fileContent: 输出被读取的文件BUFF
Return: 读取失败返回 0,读取成功则返回实际读取的文件大小
Others: 如果实际读取的文件大小大于 FILE_SIZE_MAX 则读取失败
*************************************************/
static int ReadFile(const char* filePath, char* fileContent)
{
	FILE* pFile = fopen(filePath, "r");
	if (pFile == NULL)
	{
		return 0;
	}
	int readSize = fread(fileContent, 1, FILE_SIZE_MAX + 1, pFile);
	if (readSize > FILE_SIZE_MAX || readSize == 0)
	{
		printf("The maximum file size is 4096B.\n");
		fclose(pFile);
		return 0;
	}
	fclose(pFile);
	return readSize;
}

/*************************************************
Function: WriteFile
Description: 写入文件
Calls: fopen(), fwrite(), fclose()
Called By: Decrypt()
Input: filePath: 写入文件的路径, fileContent: 写入文件的内容, fileContentLen: 写入文件的内容长度
Output: none
Return: 写入失败返回 0,写入成功则返回实际写入的文件大小
Others: none
*************************************************/
static int WriteFile(const char* filePath, const char* fileContent, size_t fileContentLen)
{
	FILE* pFile = fopen(filePath, "w");
	if (pFile == NULL)
	{
		return 0;
	}
	size_t writeSize = fwrite(fileContent, fileContentLen, 1, pFile);
	fclose(pFile);
	return writeSize;
}

/*************************************************
Function: Usage
Description: 使用手册
Calls: printf(), exit()
Called By: GetParameter(), PerformAction(), Encrypt(), Decrypt()
Input: status 退出状态[请使用 stdlib.h 中定义的 EXIT_FAILURE 或 EXIT_SUCCESS 作为函数的入参值]
Output: none
Return: none
Others: none
*************************************************/
static void Usage(int status)
{
	if (status != EXIT_SUCCESS)
	{
		printf("Try '/vendor/bin/data-sealing --help' for more information.\n");
	}
	else
	{
		printf("Usage: /vendor/bin/data-sealing [TYPE OPTION] [REQUIRED_ARGUMENT]\n"
		       "Examples:\n"
		       "\t/vendor/bin/data-sealing encrypt -k <key> -f <path>\t# Encrypt data.\n"
		       "\t/vendor/bin/data-sealing decrypt -k <key> -o <path>\t# Decrypt data.\n"
		       "TYPE OPTION:\n"
		       "\tencrypt\t\tEncrypt operation.\n"
		       "\tdecrypt\t\tDecrypt operation.\n"
		       "REQUIRED_ARGUMENT:\n"
		       "\t-k, --key <key>\t\tThe key string specified when encrypting or decrypting [0 < KEY_LENGTH <= 128, letters and numbers only].\n"
		       "\t-f, --file-path <path>\t\tEncrypted data file path [0 < FILE_SIZE <= 4096B, strong keys are recommended].\n"
		       "\t-o, --output <path>\t\tDecrypted data output path.\n"
		       "\t-h, --help\t\tDisplay this help and exit.\n");
	}
	exit(status);
}

/*************************************************
Function: Encrypt
Description: 执行加密操作
Calls: ReadFile(), DataSealingWriteFile(), TeecClose(), fprintf(), Usage(), exit()
Called By: PerformAction()
Input: key 加密时指定的key字符串, filePath 被加密的文件路径
Output: none
Return: none
Others: none
*************************************************/
static void Encrypt(const char* key, const char* filePath)
{
	TEEC_Result result;
	char fileContent[FILE_SIZE_MAX + 1];
	int fileSize = ReadFile(filePath, fileContent);
	if (fileSize > 0)
	{
		result = DataSealingWriteFile(key, (uint32_t)DATA_SEALING_MODE_WRITE, fileContent, fileSize);
		if (result != TEEC_SUCCESS)
		{
			TeecClose();
			fprintf(stderr, "Encrypt file Failed!\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			TeecClose();
			printf("Successfully performed file encryption operation.\n");
			exit(EXIT_SUCCESS);
		}
	}
	else
	{
		TeecClose();
		fprintf(stderr, "Check if the file is legal!\n");
		Usage(EXIT_FAILURE);
	}
}

/*************************************************
Function: Decrypt
Description: 执行解密操作
Calls: DataSealingReadFile(), WriteFile(), TeecClose(), fprintf(), Usage(), exit()
Called By: PerformAction()
Input: key 加密时指定的key字符串, outputFilePath 被解密的文件输出路径
Output: none
Return: none
Others: none
*************************************************/
static void Decrypt(const char* key, const char* outputFilePath)
{
	TEEC_Result result;
	char fileContent[FILE_SIZE_MAX];
	size_t readSize = sizeof(fileContent);
	result = DataSealingReadFile(key, TEE_DATA_FLAG_ACCESS_READ, fileContent, &readSize);
	if (result != TEEC_SUCCESS)
	{
		TeecClose();
		fprintf(stderr, "Decrypt file Failed!\n");
		Usage(EXIT_FAILURE);
	}
	else
	{
		if (!WriteFile(outputFilePath, fileContent, readSize))
		{
			TeecClose();
			fprintf(stderr, "Decryption is successful, but there is an exception when writing out [please check if the path to the outgoing file is legal]!\n");
			exit(EXIT_FAILURE);
		}
		TeecClose();
		printf("Successfully performed file decryption operation.\n");
		exit(EXIT_SUCCESS);
	}
}

/*************************************************
Function: PerformAction
Description: 执行的动作 encrypt 或者 decrypt
Calls: TeecInit(), TeecClose(), Encrypt(), Decrypt(), fprintf(), Usage(), exit()
Called By: main()
Input: parameterT: 输入的参数结构体
Output: none
Return: void
Others: none
*************************************************/
static void PerformAction(ParameterT parameterT)
{
	TEEC_Result result = TeecInit();
	if (result != TEEC_SUCCESS)
	{
		TeecClose();
		fprintf(stderr, "TeecInit Failed!\n");
		exit(EXIT_FAILURE);
	}
	switch (parameterT.operationType)
	{
	case ENCRYPT:
		Encrypt(parameterT.key, parameterT.filePath);
		break;
	case DECRYPT:
		Decrypt(parameterT.key, parameterT.output);
		break;
	case UNKNOWN:
		TeecClose();
		fprintf(stderr, "Please make sure your operation type is encrypt or decrypt.\n");
		Usage(EXIT_FAILURE);
		break;
	default:
		TeecClose();
		Usage(EXIT_FAILURE);
	}
}

/*************************************************
Function: GetParameter
Description: 获取输入的参数
Calls: getopt_long()
Called By: main()
Input: argc: 输入的参数个数, argv: 输入参数的字符串数组
Output: parameterT: ParameterT 参数结构体
Return: void
Others: none
*************************************************/
static void GetParameter(int argc, char** argv, ParameterT* parameterT)
{
	int c;    // getopt_long() 返回的选项元素值
	// 根据输入的选项元素分别获取对应的值
	while (true)
	{
		int option_index = 0;
		static struct option const longOptions[] = {
			{ "key", required_argument, NULL, 'k' },
			{ "file-path", required_argument, NULL, 'f' },
			{ "output", required_argument, NULL, 'o' },
			{ "help", no_argument, NULL, 'h' },
			{ NULL, 0, NULL, 0 }
		};
		c = getopt_long(argc, argv, "k:f:o:h", longOptions, &option_index);
		if (c == -1)
		{
			break;
		}
		switch (c)
		{
		case 'k':
			if (!CheckKey(optarg))
			{
				fprintf(stderr, "Check if the key is legal!\n");
				Usage(EXIT_FAILURE);
			}
			parameterT->key = optarg;
			break;
		case 'f':
			parameterT->filePath = optarg;
			break;
		case 'o':
			parameterT->output = optarg;
			break;
		case 'h':
			Usage(EXIT_SUCCESS);
		default:
			fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	// 仅执行 /vendor/bin/data-sealing 时,返回详细de帮助信息.
	if (argc == 1)
	{
		Usage(EXIT_SUCCESS);
	}
	// 根据输入的非选项元素判断操作类型[未知的操作类型及多个重复非选项元素时,则操作类型为 UNKNOWN]
	if ((argc - optind) == 1)
	{
		if (strcmp("encrypt", argv[optind]) == 0)
		{
			parameterT->operationType = ENCRYPT;
		}
		else if (strcmp("decrypt", argv[optind]) == 0)
		{
			parameterT->operationType = DECRYPT;
		}
		else
		{
			parameterT->operationType = UNKNOWN;
		}
	}
	else
	{
		parameterT->operationType = UNKNOWN;
	}
}

ParameterT parameterT;

int main(int argc, char** argv)
{
	GetParameter(argc, argv, &parameterT);
	PerformAction(parameterT);
}
