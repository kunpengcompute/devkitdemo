/*
 * Copyright 2022 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http: //www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description: sha256 demo
 * Create: 2022-07-20
 */

/*************************** HEADER FILES ***************************/
#include <stdio.h>
#include <memory.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "sha256.h"

#define ARGC_NUM 2
/*********************** FUNCTION DEFINITIONS ***********************/
int sha256_test(const char *filepath)
{
	BYTE test_data[MAX_DATA_LEN];
	FILE *fp = NULL;
	int pass = 1;
	size_t len = 0;

	BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	
	// open the file
	fp = fopen(filepath, "rb");
	if (fp == NULL) {
		printf("Can't open \"%s\"", filepath);
		return -1;
	}
	// initialize pointer
	sha256_init(&ctx);
	// Encrypted update of the read block data
	while (!feof(fp) && !ferror(fp)) {
		memset(test_data, 0x1, sizeof(test_data));
		len = fread(test_data, 1, MAX_DATA_LEN, fp);
		if (len != 0) {
			sha256_update(&ctx, test_data, len);
		}
	}
	sha256_final(&ctx, buf);

	for (int i = 0; i < 32; i++) {
		printf("%02x", buf[i]);
	}
	printf("\n");

	//close the file
	fclose(fp);
	
	return(pass);
}

int main(int argc, char *argv[])
{
	char *absPath = NULL;
	clock_t start1, end1, start2, end2;
	if (argc < ARGC_NUM) {
		printf("Please input the file path of the test data.\n");
		return -1;
	}
	// The file path of test data
	absPath = realpath((const char *)argv[1], NULL);
	if (absPath == NULL) {
		printf("The file path of the test data is invalid.\n");
		return -1;
	}
	// test asm
	start1 = clock();
	sha256_test(absPath);
	end1 = clock();
	printf("time asm=%fs\n", (double)(end1 - start1)/CLOCKS_PER_SEC);

	// test soft
	ON_ARM = 0;
	start2 = clock();
	sha256_test(absPath);
	end2 = clock();
	printf("time soft=%fs\n", (double)(end2 - start2)/CLOCKS_PER_SEC);

	free(absPath);
	return 0;
}
