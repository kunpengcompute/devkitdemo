/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 * Description: KML function
 * Author: KML
 * Create: 2020
 */
#ifndef KLAPACK_EXTERNAL_H
#define KLAPACK_EXTERNAL_H

#include <complex.h>
#include "kml_export.h"

#ifdef __cplusplus
extern "C" {
#endif

KML_EXPORT void cppsv_(const char *uplo, const int *n, const int *nrhs, float _Complex *ap, float _Complex *b,
    const int *ldb, int *info);

KML_EXPORT void dppsv_(const char *uplo, const int *n, const int *nrhs, double *ap, double *b, const int *ldb,
    int *info);

KML_EXPORT void sppsv_(const char *uplo, const int *n, const int *nrhs, float *ap, float *b, const int *ldb, int *info);

KML_EXPORT void zppsv_(const char *uplo, const int *n, const int *nrhs, double _Complex *ap, double _Complex *b,
    const int *ldb, int *info);

KML_EXPORT void cgetrf_(const int *m, const int *n, float _Complex *a, const int *lda, int *ipiv, int *info);

KML_EXPORT void dgetrf_(const int *m, const int *n, double *a, const int *lda, int *ipiv, int *info);

KML_EXPORT void sgetrf_(const int *m, const int *n, float *a, const int *lda, int *ipiv, int *info);

KML_EXPORT void zgetrf_(const int *m, const int *n, double _Complex *a, const int *lda, int *ipiv, int *info);

KML_EXPORT void cgetri_(const int *n, float _Complex *a, const int *lda, const int *ipiv, float _Complex *work,
    const int *lwork, int *info);

KML_EXPORT void dgetri_(const int *n, double *a, const int *lda, const int *ipiv, double *work, const int *lwork,
    int *info);

KML_EXPORT void sgetri_(const int *n, float *a, const int *lda, const int *ipiv, float *work, const int *lwork,
    int *info);

KML_EXPORT void zgetri_(const int *n, double _Complex *a, const int *lda, const int *ipiv, double _Complex *work,
    const int *lwork, int *info);

KML_EXPORT void cgetrs_(const char *trans, const int *n, const int *nrhs, const float _Complex *a, const int *lda,
    const int *ipiv, float _Complex *b, const int *ldb, int *info);

KML_EXPORT void dgetrs_(const char *trans, const int *n, const int *nrhs, const double *a, const int *lda,
    const int *ipiv, double *b, const int *ldb, int *info);

KML_EXPORT void sgetrs_(const char *trans, const int *n, const int *nrhs, const float *a, const int *lda,
    const int *ipiv, float *b, const int *ldb, int *info);

KML_EXPORT void zgetrs_(const char *trans, const int *n, const int *nrhs, const double _Complex *a, const int *lda,
    const int *ipiv, double _Complex *b, const int *ldb, int *info);

KML_EXPORT void cgeqrf_(const int *m, const int *n, float _Complex *a, const int *lda, float _Complex *tau,
    float _Complex *work, const int *lwork, int *info);

KML_EXPORT void dgeqrf_(const int *m, const int *n, double *a, const int *lda, double *tau, double *work,
    const int *lwork, int *info);

KML_EXPORT void sgeqrf_(const int *m, const int *n, float *a, const int *lda, float *tau, float *work, const int *lwork,
    int *info);

KML_EXPORT void zgeqrf_(const int *m, const int *n, double _Complex *a, const int *lda, double _Complex *tau,
    double _Complex *work, const int *lwork, int *info);

KML_EXPORT void cgelqf_(const int *m, const int *n, float _Complex *a, const int *lda, float _Complex *tau,
    float _Complex *work, const int *lwork, int *info);

KML_EXPORT void dgelqf_(const int *m, const int *n, double *a, const int *lda, double *tau, double *work,
    const int *lwork, int *info);

KML_EXPORT void sgelqf_(const int *m, const int *n, float *a, const int *lda, float *tau, float *work, const int *lwork,
    int *info);

KML_EXPORT void zgelqf_(const int *m, const int *n, double _Complex *a, const int *lda, double _Complex *tau,
    double _Complex *work, const int *lwork, int *info);

KML_EXPORT void cgerqf_(const int *m, const int *n, float _Complex *a, const int *lda, float _Complex *tau,
    float _Complex *work, const int *lwork, int *info);

KML_EXPORT void dgerqf_(const int *m, const int *n, double *a, const int *lda, double *tau, double *work,
    const int *lwork, int *info);

KML_EXPORT void sgerqf_(const int *m, const int *n, float *a, const int *lda, float *tau, float *work, const int *lwork,
    int *info);

KML_EXPORT void zgerqf_(const int *m, const int *n, double _Complex *a, const int *lda, double _Complex *tau,
    double _Complex *work, const int *lwork, int *info);

KML_EXPORT void cgeqlf_(const int *m, const int *n, float _Complex *a, const int *lda, float _Complex *tau,
    float _Complex *work, const int *lwork, int *info);

KML_EXPORT void dgeqlf_(const int *m, const int *n, double *a, const int *lda, double *tau, double *work,
    const int *lwork, int *info);

KML_EXPORT void sgeqlf_(const int *m, const int *n, float *a, const int *lda, float *tau, float *work, const int *lwork,
    int *info);

KML_EXPORT void zgeqlf_(const int *m, const int *n, double _Complex *a, const int *lda, double _Complex *tau,
    double _Complex *work, const int *lwork, int *info);

KML_EXPORT void sorglq_(const int *m, const int *n, const int *k, float *a, const int *lda, const float *tau,
    float *work, const int *lwork, int *info);

KML_EXPORT void dorglq_(const int *m, const int *n, const int *k, double *a, const int *lda, const double *tau,
    double *work, const int *lwork, int *info);

KML_EXPORT void cunglq_(const int *m, const int *n, const int *k, float _Complex *a, const int *lda,
    const float _Complex *tau, float _Complex *work, const int *lwork, int *info);

KML_EXPORT void zunglq_(const int *m, const int *n, const int *k, double _Complex *a, const int *lda,
    const double _Complex *tau, double _Complex *work, const int *lwork, int *info);

KML_EXPORT void sorgql_(const int *m, const int *n, const int *k, float *a, const int *lda, const float *tau,
    float *work, const int *lwork, int *info);

KML_EXPORT void dorgql_(const int *m, const int *n, const int *k, double *a, const int *lda, const double *tau,
    double *work, const int *lwork, int *info);

KML_EXPORT void cungql_(const int *m, const int *n, const int *k, float _Complex *a, const int *lda,
    const float _Complex *tau, float _Complex *work, const int *lwork, int *info);

KML_EXPORT void zungql_(const int *m, const int *n, const int *k, double _Complex *a, const int *lda,
    const double _Complex *tau, double _Complex *work, const int *lwork, int *info);

KML_EXPORT void sorgqr_(const int *m, const int *n, const int *k, float *a, const int *lda, const float *tau,
    float *work, const int *lwork, int *info);

KML_EXPORT void dorgqr_(const int *m, const int *n, const int *k, double *a, const int *lda, const double *tau,
    double *work, const int *lwork, int *info);

KML_EXPORT void cungqr_(const int *m, const int *n, const int *k, float _Complex *a, const int *lda,
    const float _Complex *tau, float _Complex *work, const int *lwork, int *info);

KML_EXPORT void zungqr_(const int *m, const int *n, const int *k, double _Complex *a, const int *lda,
    const double _Complex *tau, double _Complex *work, const int *lwork, int *info);

KML_EXPORT void sorgrq_(const int *m, const int *n, const int *k, float *a, const int *lda, const float *tau,
    float *work, const int *lwork, int *info);

KML_EXPORT void dorgrq_(const int *m, const int *n, const int *k, double *a, const int *lda, const double *tau,
    double *work, const int *lwork, int *info);

KML_EXPORT void cungrq_(const int *m, const int *n, const int *k, float _Complex *a, const int *lda,
    const float _Complex *tau, float _Complex *work, const int *lwork, int *info);

KML_EXPORT void zungrq_(const int *m, const int *n, const int *k, double _Complex *a, const int *lda,
    const double _Complex *tau, double _Complex *work, const int *lwork, int *info);

KML_EXPORT void cpptrf_(const char *uplo, const int *n, float _Complex *ap, int *info);

KML_EXPORT void dpptrf_(const char *uplo, const int *n, double *ap, int *info);

KML_EXPORT void spptrf_(const char *uplo, const int *n, float *ap, int *info);

KML_EXPORT void zpptrf_(const char *uplo, const int *n, double _Complex *ap, int *info);

KML_EXPORT void cpptrs_(const char *uplo, const int *n, const int *nrhs, const float _Complex *ap, float _Complex *b,
    const int *ldb, int *info);

KML_EXPORT void dpptrs_(const char *uplo, const int *n, const int *nrhs, const double *ap, double *b, const int *ldb,
    int *info);

KML_EXPORT void spptrs_(const char *uplo, const int *n, const int *nrhs, const float *ap, float *b, const int *ldb,
    int *info);

KML_EXPORT void zpptrs_(const char *uplo, const int *n, const int *nrhs, const double _Complex *ap, double _Complex *b,
    const int *ldb, int *info);

KML_EXPORT void cpptri_(const char *uplo, const int *n, float _Complex *ap, int *info);

KML_EXPORT void dpptri_(const char *uplo, const int *n, double *ap, int *info);

KML_EXPORT void spptri_(const char *uplo, const int *n, float *ap, int *info);

KML_EXPORT void zpptri_(const char *uplo, const int *n, double _Complex *ap, int *info);

KML_EXPORT void dormqr_(const char *side, const char *trans, const int *m, const int *n, const int *k, const double *a,
    const int *lda, const double *tau, double *c, const int *ldc, double *work, const int *lwork, int *info);

KML_EXPORT void sormqr_(const char *side, const char *trans, const int *m, const int *n, const int *k, const float *a,
    const int *lda, const float *tau, float *c, const int *ldc, float *work, const int *lwork, int *info);

KML_EXPORT void dormlq_(const char *side, const char *trans, const int *m, const int *n, const int *k, const double *a,
    const int *lda, const double *tau, double *c, const int *ldc, double *work, const int *lwork, int *info);

KML_EXPORT void sormlq_(const char *side, const char *trans, const int *m, const int *n, const int *k, const float *a,
    const int *lda, const float *tau, float *c, const int *ldc, float *work, const int *lwork, int *info);

KML_EXPORT void dormql_(const char *side, const char *trans, const int *m, const int *n, const int *k, const double *a,
    const int *lda, const double *tau, double *c, const int *ldc, double *work, const int *lwork, int *info);

KML_EXPORT void sormql_(const char *side, const char *trans, const int *m, const int *n, const int *k, const float *a,
    const int *lda, const float *tau, float *c, const int *ldc, float *work, const int *lwork, int *info);

KML_EXPORT void dormrq_(const char *side, const char *trans, const int *m, const int *n, const int *k, const double *a,
    const int *lda, const double *tau, double *c, const int *ldc, double *work, const int *lwork, int *info);

KML_EXPORT void sormrq_(const char *side, const char *trans, const int *m, const int *n, const int *k, const float *a,
    const int *lda, const float *tau, float *c, const int *ldc, float *work, const int *lwork, int *info);

KML_EXPORT void cunmqr_(const char *side, const char *trans, const int *m, const int *n, const int *k,
    const float _Complex *a, const int *lda, const float _Complex *tau, float _Complex *c, const int *ldc,
    float _Complex *work, const int *lwork, int *info);

KML_EXPORT void zunmqr_(const char *side, const char *trans, const int *m, const int *n, const int *k,
    const double _Complex *a, const int *lda, const double _Complex *tau, double _Complex *c, const int *ldc,
    double _Complex *work, const int *lwork, int *info);

KML_EXPORT void cunmlq_(const char *side, const char *trans, const int *m, const int *n, const int *k,
    const float _Complex *a, const int *lda, const float _Complex *tau, float _Complex *c, const int *ldc,
    float _Complex *work, const int *lwork, int *info);

KML_EXPORT void zunmlq_(const char *side, const char *trans, const int *m, const int *n, const int *k,
    const double _Complex *a, const int *lda, const double _Complex *tau, double _Complex *c, const int *ldc,
    double _Complex *work, const int *lwork, int *info);

KML_EXPORT void cunmql_(const char *side, const char *trans, const int *m, const int *n, const int *k,
    const float _Complex *a, const int *lda, const float _Complex *tau, float _Complex *c, const int *ldc,
    float _Complex *work, const int *lwork, int *info);

KML_EXPORT void zunmql_(const char *side, const char *trans, const int *m, const int *n, const int *k,
    const double _Complex *a, const int *lda, const double _Complex *tau, double _Complex *c, const int *ldc,
    double _Complex *work, const int *lwork, int *info);

KML_EXPORT void cunmrq_(const char *side, const char *trans, const int *m, const int *n, const int *k,
    const float _Complex *a, const int *lda, const float _Complex *tau, float _Complex *c, const int *ldc,
    float _Complex *work, const int *lwork, int *info);

KML_EXPORT void zunmrq_(const char *side, const char *trans, const int *m, const int *n, const int *k,
    const double _Complex *a, const int *lda, const double _Complex *tau, double _Complex *c, const int *ldc,
    double _Complex *work, const int *lwork, int *info);

KML_EXPORT void cpotrf_(const char *uplo, const int *n, float _Complex *a, const int *lda, int *info);

KML_EXPORT void dpotrf_(const char *uplo, const int *n, double *a, const int *lda, int *info);

KML_EXPORT void spotrf_(const char *uplo, const int *n, float *a, const int *lda, int *info);

KML_EXPORT void zpotrf_(const char *uplo, const int *n, double _Complex *a, const int *lda, int *info);

KML_EXPORT float clange_(const char *norm, const int *m, const int *n, const float _Complex *a, const int *lda,
    float *work);

KML_EXPORT double dlange_(const char *norm, const int *m, const int *n, const double *a, const int *lda, double *work);

KML_EXPORT float slange_(const char *norm, const int *m, const int *n, const float *a, const int *lda, float *work);

KML_EXPORT double zlange_(const char *norm, const int *m, const int *n, const double _Complex *a, const int *lda,
    double *work);

KML_EXPORT void claset_(const char *uplo, const int *m, const int *n, const float _Complex *alpha,
    const float _Complex *beta, float _Complex *a, const int *lda);

KML_EXPORT void dlaset_(const char *uplo, const int *m, const int *n, const double *alpha, const double *beta,
    double *a, const int *lda);

KML_EXPORT void slaset_(const char *uplo, const int *m, const int *n, const float *alpha, const float *beta, float *a,
    const int *lda);

KML_EXPORT void zlaset_(const char *uplo, const int *m, const int *n, const double _Complex *alpha,
    const double _Complex *beta, double _Complex *a, const int *lda);

KML_EXPORT void clacpy_(const char *uplo, const int *m, const int *n, const float _Complex *a, const int *lda,
    float _Complex *b, const int *ldb);

KML_EXPORT void dlacpy_(const char *uplo, const int *m, const int *n, const double *a, const int *lda, double *b,
    const int *ldb);

KML_EXPORT void slacpy_(const char *uplo, const int *m, const int *n, const float *a, const int *lda, float *b,
    const int *ldb);

KML_EXPORT void zlacpy_(const char *uplo, const int *m, const int *n, const double _Complex *a, const int *lda,
    double _Complex *b, const int *ldb);

KML_EXPORT void claswp_(const int *n, float _Complex *a, const int *lda, const int *k1, const int *k2, const int *ipiv,
    const int *incx);

KML_EXPORT void dlaswp_(const int *n, double *a, const int *lda, const int *k1, const int *k2, const int *ipiv,
    const int *incx);

KML_EXPORT void slaswp_(const int *n, float *a, const int *lda, const int *k1, const int *k2, const int *ipiv,
    const int *incx);

KML_EXPORT void zlaswp_(const int *n, double _Complex *a, const int *lda, const int *k1, const int *k2, const int *ipiv,
    const int *incx);

KML_EXPORT void csptrf_(const char *uplo, const int *n, float _Complex *ap, int *ipiv, int *info);

KML_EXPORT void dsptrf_(const char *uplo, const int *n, double *ap, int *ipiv, int *info);

KML_EXPORT void ssptrf_(const char *uplo, const int *n, float *ap, int *ipiv, int *info);

KML_EXPORT void zsptrf_(const char *uplo, const int *n, double _Complex *ap, int *ipiv, int *info);

KML_EXPORT void chptrf_(const char *uplo, const int *n, float _Complex *ap, int *ipiv, int *info);

KML_EXPORT void zhptrf_(const char *uplo, const int *n, double _Complex *ap, int *ipiv, int *info);

KML_EXPORT void cgesv_(const int *n, const int *nrhs, float _Complex *a, const int *lda, int *ipiv, float _Complex *b,
    const int *ldb, int *info);

KML_EXPORT void dgesv_(const int *n, const int *nrhs, double *a, const int *lda, int *ipiv, double *b, const int *ldb,
    int *info);

KML_EXPORT void sgesv_(const int *n, const int *nrhs, float *a, const int *lda, int *ipiv, float *b, const int *ldb,
    int *info);

KML_EXPORT void zgesv_(const int *n, const int *nrhs, double _Complex *a, const int *lda, int *ipiv, double _Complex *b,
    const int *ldb, int *info);

KML_EXPORT void cptsv_(const int *n, const int *nrhs, float *d, float _Complex *e, float _Complex *b, const int *ldb,
    int *info);

KML_EXPORT void dptsv_(const int *n, const int *nrhs, double *d, double *e, double *b, const int *ldb, int *info);

KML_EXPORT void sptsv_(const int *n, const int *nrhs, float *d, float *e, float *b, const int *ldb, int *info);

KML_EXPORT void zptsv_(const int *n, const int *nrhs, double *d, double _Complex *e, double _Complex *b, const int *ldb,
    int *info);

KML_EXPORT void cpttrf_(const int *n, float *d, float _Complex *e, int *info);

KML_EXPORT void dpttrf_(const int *n, double *d, double *e, int *info);

KML_EXPORT void spttrf_(const int *n, float *d, float *e, int *info);

KML_EXPORT void zpttrf_(const int *n, double *d, double _Complex *e, int *info);

KML_EXPORT void cpttrs_(const char *uplo, const int *n, const int *nrhs, const float *d, const float _Complex *e,
    float _Complex *b, const int *ldb, int *info);

KML_EXPORT void dpttrs_(const int *n, const int *nrhs, const double *d, const double *e, double *b, const int *ldb,
    int *info);

KML_EXPORT void spttrs_(const int *n, const int *nrhs, const float *d, const float *e, float *b, const int *ldb,
    int *info);

KML_EXPORT void zpttrs_(const char *uplo, const int *n, const int *nrhs, const double *d, const double _Complex *e,
    double _Complex *b, const int *ldb, int *info);

KML_EXPORT void cptts2_(const int *iuplo, const int *n, const int *nrhs, const float *d, const float _Complex *e,
    float _Complex *b, const int *ldb);

KML_EXPORT void dptts2_(const int *n, const int *nrhs, const double *d, const double *e, double *b, const int *ldb);

KML_EXPORT void sptts2_(const int *n, const int *nrhs, const float *d, const float *e, float *b, const int *ldb);

KML_EXPORT void zptts2_(const int *uplo, const int *n, const int *nrhs, const double *d, const double _Complex *e,
    double _Complex *b, const int *ldb);

KML_EXPORT void cpotri_(const char *uplo, const int *n, float _Complex *a, const int *lda, int *info);

KML_EXPORT void dpotri_(const char *uplo, const int *n, double *a, const int *lda, int *info);

KML_EXPORT void spotri_(const char *uplo, const int *n, float *a, const int *lda, int *info);

KML_EXPORT void zpotri_(const char *uplo, const int *n, double _Complex *a, const int *lda, int *info);

KML_EXPORT void chetrd_(const char *uplo, const int *n, float _Complex *a, const int *lda, float *d, float *e,
    float _Complex *tau, float _Complex *work, const int *lwork, int *info);

KML_EXPORT void dsytrd_(const char *uplo, const int *n, double *a, const int *lda, double *d, double *e, double *tau,
    double *work, const int *lwork, int *info);

KML_EXPORT void ssytrd_(const char *uplo, const int *n, float *a, const int *lda, float *d, float *e, float *tau,
    float *work, const int *lwork, int *info);

KML_EXPORT void zhetrd_(const char *uplo, const int *n, double _Complex *a, const int *lda, double *d, double *e,
    double _Complex *tau, double _Complex *work, const int *lwork, int *info);

KML_EXPORT void cposv_(const char *uplo, const int *n, const int *nrhs, float _Complex *a, const int *lda,
    float _Complex *b, const int *ldb, int *info);

KML_EXPORT void dposv_(const char *uplo, const int *n, const int *nrhs, double *a, const int *lda, double *b,
    const int *ldb, int *info);

KML_EXPORT void sposv_(const char *uplo, const int *n, const int *nrhs, float *a, const int *lda, float *b,
    const int *ldb, int *info);

KML_EXPORT void zposv_(const char *uplo, const int *n, const int *nrhs, double _Complex *a, const int *lda,
    double _Complex *b, const int *ldb, int *info);

KML_EXPORT void dsyev_(const char *jobz, const char *uplo, const int *n, double *a, const int *lda, double *w,
    double *work, const int *lwork, int *info);

KML_EXPORT void ssyev_(const char *jobz, const char *uplo, const int *n, float *a, const int *lda, float *w,
    float *work, const int *lwork, int *info);

KML_EXPORT void cheev_(const char *jobz, const char *uplo, const int *n, float _Complex *a, const int *lda, float *w,
    float _Complex *work, const int *lwork, float *rwork, int *info);

KML_EXPORT void zheev_(const char *jobz, const char *uplo, const int *n, double _Complex *a, const int *lda, double *w,
    double _Complex *work, const int *lwork, double *rwork, int *info);

KML_EXPORT void dsyevd_(const char *jobz, const char *uplo, const int *n, double *a, const int *lda, double *w,
    double *work, const int *lwork, int *iwork, const int *liwork, int *info);

KML_EXPORT void ssyevd_(const char *jobz, const char *uplo, const int *n, float *a, const int *lda, float *w,
    float *work, const int *lwork, int *iwork, const int *liwork, int *info);

KML_EXPORT void cheevd_(const char *jobz, const char *uplo, const int *n, float _Complex *a, const int *lda, float *w,
    float _Complex *work, const int *lwork, float *rwork, const int *lrwork, int *iwork, const int *liwork, int *info);

KML_EXPORT void zheevd_(const char *jobz, const char *uplo, const int *n, double _Complex *a, const int *lda, double *w,
    double _Complex *work, const int *lwork, double *rwork, const int *lrwork, int *iwork, const int *liwork,
    int *info);

KML_EXPORT void cgttrf_(const int *n, float _Complex *dl, float _Complex *d, float _Complex *du, float _Complex *du2,
    int *ipiv, int *info);

KML_EXPORT void dgttrf_(const int *n, double *dl, double *d, double *du, double *du2, int *ipiv, int *info);

KML_EXPORT void sgttrf_(const int *n, float *dl, float *d, float *du, float *du2, int *ipiv, int *info);

KML_EXPORT void zgttrf_(const int *n, double _Complex *dl, double _Complex *d, double _Complex *du,
    double _Complex *du2, int *ipiv, int *info);

KML_EXPORT void cgttrs_(const char *trans, const int *n, const int *nrhs, const float _Complex *dl,
    const float _Complex *d, const float _Complex *du, const float _Complex *du2, const int *ipiv, float _Complex *b,
    const int *ldb, int *info);

KML_EXPORT void dgttrs_(const char *trans, const int *n, const int *nrhs, const double *dl, const double *d,
    const double *du, const double *du2, const int *ipiv, double *b, const int *ldb, int *info);

KML_EXPORT void sgttrs_(const char *trans, const int *n, const int *nrhs, const float *dl, const float *d,
    const float *du, const float *du2, const int *ipiv, float *b, const int *ldb, int *info);

KML_EXPORT void zgttrs_(const char *trans, const int *n, const int *nrhs, const double _Complex *dl,
    const double _Complex *d, const double _Complex *du, const double _Complex *du2, const int *ipiv,
    double _Complex *b, const int *ldb, int *info);

KML_EXPORT void cgtsv_(const int *n, const int *nrhs, float _Complex *dl, float _Complex *d, float _Complex *du,
    float _Complex *b, const int *ldb, int *info);

KML_EXPORT void dgtsv_(const int *n, const int *nrhs, double *dl, double *d, double *du, double *b, const int *ldb,
    int *info);

KML_EXPORT void sgtsv_(const int *n, const int *nrhs, float *dl, float *d, float *du, float *b, const int *ldb,
    int *info);

KML_EXPORT void zgtsv_(const int *n, const int *nrhs, double _Complex *dl, double _Complex *d, double _Complex *du,
    double _Complex *b, const int *ldb, int *info);

KML_EXPORT void clasr_(const char *side, const char *pivot, const char *direct, const int *m, const int *n,
    const float *c, const float *s, float _Complex *a, const int *lda);

KML_EXPORT void slasr_(const char *side, const char *pivot, const char *direct, const int *m, const int *n,
    const float *c, const float *s, float *a, const int *lda);

KML_EXPORT void dlasr_(const char *side, const char *pivot, const char *direct, const int *m, const int *n,
    const double *c, const double *s, double *a, const int *lda);

KML_EXPORT void zlasr_(const char *side, const char *pivot, const char *direct, const int *m, const int *n,
    const double *c, const double *s, double _Complex *a, const int *lda);
KML_EXPORT void cpotrs_(const char *uplo, const int *n, const int *nrhs, const float _Complex *a, const int *lda,
    float _Complex *b, const int *ldb, int *info);

KML_EXPORT void dpotrs_(const char *uplo, const int *n, const int *nrhs, const double *a, const int *lda, double *b,
    const int *ldb, int *info);

KML_EXPORT void spotrs_(const char *uplo, const int *n, const int *nrhs, const float *a, const int *lda, float *b,
    const int *ldb, int *info);

KML_EXPORT void zpotrs_(const char *uplo, const int *n, const int *nrhs, const double _Complex *a, const int *lda,
    double _Complex *b, const int *ldb, int *info);

KML_EXPORT void dsytrd_2stage_(const char *vect, const char *uplo, const int *n, double *a, const int *lda, double *d,
    double *e, double *tau, double *hous2, const int *lhous2, double *work, const int *lwork, int *info);

KML_EXPORT void ssytrd_2stage_(const char *vect, const char *uplo, const int *n, float *a, const int *lda, float *d,
    float *e, float *tau, float *hous2, const int *lhous2, float *work, const int *lwork, int *info);

KML_EXPORT void chetrd_2stage_(const char *vect, const char *uplo, const int *n, float _Complex *a, const int *lda,
    float *d, float *e, float _Complex *tau, float _Complex *hous2, const int *lhous2, float _Complex *work,
    const int *lwork, int *info);

KML_EXPORT void zhetrd_2stage_(const char *vect, const char *uplo, const int *n, double _Complex *a, const int *lda,
    double *d, double *e, double _Complex *tau, double _Complex *hous2, const int *lhous2, double _Complex *work,
    const int *lwork, int *info);

KML_EXPORT void ctrtrs_(const char *uplo, const char *trans, const char *diag, const int *n, const int *nrhs,
    const float _Complex *a, const int *lda, float _Complex *b, const int *ldb, int *info);

KML_EXPORT void dtrtrs_(const char *uplo, const char *trans, const char *diag, const int *n, const int *nrhs,
    const double *a, const int *lda, double *b, const int *ldb, int *info);

KML_EXPORT void strtrs_(const char *uplo, const char *trans, const char *diag, const int *n, const int *nrhs,
    const float *a, const int *lda, float *b, const int *ldb, int *info);

KML_EXPORT void ztrtrs_(const char *uplo, const char *trans, const char *diag, const int *n, const int *nrhs,
    const double _Complex *a, const int *lda, double _Complex *b, const int *ldb, int *info);

KML_EXPORT void clascl_(const char *type, const int *kl, const int *ku, const float *cfrom, const float *cto,
    const int *m, const int *n, float _Complex *a, const int *lda, int *info);

KML_EXPORT void dlascl_(const char *type, const int *kl, const int *ku, const double *cfrom, const double *cto,
    const int *m, const int *n, double *a, const int *lda, int *info);

KML_EXPORT void slascl_(const char *type, const int *kl, const int *ku, const float *cfrom, const float *cto,
    const int *m, const int *n, float *a, const int *lda, int *info);

KML_EXPORT void zlascl_(const char *type, const int *kl, const int *ku, const double *cfrom, const double *cto,
    const int *m, const int *n, double _Complex *a, const int *lda, int *info);

KML_EXPORT void ctrtri_(const char *uplo, const char *diag, const int *n, float _Complex *a, const int *lda, int *info);

KML_EXPORT void dtrtri_(const char *uplo, const char *diag, const int *n, double *a, const int *lda, int *info);

KML_EXPORT void strtri_(const char *uplo, const char *diag, const int *n, float *a, const int *lda, int *info);

KML_EXPORT void ztrtri_(const char *uplo, const char *diag, const int *n, double _Complex *a, const int *lda,
    int *info);

typedef struct KMLVersion KLAPACKVersion;
KML_EXPORT int KLAPACKGetVersion(KLAPACKVersion *ver);

#ifdef __cplusplus
}
#endif

#endif /* KLAPACK_EXTERNAL_H */
