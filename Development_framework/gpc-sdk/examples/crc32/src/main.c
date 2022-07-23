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
 * Description: main.c
 * Create: 2022-07-20
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "crc32_demo.h"
#include "crc32_neon_demo.h"
 
#define ARGC 2
#define ARGV_PATH 1
#define BUFSIZE 1024*4


static int ControlParamCheck(int argc, char *argv[])
{
    if (argv == NULL) {
        printf("Argv input error!\n");
        return -1;
    }

    if (argc >ARGC) {
        printf("Argv input error!\n");
        return -1;
    }
	 return 0;
}

char *GetFilePath(int argc, char *argv[])
{
    unsigned char *absPath = NULL;
    if (argc == 1) {
        absPath = realpath("../src/main.c", NULL);
    } else {
        absPath = realpath(argv[ARGV_PATH], NULL);
    }

    if (access(absPath, 0) != 0) {
        printf("File path %s is not exist\n", argv[ARGV_PATH]);
        return NULL;
    }
    printf("Test file path %s\n", absPath);
    return absPath;
 }
 
static int calc_img_crc(const char * in_file)
{
    int fd;
    int n;
    unsigned char buf[BUFSIZE];
    unsigned int crcResult = 0;
    fd = open(in_file, O_RDONLY);
    while ((n = read(fd, buf, BUFSIZE)) > 0) {
       crcResult = CalcCrc32(crcResult, buf, n);
    }
    close(fd);
    if (n < 0) {
       printf("Read failed!\n");
       return -1;
    }
    printf("Crc:0x%x\n", crcResult);
    return 0;
}

static int calc_img_neon_crc(const char * in_file)
{
    int fd;
    int n;
    unsigned char buf[BUFSIZE];
    unsigned int crcResult = 0;
    fd = open(in_file, O_RDONLY);
    while ((n = read(fd, buf, BUFSIZE)) > 0) {
       crcResult = NeonCrc32(crcResult, buf, n);
    }
    close(fd);
    if (n < 0) {
       printf("Read failed!\n");
       return -1;
    }
    printf("Neon crc:0x%x\n", crcResult);
    return 0;
}


int main(int argc, char *argv[])
{
    struct timeval time1, time2;
    int ret = 0;
    
    if(ControlParamCheck(argc, argv) != 0) {
        return -1;
    }
    unsigned char *absTestFile = GetFilePath(argc, argv);
    if (absTestFile == NULL) {
        return -1;
    }
    
    printf("Crc calc:\n");
    gettimeofday(&time1, NULL);
    ret = calc_img_crc(absTestFile);
    if (ret != 0) {
        printf("Crc calc failed!\n");
        return -1;
    }
    gettimeofday(&time2, NULL);
    printf("Time used: %ld ms\n", (time2.tv_sec - time1.tv_sec) * 1000 + (time2.tv_usec - time1.tv_usec) /1000);
    
    printf("Crc calc:\n");
    gettimeofday(&time1, NULL);
    ret = calc_img_neon_crc(absTestFile);
    if (ret != 0) {
        printf("Neon crc calc failed!\n");
        return -1;
    }
    gettimeofday(&time2, NULL);
    printf("Time used: %ld ms\n", (time2.tv_sec - time1.tv_sec) * 1000 + (time2.tv_usec - time1.tv_usec) /1000);
    
    free (absTestFile);
    return 0;
}
