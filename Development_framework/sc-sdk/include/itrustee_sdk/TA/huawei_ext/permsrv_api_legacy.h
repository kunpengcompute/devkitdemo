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
 * Description: Functions in this header file are deprecated. Do not use
 */

#ifndef PERMSRV_API_LEGACY_H
#define PERMSRV_API_LEGACY_H

#include "tee_defines.h"

TEE_Result TEE_EXT_GetRpmbThreshold(const TEE_UUID *uuid, uint32_t *result);
TEE_Result TEE_EXT_crl_cert_process(const char *crl_cert, uint32_t crl_cert_size);
TEE_Result TEE_EXT_ta_ctrl_list_process(const char *ctrl_list, uint32_t ctrl_list_size);
#endif
