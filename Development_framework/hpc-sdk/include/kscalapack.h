/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: KScaLAPACK header
 * Author: KML
 * Create: 2021
 */
#ifndef CSRC_INCLUDE_KSCALAPACK_H
#define CSRC_INCLUDE_KSCALAPACK_H

#include <kml_export.h>

#ifdef __cplusplus
extern "C" {
#endif

KML_EXPORT void psgetrf_(const int *m, const int *n, float *a, const int *ia, const int *ja, const int *desca,
    int *ipiv, int *info);
KML_EXPORT void pdgetrf_(const int *m, const int *n, double *a, const int *ia, const int *ja, const int *desca,
    int *ipiv, int *info);
KML_EXPORT void pcgetrf_(const int *m, const int *n, float _Complex *a, const int *ia, const int *ja, const int *desca,
    int *ipiv, int *info);
KML_EXPORT void pzgetrf_(const int *m, const int *n, double _Complex *a, const int *ia, const int *ja, const int *desca,
    int *ipiv, int *info);

KML_EXPORT void psgetrf_lp_(const char *mode, const int *m, const int *n, float *a, const int *ia, const int *ja,
    const int *desca, int *ipiv, int *info);
KML_EXPORT void pdgetrf_lp_(const char *mode, const int *m, const int *n, double *a, const int *ia, const int *ja,
    const int *desca, int *ipiv, int *info);
KML_EXPORT void pcgetrf_lp_(const char *mode, const int *m, const int *n, float _Complex *a, const int *ia,
    const int *ja, const int *desca, int *ipiv, int *info);
KML_EXPORT void pzgetrf_lp_(const char *mode, const int *m, const int *n, double _Complex *a, const int *ia,
    const int *ja, const int *desca, int *ipiv, int *info);

KML_EXPORT void psgesv_(const int *n, const int *nrhs, float *a, const int *ia, const int *ja, const int *desca,
    int *ipiv, float *b, const int *ib, const int *jb, const int *descb, int *info);
KML_EXPORT void pdgesv_(const int *n, const int *nrhs, double *a, const int *ia, const int *ja, const int *desca,
    int *ipiv, double *b, const int *ib, const int *jb, const int *descb, int *info);
KML_EXPORT void pcgesv_(const int *n, const int *nrhs, float _Complex *a, const int *ia, const int *ja,
    const int *desca, int *ipiv, float _Complex *b, const int *ib, const int *jb, const int *descb, int *info);
KML_EXPORT void pzgesv_(const int *n, const int *nrhs, double _Complex *a, const int *ia, const int *ja,
    const int *desca, int *ipiv, double _Complex *b, const int *ib, const int *jb, const int *descb, int *info);

KML_EXPORT void pspotrf_(const char *uplo, const int *n, float *a, const int *ia, const int *ja, const int *desca,
    int *info);
KML_EXPORT void pdpotrf_(const char *uplo, const int *n, double *a, const int *ia, const int *ja, const int *desca,
    int *info);
KML_EXPORT void pcpotrf_(const char *uplo, const int *n, float _Complex *a, const int *ia, const int *ja,
    const int *desca, int *info);
KML_EXPORT void pzpotrf_(const char *uplo, const int *n, double _Complex *a, const int *ia, const int *ja,
    const int *desca, int *info);

KML_EXPORT void psgeqrf_(const int *m, const int *n, float *a, const int *ia, const int *ja, const int *desca,
    float *tau, float *work, int *lwork, int *info);
KML_EXPORT void pdgeqrf_(const int *m, const int *n, double *a, const int *ia, const int *ja, const int *desca,
    double *tau, double *work, int *lwork, int *info);
KML_EXPORT void pcgeqrf_(const int *m, const int *n, float _Complex *a, const int *ia, const int *ja, const int *desca,
    float _Complex *tau, float _Complex *work, int *lwork, int *info);
KML_EXPORT void pzgeqrf_(const int *m, const int *n, double _Complex *a, const int *ia, const int *ja, const int *desca,
    double _Complex *tau, double _Complex *work, int *lwork, int *info);

KML_EXPORT void psgels_(const char *trans, const int *m, const int *n, const int *nrhs, float *a, const int *ia,
    const int *ja, const int *desca, float *b, const int *ib, const int *jb, const int *descb, float *work, int *lwork,
    int *info);
KML_EXPORT void pdgels_(const char *trans, const int *m, const int *n, const int *nrhs, double *a, const int *ia,
    const int *ja, const int *desca, double *b, const int *ib, const int *jb, const int *descb, double *work,
    int *lwork, int *info);
KML_EXPORT void pcgels_(const char *trans, const int *m, const int *n, const int *nrhs, float _Complex *a,
    const int *ia, const int *ja, const int *desca, float _Complex *b, const int *ib, const int *jb, const int *descb,
    float _Complex *work, int *lwork, int *info);
KML_EXPORT void pzgels_(const char *trans, const int *m, const int *n, const int *nrhs, double _Complex *a,
    const int *ia, const int *ja, const int *desca, double _Complex *b, const int *ib, const int *jb, const int *descb,
    double _Complex *work, int *lwork, int *info);

KML_EXPORT void pssyev_(const char *jobz, const char *uplo, const int *n, float *a, const int *ia, const int *ja,
    const int *desca, float *w, float *z, const int *iz, const int *jz, const int *descz, float *work, const int *lwork,
    int *info);
KML_EXPORT void pdsyev_(const char *jobz, const char *uplo, const int *n, double *a, const int *ia, const int *ja,
    const int *desca, double *w, double *z, const int *iz, const int *jz, const int *descz, double *work,
    const int *lwork, int *info);
KML_EXPORT void pcheev_(const char *jobz, const char *uplo, const int *n, float _Complex *a, const int *ia,
    const int *ja, const int *desca, float *w, float _Complex *z, const int *iz, const int *jz, const int *descz,
    float _Complex *work, const int *lwork, float *rwork, const int *lrwork, int *info);
KML_EXPORT void pzheev_(const char *jobz, const char *uplo, const int *n, double _Complex *a, const int *ia,
    const int *ja, const int *desca, double *w, double _Complex *z, const int *iz, const int *jz, const int *descz,
    double _Complex *work, const int *lwork, double *rwork, const int *lrwork, int *info);

KML_EXPORT void pssyevd_(const char *jobz, const char *uplo, const int *n, float *a, const int *ia, const int *ja,
    const int *desca, float *w, float *z, const int *iz, const int *jz, const int *descz, float *work, const int *lwork,
    int *iwork, const int *liwork, int *info);
KML_EXPORT void pdsyevd_(const char *jobz, const char *uplo, const int *n, double *a, const int *ia, const int *ja,
    const int *desca, double *w, double *z, const int *iz, const int *jz, const int *descz, double *work,
    const int *lwork, int *iwork, const int *liwork, int *info);
KML_EXPORT void pcheevd_(const char *jobz, const char *uplo, const int *n, float _Complex *a, const int *ia,
    const int *ja, const int *desca, float *w, float _Complex *z, const int *iz, const int *jz, const int *descz,
    float _Complex *work, const int *lwork, float *rwork, const int *lrwork, int *iwork, const int *liwork, int *info);
KML_EXPORT void pzheevd_(const char *jobz, const char *uplo, const int *n, double _Complex *a, const int *ia,
    const int *ja, const int *desca, double *w, double _Complex *z, const int *iz, const int *jz, const int *descz,
    double _Complex *work, const int *lwork, double *rwork, const int *lrwork, int *iwork, const int *liwork,
    int *info);

typedef struct KMLVersion KSCALAPACKVersion;
KML_EXPORT int KSCALAPACKGetVersion(KSCALAPACKVersion *ver);

#ifdef __cplusplus
}
#endif

#endif // CSRC_INCLUDE_KSCALAPACK_H
