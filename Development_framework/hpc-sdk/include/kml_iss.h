/*******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Part of KML library
 * Author: KML
 * Create: 2020
 ******************************************************************************/

#ifndef KML_ISS_H_INCLUDED
#define KML_ISS_H_INCLUDED

#include "kml_export.h"
#include "kml_solver_defs.h"

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef OMIT_CG_SPD_SI
/**
 * @defgroup cg_spd_si Conjugate Gradient routines for SPD matrix,
 * single precision, 32-bit indexing
 ******************************************************************************/

/** init - add sparse matrix.
 * @ingroup cg_spd_si
 *
 * @param handle - ISS handle
 * @param n - matrix size
 * @param a - non-zero elements of the matrix in CSR format
 * @param ja - column indices of the non-zero elements
 * @param ia - row indices for CSR format
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssCgInitSI(KmlSolverTask **handle, int n, const float *a, const int *ja, const int *ia);

/** analyze[reserved for future] - analyze sparse matrix and change
 * storage representation if required.
 * @ingroup cg_spd_si
 *
 * @param handle - ISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssCgAnalyzeSI(KmlSolverTask **handle);

/** factorize[reserved for future] - compute preconditioner if required.
 * @ingroup cg_spd_si
 *
 * @param handle - ISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssCgFactorizeSI(KmlSolverTask **handle);

/** solve - solve system $Ax = b$ with `n-by-nb` matrices `x` and `b`.
 * @ingroup cg_spd_si
 *
 * @param handle - ISS handle
 * @param nb - number of right-hand side vectors in `b`
 * @param x - (dense matrix) solutions on output, initial guess on input
 * @param ldx - leading dimension of `x`
 * @param b - (dense matrix) right hand side
 * @param ldb - leading dimension of `b`
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssCgSolveSI(KmlSolverTask **handle, int nb, float *x, int ldx, const float *b, int ldb);

/** clean - clean internal data structure.
 * @ingroup cg_spd_si
 *
 * @param handle - ISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssCgCleanSI(KmlSolverTask **handle);

/** setup solver-agnostic user preconditioner.
 * @ingroup cg_spd_si
 *
 * @param handle - ISS handle
 * @param ustruct - user data
 * @param fptr - user-provided preconditioner computing $x=M^{-1}x$
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssCgSetUserPreconditionerSI(KmlSolverTask **handle, void *ustruct,
    int (*fptr)(void *ustruct, float *x));

/** Set an integer parameter in IssCgSI handle
 * @ingroup cg_spd_si
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssCgSetSII(KmlSolverTask **handle, enum KmlSolverParam param, const int *data, int nd);

/** Set a floating point parameter in IssCgSI handle
 * @ingroup cg_spd_si
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssCgSetSIS(KmlSolverTask **handle, enum KmlSolverParam param, const float *data, int nd);

/** Get an integer parameter in IssCgSI handle
 * @ingroup cg_spd_si
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssCgGetSII(KmlSolverTask **handle, enum KmlSolverParam param, int *data, int nd);

/** Get a floating point parameter in IssCgSI handle
 * @ingroup cg_spd_si
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssCgGetSIS(KmlSolverTask **handle, enum KmlSolverParam param, float *data, int nd);
#endif // OMIT_CG_SPD_SI

#ifndef OMIT_GCR_SI
/**
 * @defgroup gcr_si Generalized Conjugate Residual routines for single precision
 * 32-bit indexed matrices
 ******************************************************************************/

/** init - add sparse matrix.
 * @ingroup gcr_si
 *
 * @param handle - ISS handle
 * @param n - matrix size
 * @param a - non-zero elements of the matrix in CSR format
 * @param ja - column indices of the non-zero elements
 * @param ia - row indices for CSR format
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssGcrInitSI(KmlSolverTask **handle, int n, const float *a, const int *ja, const int *ia);

/** analyze[reserved for future] - analyze sparse matrix and change storage
 * representation if required.
 * @ingroup gcr_si
 *
 * @param handle - ISS handle
 */
