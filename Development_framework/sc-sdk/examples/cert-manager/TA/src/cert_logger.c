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
 * Description: logger tool implementations.
 */
#include "cert_logger.h"

#include <tee_log.h>
#include <tee_trusted_storage_api.h>
#include <string.h>
#include <stdlib.h>
#include <securec.h>

static const char *g_log_path = "dyn_crt_op.log";

static TEE_Result log_open(TEE_ObjectHandle *obj, uint32_t flag)
{
    uint32_t storage_id = TEE_OBJECT_STORAGE_PRIVATE;
    uint32_t create_flag = TEE_DATA_FLAG_ACCESS_WRITE |
                           TEE_DATA_FLAG_ACCESS_WRITE_META |
                           TEE_DATA_FLAG_ACCESS_READ;
    /* open log file */
    TEE_Result ret = TEE_OpenPersistentObject(storage_id, g_log_path, strlen(g_log_path), flag, obj);
    if (ret == TEE_ERROR_ITEM_NOT_FOUND &&
        (((flag & TEE_DATA_FLAG_ACCESS_WRITE) != 0) || ((flag & TEE_DATA_FLAG_ACCESS_WRITE_META) != 0) ||
        ((flag & TEE_DATA_FLAG_SHARE_WRITE) != 0))) {
        /* create it if file is not exist when writing or changing metadata */
        tlogi("file not exist, creating: %s\n", g_log_path);
        ret = TEE_CreatePersistentObject(storage_id, g_log_path, strlen(g_log_path),
                                         create_flag, TEE_HANDLE_NULL,
                                         NULL, 0, obj);
        if (ret != TEE_SUCCESS)
            tloge("create file failed: %s\n", g_log_path);
    }
    return ret;
}

/* truncate if the file beyonds MAX_LOG_LINE_NUM */
static TEE_Result log_truncate(TEE_ObjectHandle obj)
{
    TEE_Result ret;
    char buf[MAX_LOG_SIZE] = { 0 };
    uint32_t len = 0;
    /* 1. read all content in the log file into memory buf */
    ret = TEE_SeekObjectData(obj, 0, TEE_DATA_SEEK_SET);
    if (ret != TEE_SUCCESS) {
        tloge("seek file failed: %s\n", g_log_path);
        goto end;
    }
    ret = TEE_ReadObjectData(obj, buf, sizeof(buf), &len);
    if (ret != TEE_SUCCESS) {
        tloge("read file failed: %s\n", g_log_path);
        goto end;
    }
    /* 2. statistic line-breaks for counting lines */
    int line_cnt = 0;
    uint32_t i = 0;
    for (i = 0; i < len; ++i) {
        if (buf[i] == '\n')
            line_cnt += 1;
    }
    /* 3. if the line number is overflow, remove the first (line_cnt - MAX_LOG_LINE_NUM) lines */
    if (line_cnt > MAX_LOG_LINE_NUM) {
        /* line break */
        uint32_t line_break_idx = 0;
        uint32_t remain = line_cnt - MAX_LOG_LINE_NUM;
        for (i = 0; remain > 0; ++i) {
            if (buf[i] == '\n') {
                line_break_idx = i;
                remain--;
            }
        }
        /* 3.1 override the first (line_cnt - MAX_LOG_LINE_NUM) lines */
        uint32_t resize_to = len - line_break_idx - 1;
        if (memmove_s(buf, sizeof(buf), buf + line_break_idx + 1, resize_to) != EOK) {
            tloge("memory movement failed\n");
            ret = TEE_ERROR_OUT_OF_MEMORY;
            goto end;
        }
        /* 3.2 write back to the start of file */
        ret = TEE_SeekObjectData(obj, 0, TEE_DATA_SEEK_SET);
        if (ret != TEE_SUCCESS) {
            tloge("seek file failed: %s\n", g_log_path);
            goto end;
        }
        ret = TEE_WriteObjectData(obj, buf, resize_to);
        if (ret != TEE_SUCCESS)
            tloge("write file failed: %s\n", g_log_path);
        /* 3.3 truncate to correct size */
        ret = TEE_TruncateObjectData(obj, resize_to);
        if (ret != TEE_SUCCESS)
            tloge("truncate file failed: %s\n", g_log_path);
    }
end:
    return ret;
}

/* write a NULL-terminated string into log */
TEE_Result cert_log_write(char *log_info)
{
    TEE_Result ret;
    TEE_ObjectHandle obj;
    if (log_info == NULL)
        return TEE_ERROR_BAD_PARAMETERS;
    /* 1. open log file */
    uint32_t open_flag = TEE_DATA_FLAG_ACCESS_WRITE |
                         TEE_DATA_FLAG_SHARE_WRITE |
                         TEE_DATA_FLAG_SHARE_READ |
                         TEE_DATA_FLAG_ACCESS_READ;
    ret = log_open(&obj, open_flag);
    if (ret != TEE_SUCCESS) {
        tloge("open file failed: %s\n", g_log_path);
        goto end;
    }
    /* 2. append log to the end */
    ret = TEE_SeekObjectData(obj, 0, TEE_DATA_SEEK_END);
    if (ret != TEE_SUCCESS) {
        tloge("seek file failed: %s\n", g_log_path);
        goto close;
    }
    ret = TEE_WriteObjectData(obj, log_info, strlen(log_info));
    if (ret != TEE_SUCCESS) {
        tloge("write file failed: %s\n", g_log_path);
        goto close;
    }
    /* 3. truncate the file for keeping the number of lines MAX_LOG_LINE_NUM */
    ret = log_truncate(obj);
    if (ret != TEE_SUCCESS) {
        tloge("roll back file failed: %s\n", g_log_path);
        goto close;
    }
close:
    (void)TEE_SyncPersistentObject(obj);
    TEE_CloseObject(obj);
end:
    return ret;
}

TEE_Result cert_log_read(char *dst, uint64_t dst_len, uint32_t *read_len)
{
    TEE_Result ret;
    TEE_ObjectHandle obj;
    if (dst == NULL || read_len == NULL)
        return TEE_ERROR_BAD_PARAMETERS;
    /* 1. open log file */
    uint32_t open_flag = TEE_DATA_FLAG_ACCESS_READ | TEE_DATA_FLAG_SHARE_READ;
    ret = log_open(&obj, open_flag);
    if (ret != TEE_SUCCESS) {
        tloge("open file failed: %s\n", g_log_path);
        goto end;
    }
    /* 2. read log file */
    ret = TEE_ReadObjectData(obj, dst, dst_len, read_len);
    if (ret != TEE_SUCCESS)
        tloge("read file failed: %s\n", g_log_path);
    TEE_CloseObject(obj);
end:
    return ret;
}
