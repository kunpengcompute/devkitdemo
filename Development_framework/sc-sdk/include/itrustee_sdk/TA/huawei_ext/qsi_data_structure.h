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
 * Description: Data Structure Definations of QSI.
 */
#ifndef QSI_DATA_STRUCTURE_H
#define QSI_DATA_STRUCTURE_H
#include <tee_defines.h>

enum seal_operation {
    SEAL,
    UNSEAL
};

struct ra_buffer_data {
    uint32_t length;
    uint8_t *buffer;
};

#endif