KML_EXPORT int KmlIssGcrAnalyzeSI(KmlSolverTask **handle);

/** factorize[reserved for future] - compute preconditioner if required.
 * @ingroup gcr_si
 *
 * @param handle - ISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssGcrFactorizeSI(KmlSolverTask **handle);

/** solve system $Ax=b$ where $x$ is initial guess on input, solution on output.
 * @ingroup gcr_si
 *
 * @param handle - ISS handle
 * @param nb - number of right-hand side vectors in `b`
 * @param x - (dense matrix) solutions on output, initial guess on input
 * @param ldx - leading dimension of `x`
 * @param b - (dense matrix) right hand side
 * @param ldb - leading dimension of `b`
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssGcrSolveSI(KmlSolverTask **handle, int nb, float *x, int ldx, const float *b, int ldb);

/** clean internal data structure.
 * @ingroup gcr_si
 *
 * @param handle - ISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssGcrCleanSI(KmlSolverTask **handle);

/** Set an integer parameter in IssGcrSI handle
 * @ingroup gcr_si
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssGcrSetSII(KmlSolverTask **handle, enum KmlSolverParam param, const int *data, int nd);

/** Set a floating point parameter in IssGcrSI handle
 * @ingroup gcr_si
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssGcrSetSIS(KmlSolverTask **handle, enum KmlSolverParam param, const float *data, int nd);

/** Get an integer parameter in IssGcrSI handle
 * @ingroup gcr_si
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssGcrGetSII(KmlSolverTask **handle, enum KmlSolverParam param, int *data, int nd);

/** Get a floating point parameter in IssGcrSI handle
 * @ingroup gcr_si
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssGcrGetSIS(KmlSolverTask **handle, enum KmlSolverParam param, float *data, int nd);
#endif // OMIT_GCR_SI

#ifndef OMIT_CG_SPD_DI
/**
 * @defgroup cg_spd_di Conjugate Gradient routines for SPD matrix, double
 * precision, 32-bit indexing
 ******************************************************************************/

/** init - add sparse matrix.
 * @ingroup cg_spd_di
 *
 * @param handle - ISS handle
 * @param n - matrix size
 * @param a - non-zero elements of the matrix in CSR format
 * @param ja - column indices of the non-zero elements
 * @param ia - row indices for CSR format
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssCgInitDI(KmlSolverTask **handle, int n, const double *a, const int *ja, const int *ia);

/** analyze[reserved for future] - analyze sparse matrix and change
 * storage representation if required.
 * @ingroup cg_spd_di
 *
 * @param handle - ISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssCgAnalyzeDI(KmlSolverTask **handle);

/** factorize[reserved for future] - compute preconditioner if required.
 * @ingroup cg_spd_di
 *
 * @param handle - ISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssCgFactorizeDI(KmlSolverTask **handle);

/** solve - solve system $Ax = b$ with `n-by-nb` matrices `x` and `b`.
 * @ingroup cg_spd_di
 *
 * @param handle - ISS handle
 * @param nb - number of right-hand side vectors in `b`
 * @param x - (dense matrix) solutions on output, initial guess on input
 * @param ldx - leading dimension of `x`
 * @param b - (dense matrix) right hand side
 * @param ldb - leading dimension of `b`
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssCgSolveDI(KmlSolverTask **handle, int nb, double *x, int ldx, const double *b, int ldb);

/** clean - clean internal data structure.
 * @ingroup cg_spd_di
 *
 * @param handle - ISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssCgCleanDI(KmlSolverTask **handle);

/** setup solver-agnostic user preconditioner.
 * @ingroup cg_spd_di
 *
 * @param handle - ISS handle
 * @param ustruct - user data
 * @param fptr - user-provided preconditioner computing $x=M^{-1}x$
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssCgSetUserPreconditionerDI(KmlSolverTask **handle, void *ustruct,
    int (*fptr)(void *ustruct, double *x));

/** Set an integer parameter in IssCgDI handle
 * @ingroup cg_spd_di
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssCgSetDII(KmlSolverTask **handle, enum KmlSolverParam param, const int *data, int nd);

/** Set a floating point parameter in IssCgDI handle
 * @ingroup cg_spd_di
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssCgSetDID(KmlSolverTask **handle, enum KmlSolverParam param, const double *data, int nd);

/** Get an integer parameter in IssCgDI handle
 * @ingroup cg_spd_di
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssCgGetDII(KmlSolverTask **handle, enum KmlSolverParam param, int *data, int nd);

/** Get a floating point parameter in IssCgDI handle
 * @ingroup cg_spd_di
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssCgGetDID(KmlSolverTask **handle, enum KmlSolverParam param, double *data, int nd);
#endif // OMIT_CG_SPD_DI

#ifndef OMIT_GCR_DI
/**
 * @defgroup gcr_di Generalized Conjugate Residual routines for double
 * precision 32-bit indexed matrices
 ******************************************************************************/

