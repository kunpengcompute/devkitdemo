/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */
#ifndef HCRC_H
#define HCRC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t KsalCrc32c(uint32_t crc, uint8_t *data, uint64_t len);
uint16_t KsalCrc16T10Dif(uint16_t crc, uint8_t *data, uint64_t len);

#ifdef __cplusplus
}
#endif

#endif