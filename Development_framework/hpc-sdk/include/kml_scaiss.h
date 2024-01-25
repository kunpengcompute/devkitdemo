/*******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Part of KML library
 * Author: KML
 * Create: 2022
 ******************************************************************************/

#ifndef KML_SCAISS_H_INCLUDED
#define KML_SCAISS_H_INCLUDED

#include <mpi.h>
#include "kml_export.h"
#include "kml_scasolver_defs.h"

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
 * @param handle - SCAISS matrix handle
 * @param n - matrix size
 * @param nstripes - the number of strips stored within the callee MPI process memory
 * @param stripeWidth - array of nstripes elements, i-th element is the number of rows in i-th matrix strip
 * @param stripeRow - array of nstripes elements, i-th element is the global number of the first row of the i-th matrix
 * strip
 * @param a - non-zero elements of the input matrix strip in CSR format
 * @param ja - column indices of the non-zero elements of the input matrix strip
 * @param ia - row indices for the input matrix strip in CSR format
 * @param comm - MPI communicator handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlScaissCgInitStripesSI(KmlScasolverTask **handle, int n, int nstripes, const int *stripeWidth,
    const int *stripeRow, const float * const *a, const int * const *ja, const int * const *ia, MPI_Comm comm);

/** analyze[reserved for future] - analyze sparse matrix and change
 * storage representation if required.
 * @ingroup cg_spd_si
 *
 * @param handle - SCAISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlScaissCgAnalyzeSI(KmlScasolverTask **handle);

/** factorize[reserved for future] - compute preconditioner if required.
 * @ingroup cg_spd_si
 *
 * @param handle - SCAISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlScaissCgFactorizeSI(KmlScasolverTask **handle);

/** solve - solve system $Ax = b$ with `n-by-nb` matrices `x` and `b`.
 * @ingroup cg_spd_si
 *
 * @param handle - SCAISS handle
 * @param nb - number of right-hand side vectors in `b`
 * @param x - (dense matrix) solutions on output, initial guess on input
 * @param ldx - leading dimension of `x`
 * @param b - (dense matrix) right hand side
 * @param ldb - leading dimension of `b`
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlScaissCgSolveSI(KmlScasolverTask **handle, int nb, float *x, int ldx, const float *b, int ldb);

/** clean - clean internal data structure.
 * @ingroup cg_spd_si
 *
 * @param handle - SCAISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlScaissCgCleanSI(KmlScasolverTask **handle);

/** setup solver-agnostic user preconditioner.
 * @ingroup cg_spd_si
 *
 * @param handle - SCAISS handle
 * @param ustruct - user data
 * @param fptr - user-provided preconditioner computing $x=M^{-1}x$
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlScaissCgSetUserPreconditionerSI(KmlScasolverTask **handle, void *ustruct,
    int (*fptr)(void *ustruct, float *x));

/** Set an integer parameter in IssCgSI handle
 * @ingroup cg_spd_si
 *
 * @param handle SCAISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlScaissCgSetSII(KmlScasolverTask **handle, enum KmlSolverParam param, const int *data, int nd);

/** Set a floating point parameter in IssCgSI handle
 * @ingroup cg_spd_si
 *
 * @param handle SCAISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlScaissCgSetSIS(KmlScasolverTask **handle, enum KmlSolverParam param, const float *data, int nd);

/** Get an integer parameter in IssCgSI handle
 * @ingroup cg_spd_si
 *
 * @param handle SCAISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlScaissCgGetSII(KmlScasolverTask **handle, enum KmlSolverParam param, int *data, int nd);

/** Get a floating point parameter in IssCgSI handle
 * @ingroup cg_spd_si
 *
 * @param handle SCAISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlScaissCgGetSIS(KmlScasolverTask **handle, enum KmlSolverParam param, float *data, int nd);
#endif // OMIT_CG_SPD_SI

#ifndef OMIT_CG_SPD_DI
/**
 * @defgroup cg_spd_di Conjugate Gradient routines for SPD matrix, double
 * precision, 32-bit indexing
 ******************************************************************************/

