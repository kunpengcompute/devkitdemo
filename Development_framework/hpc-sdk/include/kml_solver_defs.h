/*******************************************************************************
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Part of KML library
 * Author: KML
 * Create: 2020
 ******************************************************************************/

#ifndef KML_SOLVER_DEFS_H_INCLUDED
#define KML_SOLVER_DEFS_H_INCLUDED

/** Opaque structure with the data for the problem being solved */
typedef struct KmlSolverTask KmlSolverTask;

#if defined(__cplusplus)
extern "C" {
#endif

/** Error code returned by KML solver functions.
 *
 * @sa{KmlIssGetErrorText, KmlIssClearError}
 */
typedef enum KmlSolverStatus {
    KMLSS_NO_ERROR = 0,
    KMLSS_NONZERO_INDEXING = 1,
    KMLSS_MISSING_DIAGONAL_ELEMENT = 2,
    KMLSS_ZERO_DIAGONAL_ELEMENT = 3,
    KMLSS_NO_MEMORY = 4,
    KMLSS_NULL_ARGUMENT = 5,
    KMLSS_BAD_DATA_SIZE = 6,
    KMLSS_BAD_DATA = 7, /* such as inconsistent permutation */
    KMLSS_BAD_SELECTOR = 8,
    KMLSS_BAD_N = 9,
    KMLSS_BAD_NB = 10,
    KMLSS_BAD_LDX = 11,
    KMLSS_BAD_LDB = 12,
    KMLSS_BAD_HANDLE = 13,
    KMLSS_BAD_PRECONDITIONER = 14,
    KMLSS_INVALID_CALL_ORDER = 15,
    KMLSS_BAD_MATRIX_FORMAT = 16,
    KMLSS_REORDERING_PROBLEM = 1001,
    KMLSS_ZERO_PARTIAL_PIVOT = 1002,
    KMLSS_INTERNAL_ERROR = 1000001,
    KMLSS_NOT_IMPLEMENTED = 1000002,
} KmlSolverStatus;

typedef enum KmlSolverParam {
    KMLSS_FILL_IN = 0,
    KMLSS_PERM = 1,
    KMLSS_REFINEMENT_MAX_STEPS = 2, /* iterative refinement steps */
    KMLSS_THRESHOLD = 3,
    KMLSS_MAX_ITERATION_COUNT = 4,
    KMLSS_RESTART_PARAM = 5,
    KMLSS_ITERATION_COUNT = 6,
    KMLSS_TOLERANCE = 7,
    KMLSS_INCREASE_ACCURACY = 8,
    KMLSS_PRECONDITIONER_TYPE = 9,
    KMLSS_ORTHOGONALIZATION_TYPE = 10,
    KMLSS_BOOST_THRESHOLD = 11,
    KMLSS_SCALING_TYPE = 12,
    KMLSS_MATRIX_FORMAT = 13,
    KMLSS_REFINEMENT_STEPS = 14,
    KMLSS_REFINEMENT_TOLERANCE_LEVEL = 15,
    KMLSS_REFINEMENT_RESIDUAL = 16,
    KMLSS_PIVOTING_THRESHOLD = 17,
    KMLSS_MATCHING_TYPE = 18,
} KmlSolverParam;

typedef enum KmlSolverPreconditionerType {
    KMLSS_NO_PC = 0,
    KMLSS_JACOBI = 1,
    KMLSS_BJACOBI = 2,
    KMLSS_SOR = 3,
    KMLSS_ILU = 4,
} KmlSolverPreconditionerType;

typedef enum KmlSolverPreconditionerParam {
    KMLSS_RELAXATION_FACTOR = 0,
    KMLSS_NUM_BLOCKS = 1,
    KMLSS_BLOCK_SIZES = 2,
    KMLSS_BLOCK_METHOD = 3,
} KmlSolverPreconditionerParam;

typedef enum KmlSolverOrthogonalizationMethod {
    KMLSS_GS = 1,
    KMLSS_MGS = 2,
} KmlSolverOrthogonalizationMethod;

typedef enum KmlSolverMatchingType {
    KMLSS_MATCHING_OFF = 0,
    KMLSS_MATCHING_HUNGARIAN = 1,
    KMLSS_MATCHING_AUTO = 2,
} KmlSolverMatchingType;

typedef enum KmlSolverScalingType {
    KMLSS_NO_SCALING = 0,
    KMLSS_ROWCOLUMN_SCALING = 1,
    KMLSS_MATCHBASED_SCALING = 2,
} KmlSolverScalingType;

typedef enum KmlSolverMatrixFormat {
    KMLSS_CSR = 1, /* compressed sparse row */
    KMLSS_ELL = 2, /* sliced ELL storage format (introduced by elliptic problem solver ELLPACK) */
} KmlSolverMatrixFormat;

#if defined(__cplusplus)
}
#endif

#endif
