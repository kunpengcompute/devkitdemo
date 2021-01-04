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
#define M    50

struct MemInfo {
    char memTotal[20];
    char memFree[20];
    char memAvailable[20];
    char swapTotal[20];
    char swapFree[20];
};

typedef struct MemInfo MemInfoRead, *p_MemInfoRead;

int GetFileLine(char *result, char *fileName, int lineNumber)
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
        return 1;
    }

    return 0;
}

int GetMemInfo(p_MemInfoRead mem)
{
    const size_t BUFSIZE = 300;
    char buffer[BUFSIZE];
    if (mem == NULL) {
        printf("\nget_mem_info error!\n");
        return 0;
    }
    memset(mem, 0, sizeof(MemInfoRead));
    if (GetFileLine(buffer, MEM_PART, 1) == 0) { // read 1st line: memTotal
        sscanf(buffer, "%*s %s", mem->memTotal);
    }
    if (GetFileLine(buffer, MEM_PART, 2) == 0) { // read 2nd line: memFree
        sscanf(buffer, "%*s %s", mem->memFree);
    }
    if (GetFileLine(buffer, MEM_PART, 3) == 0) { // read 3rd line: memAvailable
        sscanf(buffer, "%*s %s", mem->memAvailable);
    }
    if (GetFileLine(buffer, MEM_PART, 15) == 0) { // read 15th line: swapTotal
        sscanf(buffer, "%*s %s", mem->swapTotal);
    }
    if (GetFileLine(buffer, MEM_PART, 16) == 0) { // read 16th line: swapFree
        sscanf(buffer, "%*s %s", mem->swapFree);
    }
    return 0;
}

int main()
{
    MemInfoRead mem;
    const int STEP = 4096;
    const int MALLOC_SIZE = 900;
    int j;
    long long i;

    GetMemInfo(&mem);
    printf("\n MemTotal: %s, MemFree: %s, MemAvailable: %s, SwapTotal: %s, SwapFree: %s\n", 
        mem.memTotal, mem.memFree, mem.memAvailable, mem.swapTotal, mem.swapFree);
    long long MemFree = strtol(mem.memFree, NULL, 10); // convert string to long int base 10
    long long SwapFree = strtol(mem.swapFree, NULL, 10); // convert string to long int base 10
    const long long N = MALLOC_SIZE * (MemFree + SwapFree);

    for (j = 0; j < M; j++) {
        char *p = (char*) malloc(sizeof(char) * N);
        if (p == NULL) {
            perror("Memory allocation through malloc failed");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < N; i += STEP) {
            p[i] = 'A';
        }
        sleep(1);
        free(p);
    }

    return 0;
}