/** init - add sparse matrix.
 * @ingroup cg_spd_di
 *
 * @param handle - SCAISS matrix handle
 * @param n - matrix size
 * @param nstripes - the number of strips stored within the callee MPI process memory
 * @param stripeWidth - array of nstripes elements, i-th element is the number of rows in i-th matrix strip
 * @param stripeRow - array of nstripes elements, i-th element is the global number of the first row of the i-th matrix
 * strip
 * @param a - non-zero elements of the input matrix strip in CSR format
 * @param ja - column indices of the non-zero elements of the input matrix strip
 * @param ia - row indices for the input matrix strip in CSR format
 * @param comm - MPI communicator handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlScaissCgInitStripesDI(KmlScasolverTask **handle, int n, int nstripes, const int *stripeWidth,
    const int *stripeRow, const double * const *a, const int * const *ja, const int * const *ia, MPI_Comm comm);

/** analyze[reserved for future] - analyze sparse matrix and change
 * storage representation if required.
 * @ingroup cg_spd_di
 *
 * @param handle - SCAISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlScaissCgAnalyzeDI(KmlScasolverTask **handle);

/** factorize[reserved for future] - compute preconditioner if required.
 * @ingroup cg_spd_di
 *
 * @param handle - SCAISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlScaissCgFactorizeDI(KmlScasolverTask **handle);

/** solve - solve system $Ax = b$ with `n-by-nb` matrices `x` and `b`.
 * @ingroup cg_spd_di
 *
 * @param handle - SCAISS handle
 * @param nb - number of right-hand side vectors in `b`
 * @param x - (dense matrix) solutions on output, initial guess on input
 * @param ldx - leading dimension of `x`
 * @param b - (dense matrix) right hand side
 * @param ldb - leading dimension of `b`
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlScaissCgSolveDI(KmlScasolverTask **handle, int nb, double *x, int ldx, const double *b, int ldb);

/** clean - clean internal data structure.
 * @ingroup cg_spd_di
 *
 * @param handle - SCAISS handle
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlScaissCgCleanDI(KmlScasolverTask **handle);

/** setup solver-agnostic user preconditioner.
 * @ingroup cg_spd_di
 *
 * @param handle - SCAISS handle
 * @param ustruct - user data
 * @param fptr - user-provided preconditioner computing $x=M^{-1}x$
 * @return a KmlSolverStatus value
 */
KML_EXPORT int KmlScaissCgSetUserPreconditionerDI(KmlScasolverTask **handle, void *ustruct,
    int (*fptr)(void *ustruct, double *x));

/** Set an integer parameter in IssCgDI handle
 * @ingroup cg_spd_di
 *
 * @param handle SCAISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlScaissCgSetDII(KmlScasolverTask **handle, enum KmlSolverParam param, const int *data, int nd);

/** Set a floating point parameter in IssCgDI handle
 * @ingroup cg_spd_di
 *
 * @param handle SCAISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlScaissCgSetDID(KmlScasolverTask **handle, enum KmlSolverParam param, const double *data, int nd);

/** Get an integer parameter in IssCgDI handle
 * @ingroup cg_spd_di
 *
 * @param handle SCAISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlScaissCgGetDII(KmlScasolverTask **handle, enum KmlSolverParam param, int *data, int nd);

/** Get a floating point parameter in IssCgDI handle
 * @ingroup cg_spd_di
 *
 * @param handle SCAISS handle
 * @param param Selector of the parameter
 * @param data Values of the parameter
 * @param nd Number of the values
 * @return Error code of the last error
 */
KML_EXPORT int KmlScaissCgGetDID(KmlScasolverTask **handle, enum KmlSolverParam param, double *data, int nd);
#endif // OMIT_CG_SPD_DI

#if defined(__cplusplus)
}
#endif

#endif // KML_SCAISS_H_INCLUDED
