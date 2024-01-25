/*******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 * Description: Part of KML library
 * Author: KML
 * Create: 2023
 ******************************************************************************/

#ifndef KML_STRUCT_ILU_H
#define KML_STRUCT_ILU_H

#define KML_EXPORT __attribute__((visibility("default")))

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef KmlIssPsStructilu0Handle
struct KmlIssPcStructilu0;
typedef struct KmlIssPcStructilu0 *KmlIssPsStructilu0Handle ;
#endif

KML_EXPORT int KmlIssPcStructilu0Setup(KmlIssPsStructilu0Handle  *gSmoother, float *v,
    const int *xl, const int *xr, const int *yl, const int *yr, const int *zl, const int *zr);

KML_EXPORT int KmlIssPcStructilu0Apply(KmlIssPsStructilu0Handle  gSmoother, float *bb, float *xx);

KML_EXPORT int KmlIssPcStructilu0Clean(KmlIssPsStructilu0Handle  gSmoother);

#if defined(__cplusplus)
}
#endif

#endif