/** init - add sparse matrix.
 * @ingroup gcr_di
 *
 * @param handle - ISS handle
 * @param n - matrix size
 * @param a - non-zero elements of the matrix in CSR format
 * @param ja - column indices of the non-zero elements
 * @param ia - row indices for CSR format
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssGcrInitDI(KmlSolverTask **handle, int n, const double *a, const int *ja, const int *ia);

/** analyze[reserved for future] - analyze sparse matrix and change storage
 * representation if required.
 * @ingroup gcr_di
 *
 * @param handle - ISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssGcrAnalyzeDI(KmlSolverTask **handle);

/** factorize[reserved for future] - compute preconditioner if required.
 * @ingroup gcr_di
 *
 * @param handle - ISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssGcrFactorizeDI(KmlSolverTask **handle);

/** solve system $Ax=b$ where $x$ is initial guess on input, solution on output.
 * @ingroup gcr_di
 *
 * @param handle - ISS handle
 * @param nb - number of right-hand side vectors in `b`
 * @param x - (dense matrix) solutions on output, initial guess on input
 * @param ldx - leading dimension of `x`
 * @param b - (dense matrix) right hand side
 * @param ldb - leading dimension of `b`
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssGcrSolveDI(KmlSolverTask **handle, int nb, double *x, int ldx, const double *b, int ldb);

/** clean internal data structure.
 * @ingroup gcr_di
 *
 * @param handle - ISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlIssGcrCleanDI(KmlSolverTask **handle);

/** Set an integer parameter in IssGcrDI handle
 * @ingroup gcr_di
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssGcrSetDII(KmlSolverTask **handle, enum KmlSolverParam param, const int *data, int nd);

/** Set a floating point parameter in IssGcrDI handle
 * @ingroup gcr_di
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssGcrSetDID(KmlSolverTask **handle, enum KmlSolverParam param, const double *data, int nd);

/** Get an integer parameter in IssGcrDI handle
 * @ingroup gcr_di
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssGcrGetDII(KmlSolverTask **handle, enum KmlSolverParam param, int *data, int nd);

/** Get a floating point parameter in IssGcrDI handle
 * @ingroup gcr_di
 *
 * @param handle ISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssGcrGetDID(KmlSolverTask **handle, enum KmlSolverParam param, double *data, int nd);
#endif // OMIT_GCR_DI

#ifndef OMIT_ISS_MISC
/**
 * @defgroup iss_misc Support functions
 ******************************************************************************/

/** Get description of the last error returned for a handle.
 * @ingroup iss_misc
 *
 * @param handle ISS handle
 * @param text Array receiving the description as zero-terminated string
 * @param textSize Size of `text`
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssGetErrorText(KmlSolverTask **handle, char *text, int textSize);

/** Clear last error for the handle.
 * @ingroup iss_misc
 *
 * @param handle ISS handle
 * @return Error code of the last error
 */
KML_EXPORT int KmlIssClearError(KmlSolverTask **handle);
#endif // OMIT_ISS_MISC

#if defined(__cplusplus)
}
#endif

#endif // KML_ISS_H_INCLUDED
