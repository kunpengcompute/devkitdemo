/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: CA for certification management.
 */
#include "cert_file.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <tee_client_type.h>

#include "cert_common.h"

bool IsFileExist(const char *path)
{
    return access(path, F_OK) == 0;
}

static int32_t GetFileSize(const char *path, long *size)
{
    int32_t ret;
    struct stat buf;
    ret = stat(path, &buf);
    if (ret != 0)
        printf_err("file stat failed: %s\n", path);
    else
        *size = buf.st_size;
    return ret;
}

int32_t LoadFromFs(void *buffer, size_t size, const char *filePath, size_t *fileSize)
{
    int32_t ret;
    /* check file name */
    if (strstr(filePath, ".der") == NULL && strstr(filePath, ".crl") == NULL) {
        ret = errno;
        printf_err("only support der or crl file\n");
        goto end;
    }
    /* get file length */
    ret = GetFileSize(filePath, fileSize);
    if (ret != 0) {
        printf_err("get file length failed: %s\n", filePath);
        goto end;
    }
    /* check file content overflow */
    if (*fileSize > size) {
        printf_err("file is too long: %s\n", filePath);
        ret = errno;
        goto end;
    }
    /* read contents from file into buffer */
    FILE *fp = fopen(filePath, "r");
    if (fp == NULL) {
        ret = errno;
        printf_err("open file failed: %s\n", filePath);
        goto end;
    }
    size_t readLen = fread(buffer, 1, size, fp);
    if (readLen != *fileSize) {
        ret = errno;
        printf_err("read file failed: %s\n", filePath);
    }
    (void)fclose(fp);
end:
    return ret;
}

int32_t StoreToFs(const void *buffer, uint32_t size, const char *filePath)
{
    int32_t ret = 0;
    if (buffer == NULL || size == 0 || filePath == NULL) {
        printf_err("store to fs bad parameters\n");
        return errno;
    }
    /* write size of buffer into file */
    FILE *fp = fopen(filePath, "w");
    if (fp == NULL) {
        ret = errno;
        printf_err("open file failed: %s\n", filePath);
        goto end;
    }
    size_t writeLen = fwrite(buffer, 1, size, fp);
    if (writeLen != size) {
        ret = errno;
        printf_err("write file failed: %s\n", filePath);
    }
    (void)fclose(fp);
end:
    return ret;
}
