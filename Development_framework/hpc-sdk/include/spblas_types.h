/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Sparse BLAS数据类型定义
 * Author: KML
 * Create: 2020-4-21
 */

#ifndef SPBLAS_TYPES_H
#define SPBLAS_TYPES_H

typedef struct {
    float real;
    float imag;
} KML_Complex8;

typedef struct {
    double real;
    double imag;
} KML_Complex16;

#ifdef KML_ILP64
typedef unsigned long UNSIGNED_KML_INT;
typedef long KML_INT;

/* maximum value of KML_INT，preventing overflow caused by multiplying KML_INT by 256 */
#define MAX_KML_INT 0x3FFFFFFFFFFFFF
#else
typedef unsigned int UNSIGNED_KML_INT;
typedef int KML_INT;

/* maximum value of KML_INT minus 2，preventing overflow caused by applications of memory to rows+2 */
#define MAX_KML_INT 0x7FFFFFFD
#endif
typedef long KML_INT64;

/* status of the routines */
typedef enum {
    KML_SPARSE_STATUS_SUCCESS = 0,          /* the operation was successful */
    KML_SPARSE_STATUS_NOT_INITIALIZED = 1,  /* empty handle or matrix arrays */
    KML_SPARSE_STATUS_ALLOC_FAILED = 2,     /* internal error: memory allocation failed */
    KML_SPARSE_STATUS_INVALID_VALUE = 3,    /* invalid input value */
    KML_SPARSE_STATUS_EXECUTION_FAILED = 4, /* e.g. 0-diagonal element for triangular solver, etc. */
    KML_SPARSE_STATUS_INTERNAL_ERROR = 5,   /* internal error */
    KML_SPARSE_STATUS_NOT_SUPPORTED = 6     /* e.g. operation for double precision doesn't support other types */
} kml_sparse_status_t;

/* sparse matrix operations */
typedef enum {
    KML_SPARSE_OPERATION_NON_TRANSPOSE = 0,
    KML_SPARSE_OPERATION_TRANSPOSE = 1,
    KML_SPARSE_OPERATION_CONJUGATE_TRANSPOSE = 2,
    KML_SPARSE_OPERATION_NUM
} kml_sparse_operation_t;

/* supported matrix types */
typedef enum {
    KML_SPARSE_MATRIX_TYPE_GENERAL = 0,   /*    General case                    */
    KML_SPARSE_MATRIX_TYPE_SYMMETRIC = 1, /*    Triangular part of              */
    KML_SPARSE_MATRIX_TYPE_HERMITIAN = 2, /*    the matrix is to be processed   */
    KML_SPARSE_MATRIX_TYPE_TRIANGULAR = 3,
    KML_SPARSE_MATRIX_TYPE_DIAGONAL = 4, /* diagonal matrix; only diagonal elements will be processed */
    KML_SPARSE_MATRIX_TYPE_BLOCK_TRIANGULAR = 5,
    KML_SPARSE_MATRIX_TYPE_BLOCK_DIAGONAL = 6, /* block-diagonal matrix; only diagonal blocks will be processed */
    KML_SPARSE_MATRIX_TYPE_NUM
} kml_sparse_matrix_type_t;

/* sparse matrix indexing */
typedef enum {
    KML_SPARSE_INDEX_BASE_ZERO = 0,
    KML_SPARSE_INDEX_BASE_ONE = 1
} kml_sparse_index_base_t;

#define KML_SPARSE_INDEX_NUM 2

/*
 * applies to triangular matrices only ( KML_SPARSE_MATRIX_TYPE_SYMMETRIC, KML_SPARSE_MATRIX_TYPE_HERMITIAN,
 * KML_SPARSE_MATRIX_TYPE_TRIANGULAR )
 */
typedef enum {
    KML_SPARSE_FILL_MODE_LOWER = 0, /* lower triangular part of the matrix is stored */
    KML_SPARSE_FILL_MODE_UPPER = 1  /* upper triangular part of the matrix is stored */
} kml_sparse_fill_mode_t;
#define KML_SPARSE_FILL_MODE_NUM 2

typedef enum {
    KML_SPARSE_NON_ORDER = 0,
    KML_SPARSE_ORDER = 1
} kml_sparse_order_t;

/* applies to triangular matrices only */
typedef enum {
    KML_SPARSE_DIAG_NON_UNIT = 0, /* triangular matrix with non-unit diagonal */
    KML_SPARSE_DIAG_UNIT = 1      /* triangular matrix with unit diagonal */
} kml_sparse_diag_type_t;

#define KML_SPARSE_DIAG_TYPE_NUM 2

/*
 * applicable for Level 3 operations with dense matrices;
 * describes storage scheme for dense matrix (row major or column major)
 */
typedef enum {
    KML_SPARSE_LAYOUT_ROW_MAJOR = 0,   /* C-style */
    KML_SPARSE_LAYOUT_COLUMN_MAJOR = 1 /* Fortran-style */
} kml_sparse_layout_t;

#define KML_SPARSE_LAYOUT_NUM 2

/*************************************************************************************************/
/*   Opaque structure for sparse matrix in internal format, further D - means double precision   */
/*************************************************************************************************/
/* kml sparse matirx implement; */
typedef enum {
    KML_SPARSE_FORMAT_COO = 0,
    KML_SPARSE_FORMAT_CSR = 1,
    KML_SPARSE_FORMAT_CSC = 2,
    KML_SPARSE_FORMAT_BSR = 3,
    KML_SPARSE_FORMAT_SKY = 4,
    KML_SPARSE_FORMAT_DIA = 5,
    KML_SPARSE_FORMAT_ELL = 6
} kml_sparse_format_t;

#define KML_SPARSE_FORMAT_NUM 7

typedef enum {
    KML_SPARSE_DATATYPE_FLOAT = 0,
    KML_SPARSE_DATATYPE_DOUBLE = 1,
    KML_SPARSE_DATATYPE_FLOAT_COMPLEX = 2,
    KML_SPARSE_DATATYPE_DOUBLE_COMPLEX = 3
} kml_sparse_datatype_t;

#define KML_SPARSE_DATATYPE_NUM 4

typedef void *kml_internal_spmat;

typedef struct {
    kml_internal_spmat mat;
    kml_sparse_format_t format;
    kml_sparse_datatype_t datatype;
} kml_sparse_matrix_t;

/*
 * descriptor of main sparse matrix properties
 * type, matrix type: general, diagonal or triangular / symmetric / hermitian
 * mode, upper or lower triangular part of the matrix ( for triangular / symmetric / hermitian case)
 * diag, unit or non-unit diagonal ( for triangular / symmetric / hermitian case)
 */
struct kml_matrix_descr {
    kml_sparse_matrix_type_t type;
    kml_sparse_fill_mode_t mode;
    kml_sparse_diag_type_t diag;
};

#define REQUEST_UPDATE_ALL       0
#define REQUEST_UPDATE_IC        1
#define REQUEST_UPDATE_C_AND_JC  2

#define UNUSED(x) (void)(x)
#define KML_API __attribute__((visibility("default")))
#endif
