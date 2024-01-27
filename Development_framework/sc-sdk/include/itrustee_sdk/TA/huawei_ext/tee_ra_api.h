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
 * Description: API of TCMGR service.
 */
#ifndef TCMGR_SERVICE_TEE_RA_API_H
#define TCMGR_SERVICE_TEE_RA_API_H
#include <tee_defines.h>
#include "qsi_data_structure.h"

TEE_Result ra_seal(uint8_t *data, size_t in_size, uint8_t *cipher_data, size_t *cipher_size, uint32_t alg);
TEE_Result ra_unseal(uint8_t *cipher_data, size_t cipher_size, uint8_t *data, size_t *out_size, uint32_t alg);
TEE_Result ra_local_report(struct ra_buffer_data *in, struct ra_buffer_data *out);
TEE_Result ra_qsi_invoke(struct ra_buffer_data *in, struct ra_buffer_data *out);

#endif
