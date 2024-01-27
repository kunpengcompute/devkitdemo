/*
 * @CopyRight: Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * @Description: neon functions header file
 * @Author: LiuYongYang
 * @Date: 2023-02-23
 * @LastEditTime: 2023-05-22
 */
#ifndef KNPY_H
#define KNPY_H

#include "arm_neon.h"

#define NEON_LANE0 0x0
#define NEON_LANE1 0x1
#define NEON_LANE2 0x2
#define NEON_LANE3 0x3

#define NEON_STEP2 0x2
#define NEON_STEP4 0x4
#define NEON_STEP8 0x8
#define NEON_STEP16 0x10

#define NUMPY_VERSION_INFO_LEN 100
typedef struct {
    char productName[NUMPY_VERSION_INFO_LEN];
    char productVersion[NUMPY_VERSION_INFO_LEN];
    char componentName[NUMPY_VERSION_INFO_LEN];
    char componentVersion[NUMPY_VERSION_INFO_LEN];
    char componentAppendInfo[NUMPY_VERSION_INFO_LEN];
    char softwareName[NUMPY_VERSION_INFO_LEN];
    char softwareVersion[NUMPY_VERSION_INFO_LEN];
} NumpyProVersion;

typedef enum {
    /* Definition of common error codes. Initial value is 0.       */
    NUMPY_STS_NO_ERR = 0,
    NUMPY_STS_NULL_PTR_ERR,
    NUMPY_STS_SIZE_TOOLARGE,
    NUMPY_STS_SIZE_TOOSMALL,
    NUMPY_STS_UNKNOWN_FEATURE
} NumpyResult;

NumpyResult NumpyGetProductVersion(NumpyProVersion *packageInfo);

float NeonSumf(float* matrix, const long int length);

double NeonSumd(double* matrix, const long int length);

long int NeonSumi(long int* matrix, const long int length);

float NeonMaxf(float* matrix, const long int length);

double NeonMaxd(double* matrix, const long int length);

long int NeonMaxi(long int* matrix, const long int length);

float NeonMinf(float* matrix, const long int length);

double NeonMind(double* matrix, const long int length);

long int NeonMini(long int* matrix, const long int length);

#endif // KNPY_H