/*******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 * Description: Part of KML library
 * Author: KML
 * Create: 2023
 ******************************************************************************/

#ifndef KML_GCR_SPECIFIC_H
#define KML_GCR_SPECIFIC_H

#define KML_EXPORT __attribute__((visibility("default")))

#if defined(__cplusplus)
extern "C" {
#endif

KML_EXPORT int KmlIssGcrAxpyP(float *r, float *p, const double *b, const int *m, const int *mt,
    const int *xl, const int *xr, const int *yl, const int *yr, const int *zl, const int *zr,
    const int *ybegin, const int *yend, const int *zbegin, const int *zend);

KML_EXPORT int KmlIssGcrAxpyAp(float *ar, float *ap, const double *b, const int *m, const int *mt,
    const int *xl, const int *xr, const int *yl, const int *yr, const int *zl, const int *zr,
    const int *ybegin, const int *yend, const int *zbegin, const int *zend);

KML_EXPORT int KmlIssGcrDotprodBeta(const float *ar, const float *ap, double *c, const int *m,
    const int *xl, const int *xr, const int *yl, const int *yr,
    const int *zl, const int *zr, const int *ybegin, const int *yend, const int *zbegin, const int *zend);

KML_EXPORT int KmlIssGcrAxpyX(float *x, const float *p, const double *ac, const int *xl, const int *xr,
    const int *yl, const int *yr, const int *zl, const int *ylT,
    const int *yrT, const int *zlT, const int *ybegin, const int *yend, const int *zbegin, const int *zend);

KML_EXPORT int KmlIssGcrAxpyR(float *r, const float *ap, double *d, const double *ac,
    const int *xl, const int *xr, const int *yl, const int *zl,
    const int *ylT, const int *yrT, const int *zlT,
    const int *ybegin, const int *yend, const int *zbegin, const int *zend);

KML_EXPORT int KmlIssGcrDotprodAlpha(const float *r, const float *ap, double *c, const int *xl, const int *xr,
    const int *yl, const int *yr, const int *zl,
    const int *ybegin, const int *yend, const int *zbegin, const int *zend);

KML_EXPORT int KmlIssGcrSpmv(const float *a, const float *x, float *b, const int *xl, const int *xr,
    const int *yl, const int *yr, const int *zl,
    const int *ybegin, const int *yend, const int *zbegin, const int *zend);
 
#if defined(__cplusplus)
}
#endif

#endif