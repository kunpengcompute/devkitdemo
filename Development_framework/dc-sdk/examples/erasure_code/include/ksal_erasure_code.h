/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */
#ifndef KSAL_EC_H
#define KSAL_EC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
struct HecParam {
    uint32_t dataNum;
    uint32_t parityNum;
    uint32_t sectorSize;
    uint32_t blockSize;
    uint32_t targetColNum;
    uint32_t targetColArray[6];
    uint32_t version;
};
#define KSAL_RET_OK (0)
#define KSAL_RET_UNVALID_NM (1)
#define KSAL_RET_UNVALID_DECODE (2)
#define KSAL_RET_UNVALID_PTR (3)
int KsalEcEncode(uint8_t **data, uint8_t **parity, struct HecParam *param);
int KsalEcDecode(uint8_t **data, uint8_t **parity, struct HecParam *param);
#ifdef __cplusplus
}
#endif
#endif