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
 * Description: permsrv cert api interface head file
 */

#ifndef PEMRSRV_API_CERT_H
#define PEMRSRV_API_CERT_H

#include "tee_defines.h"

TEE_Result ta_signing_cert_import(const char *cert_buf, uint32_t cert_size, const char *pub_key_buf, uint32_t pub_size);
TEE_Result ta_signing_cert_export(uint8_t *dst, uint32_t *len, uint32_t limit);
TEE_Result ta_signing_cert_destroy(void);
#endif
