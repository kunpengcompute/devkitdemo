/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: vsl random number generator followed different distribution.
 * Author: zky
 * Create: 2022-06-30
 */
#ifndef KVSL_H
#define KVSL_H
#include "vsl_types.h"

#ifdef __cplusplus
extern "C" {
#endif
KmlVslResult kml_vsl_runif(VslPolicy *policy, N01type normalMethod, int len, double *dst, double a, double b);
KmlVslResult kml_vsl_rexp(VslPolicy *policy, N01type normalMethod, int len, double *dst, double scale);
KmlVslResult kml_vsl_rnorm(VslPolicy *policy, N01type normalMethod, int len, double *dst, double mu, double sigma);
KmlVslResult kml_vsl_rbeta(VslPolicy *policy, N01type normalMethod, int len, double *dst, double aa, double bb);
KmlVslResult kml_vsl_rbinom(VslPolicy *policy, N01type normalMethod, int len, int *dst, int size, double prob);
KmlVslResult kml_vsl_rgeom(VslPolicy *policy, N01type normalMethod, int len, int *dst, double a);
KmlVslResult kml_vsl_rpois(VslPolicy *policy, N01type normalMethod, int len, int *dst, double mu);
KmlVslResult kml_vsl_rgamma(VslPolicy *policy, N01type normalMethod, int len, double *dst, double a, double scale);
KmlVslResult kml_vsl_rnbinom(VslPolicy *policy, N01type normalMethod, int len, int *dst, int size, double mu);
KmlVslResult kml_vsl_rlnorm(VslPolicy *policy, N01type normalMethod, int len, double *dst, double meanlog,
    double sdlog);
KmlVslResult kml_vsl_rcauchy(VslPolicy *policy, N01type normalMethod, int len, double *dst, double location,
    double scale);
KmlVslResult kml_vsl_rhyper(VslPolicy *policy, N01type normalMethod, int len, int *dst, int nn1in, int nn2in,
    int kkin);
KmlVslResult kml_vsl_rweibull(VslPolicy *policy, N01type normalMethod, int len, double *dst, double shape,
    double scale);
KmlVslResult kml_vsl_init(VslPolicy **policy, RNGtype type, uint32_t seed);
KmlVslResult kml_vsl_destroy(VslPolicy *policy);
KmlVslResult kml_vsl_setRNGKind(VslPolicy *policy, RNGtype newkind);
KmlVslResult kml_vsl_setSeed(VslPolicy *policy, uint32_t seed);

#define VERSION_STRUCT_LEN 100
typedef struct {
    char productName[VERSION_STRUCT_LEN];
    char productVersion[VERSION_STRUCT_LEN];
    char componentName[VERSION_STRUCT_LEN];
    char componentVersion[VERSION_STRUCT_LEN];
    char componentAppendInfo[VERSION_STRUCT_LEN];
    char softwareName[VERSION_STRUCT_LEN];
    char softwareVersion[VERSION_STRUCT_LEN];
}KVSLVersion;
KmlVslResult kml_vsl_getVersion(KVSLVersion *ver);

#ifdef __cplusplus
}
#endif
#endif