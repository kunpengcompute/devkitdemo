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
 */

// gcc test_swap.c -o test_swap
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MEM_PART "/proc/meminfo"
#define STEP    4096
#define M    50

struct mem_info {
    char MemTotal[20];
    char MemFree[20];
    char MemAvailable[20];
    char SwapTotal[20];
    char SwapFree[20];
};

typedef struct mem_info MEM_info, *pMEM_info；

int get_file_line(char *result, char *fileName, int lineNumber)
{
    FILE *filePointer = NULL;
    int i = 0;
    const size_t BUFSIZE = 300;
    char buffer[BUFSIZE];

    if ((fileName == NULL) || (result == NULL)) {
        return 0;
    }

    if (!(filePointer = fopen(fileName, "rb"))) {
        return 0;
    }

    while ((!feof(filePointer)) && (i < lineNumber)) {
        if (!fgets(buffer, BUFSIZE, filePointer)) {
            return 0;
        }
        i++;
    }

    strcpy(result, buffer);

    if (fclose(filePointer) != 0) {
        return 0;
    }

    return 1;
}

int get_mem_info(pMEM_info mem)
{
    const size_t BUFSIZE = 300;
    char buffer[BUFSIZE];
    if (mem == NULL) {
        printf("\nget_mem_info error!\n");
        return 0;
    }
    memset(mem, 0, sizeof(MEM_info));
    if (get_file_line(buffer, MEM_PART, 1) == 1) { // read 1st line: MemTotal
        sscanf(buffer, "%*s %s", mem->MemTotal);
    }
    if (get_file_line(buffer, MEM_PART, 2) == 1) { // read 2nd line: MemFree
        sscanf(buffer, "%*s %s", mem->MemFree);
    }
    if (get_file_line(buffer, MEM_PART, 3) == 1) { // read 3rd line: MemAvailable
        sscanf(buffer, "%*s %s", mem->MemAvailable);
    }
    if (get_file_line(buffer, MEM_PART, 15) == 1) { // read 15th line: SwapTotal
        sscanf(buffer, "%*s %s", mem->SwapTotal);
    }
    if (get_file_line(buffer, MEM_PART, 16) == 1) { // read 16th line: SwapFree
        sscanf(buffer, "%*s %s", mem->SwapFree);
    }
    return 0;
}

int main()
{
    MEM_info mem;
    int a;
    long long i;

    get_mem_info(&mem);
    printf("\n MemTotal: %s, MemFree: %s, MemAvailable: %s, SwapTotal: %s, SwapFree: %s\n", \
    mem.MemTotal, mem.MemFree, mem.MemAvailable, mem.SwapTotal, mem.SwapFree);
    long long MemFree = strtol(mem.MemFree, NULL, 10);
    long long SwapFree = strtol(mem.SwapFree, NULL, 10);
    const long long N = 900 * (MemFree + SwapFree);

    for (a = 0; a < M; a++) {
        char *p = (char*) malloc(sizeof(char) * N);
        for (i = 0; i < N; i += STEP) {
            p[i] = "A";
        }
        sleep(1);
        free(p);
    }

    return 0;
}