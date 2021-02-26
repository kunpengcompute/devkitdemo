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
 *
 *   IO写文件， IO writes files
 *   gcc -g io_continuum.c -o io_continuum
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#define WRITE_BUF_SIZE 64
#define WRITE_COUNTS 100*1024*1024
#define INT int
#define CHAR char
#define VOID void
#define WRITE_LEVEL 0777
#define IO_CONTINUUM_TEXT "./io_continuum_test.txt"

enum IO_Operation {
    Sync_Write = 0,
    Nosync_Write,
};

INT remove_test_file()
{
    if (remove(IO_CONTINUUM_TEXT) != 0) {
        printf("Failed to remove file: %s, please delete the file.\n", IO_CONTINUUM_TEXT);
    }
    else {
        printf("Success to remove file: %s\n", IO_CONTINUUM_TEXT);
    }
    return 0;
}

INT IO_Write_With_Sync()
{
    INT count = 0;
    INT fd;
    CHAR buf = "test io continuum!\n";
    if ((fd = open(IO_CONTINUUM_TEXT, O_WRONLY | O_TRUNC | O_CREAT, WRITE_LEVEL)) == -1) {
        return -1;
    }
    else {
        while (count <= WRITE_COUNTS) {
            write(fd, buf, WRITE_COUNTS);
            fsync(fd);
            count++;
        }
    }
    return 0;
}

INT IO_Write_Without_Sync()
{
    INT count = 0;
    INT fd;
    CHAR buf = "test io continuum!\n";
    if ((fd = open(IO_CONTINUUM_TEXT, O_WRONLY | O_TRUNC | O_CREAT, WRITE_LEVEL)) == -1) {
        return -1;
    }
    else {
        while (count <= WRITE_COUNTS) {
            write(fd, buf, WRITE_COUNTS);
            count++;
        }
    }
    return 0;
}

VOID sighandler(INT signum)
{
    remove_test_file();
    exit(1);
}

INT main(INT argc, CHAR **argv)
{
    INT val;
    val = atoi(argv[1]);
    signal(SIGINT, sighandler);
    switch (val) {
    case Sync_Write:
        IO_Write_With_Sync();
        remove_test_file();
        break;
    case Nosync_Write:
        IO_Write_Without_Sync();
        remove_test_file();
        break;
    default:
        break;
    }
    return 0;
}