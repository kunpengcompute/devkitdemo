/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Description: declaration of logger tool interfaces.
 */
#ifndef CERT_LOGGER_H
#define CERT_LOGGER_H

#include <tee_defines.h>

/*
 * MAX_LOG_LINE_NUM: defines the max line numbers of which the operation log records.
 * MAX_LOG_LINE_LEN: defines the max length of each operation log entry.
 * MAX_LOG_SIZE: defines the max size of the operation log file.
 * Notice: MAX_LOG_LINE_NUM * MAX_LOG_LINE_LEN <= MAX_LOG_SIZE
 */
#define MAX_LOG_LINE_NUM 60
#define MAX_LOG_LINE_LEN 150
#define MAX_LOG_SIZE 10000

TEE_Result cert_log_write(char *log_info);
TEE_Result cert_log_read(char *dst, uint64_t dst_len, uint32_t *read_len);

#endif
