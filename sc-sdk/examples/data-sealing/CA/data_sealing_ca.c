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
Description: Operation types
*************************************************/
enum OperationType
{
	ENCRYPT,    //Encryption operation.
	DECRYPT,    //Decryption operation.
	UNKNOWN     //Unknown operation.
};

/*************************************************
Description: Input parameter structure
*************************************************/
typedef struct
{
	enum OperationType operationType;    // Operation type.
	char* key;                           // Key character string specified during encryption or decryption.
	char* filePath;                      // Encrypted file path.
	char* output;                        // Output path of the decrypted file.
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

/* initialize context and opensession. */
static TEEC_Result TeecInit(void)
{
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin = 0;
	result = TEEC_InitializeContext(NULL, &g_context);
	if (result != TEEC_SUCCESS)
	{
		TEEC_Error("TEEC initial failed.");
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
	TEEC_Debug("TEEC initialize context andopen session success, session id: 0x%x, service id: 0x%x, context 0x%x.",
		g_session.session_id, g_session.service_id, g_session.context);
	TEEC_Debug("TEEC init OK.");
	return result;
}

static void TeecClose(void)
{
	TEEC_CloseSession(&g_session);
	TEEC_FinalizeContext(&g_context);
	TEEC_Debug("TEEC uninit OK.");
}

static TEEC_Result
DataSealingReadFile(IN const char* path, IN const uint32_t mode, OUT char* readBuff, OUT size_t* readSize)
{
	TEEC_Operation operation;
	TEEC_Result result;
	uint32_t origin = 0;
	printf("LOG: DataSealing read file is running.\n");
	if (path == NULL)
	{
		printf("LOG: DataSealing read file failed, path is null.\n");
		TEEC_Error("DataSealing read file failed, path is null.\n");
		return TEEC_ERROR_BAD_STATE;
	}
	/* First, Check Init is ok or not. */
	if ((g_context.fd == 0) || (g_session.session_id == 0))
	{
		printf("LOG: DataSealing read file failed, Please Init Storage Service.\n");
		TEEC_Error("DataSealing read file failed, Please Init Storage Service.\n");
		return TEEC_ERROR_BAD_STATE;
	}
	/* Then, Invoke Command. */
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
	printf("LOG: DataSealing write file is running.\n");
	if (path == NULL)
	{
		printf("LOG: DataSealing write file failed, path is null.\n");
		TEEC_Error("DataSealing write file failed, path is null.\n");
		return TEEC_ERROR_BAD_STATE;
	}
	if ((mode & TEE_DATA_FLAG_CREATE) && (mode & TEE_DATA_FLAG_EXCLUSIVE))
	{
		printf("LOG: DataSealing write file failed, mode error, CREATE & EXCLUSIVE canot be concurrence.\n");
		TEEC_Error("DataSealing write file failed, mode error, CREATE & EXCLUSIVE canot be concurrence.\n");
		return TEEC_ERROR_BAD_STATE;
	}
	/* First, Check Init is ok or not. */
	if ((g_context.fd == 0) || (g_session.session_id == 0))
	{
		printf("LOG: DataSealing write file failed, Please Init Storage Service.\n");
		TEEC_Error("DataSealing write file failed, Please Init Storage Service.\n");
		return TEEC_ERROR_BAD_STATE;
	}
	/* Then, Invoke Command. */
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
Description: Verify the key validity.
Calls: ValidateString()
Called By: GetParameter()
Input: key: Key character string specified during encryption or decryption.
Output: none
Return: ‘true’ is returned if the requirements are met. Otherwise, ‘false’ is returned.
Others: A valid key string consists of only letters and digits, and the length is: 0 < KEY_LENGTH <= 128.
*************************************************/
static bool CheckKey(char* key)
{
	return (key != NULL && strlen(key) <= KEY_LENGTH_MAX && ValidateString(key, BASE_STR));
}

/*************************************************
Function: ReadFile
Description: Read a file.
Calls: fopen(), fread(), fclose()
Called By: Encrypt()
Input: filePath: Path of the file to be read.
Output: fileContent: Output the read file buff.
Return: If the read fails, '0' is returned. If the read succeeds, the actual file size is returned.
Others: If the actual file size is greater than FILE_SIZE_MAX, the read fails.
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
		printf("The maximum file size is 4096 byte.\n");
		fclose(pFile);
		return 0;
	}
	fclose(pFile);
	return readSize;
}

/*************************************************
Function: WriteFile
Description: Write a file.
Calls: fopen(), fwrite(), fclose()
Called By: Decrypt()
Input: filePath: Target path of the file to be written.
       fileContent: Content to be written into the file.
       fileContentLen: Length of the content to be written into the file.
Output: none
Return: If the write fails, '0' is returned. If the write succeeds, the actual file size is returned.
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
Description: User manual.
Calls: printf(), exit()
Called By: GetParameter(), PerformAction(), Encrypt(), Decrypt()
Input: status: exit status [Use the 'EXIT_FAILURE' or 'EXIT_SUCCESS' defined in stdlib.h as the input parameter value of the function].
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
		       "\t-k, --key <key>\t\tKey string specified when encrypting or decrypting [0 < KEY_LENGTH <= 128, letters and numbers only].\n"
		       "\t-f, --file-path <path>\t\tEncrypted data file path [0 < FILE_SIZE <= 4096B, strong keys are recommended].\n"
		       "\t-o, --output <path>\t\tDecrypted data output path.\n"
		       "\t-h, --help\t\tDisplay this help and exit.\n");
	}
	exit(status);
}

/*************************************************
Function: Encrypt
Description: Perform an encryption operation.
Calls: ReadFile(), DataSealingWriteFile(), TeecClose(), fprintf(), Usage(), exit()
Called By: PerformAction()
Input: key: Key character string specified during encryption.
       filePath: Path of the encrypted file.
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
			fprintf(stderr, "Failed to encrypt the file.!\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			TeecClose();
			printf("File encrypted successfully.\n");
			exit(EXIT_SUCCESS);
		}
	}
	else
	{
		TeecClose();
		fprintf(stderr, "Check if the file is valid.\n");
		Usage(EXIT_FAILURE);
	}
}

/*************************************************
Function: Decrypt
Description: Perform a decryption operation.
Calls: DataSealingReadFile(), WriteFile(), TeecClose(), fprintf(), Usage(), exit()
Called By: PerformAction()
Input: key: Key character string specified during encryption.
       outputFilePath: Output path of the decrypted file.
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
		fprintf(stderr, "Failed to decrypt the file.\n");
		Usage(EXIT_FAILURE);
	}
	else
	{
		if (!WriteFile(outputFilePath, fileContent, readSize))
		{
			TeecClose();
			fprintf(stderr, "Filed decrypted successfully. But there is an exception during output. [Please check if the path of the output file is valid.]\n");
			exit(EXIT_FAILURE);
		}
		TeecClose();
		printf("File decrypted successfully.\n");
		exit(EXIT_SUCCESS);
	}
}

/*************************************************
Function: PerformAction
Description: Perform encryption or decryption.
Calls: TeecInit(), TeecClose(), Encrypt(), Decrypt(), fprintf(), Usage(), exit()
Called By: main()
Input: parameterT: Input parameter structure.
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
Description: Obtain the input parameters.
Calls: getopt_long()
Called By: main()
Input: argc: Number of input parameters.
       argv: String array of input parameters.
Output: parameterT: Command line option parameter structure[ParameterT].
Return: void
Others: none
*************************************************/
static void GetParameter(int argc, char** argv, ParameterT* parameterT)
{
	int c;    // Value of the option element returned by getopt_long().
	// Obtain the corresponding values based on the input option elements.
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
				fprintf(stderr, "Check if the key is valid.\n");
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
	// Detailed help information is returned only when /vendor/bin/data-sealing is executed.
	if (argc == 1)
	{
		Usage(EXIT_SUCCESS);
	}
	// Determine the operation type based on the input non-option elements.
	// [If the operation type is unknown or multiple repeated non-option elements are input, the operation type is UNKNOWN.]
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
