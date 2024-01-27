/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: sparse blas接口声明
 * Author: wangyuanxiang
 * Create: 2020-7-30
 */

#ifndef KML_SPBLAS_H
#define KML_SPBLAS_H
#include "spblas_types.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define VERSION_STRUCT_LEN 100
typedef struct {
    char productName[VERSION_STRUCT_LEN];
    char productVersion[VERSION_STRUCT_LEN];
    char componentName[VERSION_STRUCT_LEN];
    char componentVersion[VERSION_STRUCT_LEN];
    char componentAppendInfo[VERSION_STRUCT_LEN];
    char softwareName[VERSION_STRUCT_LEN];
    char softwareVersion[VERSION_STRUCT_LEN];
}KSPBLASVersion;
int KSPBLASGetVersion(KSPBLASVersion *ver);

/*
 * 矩阵与向量计算,y := alpha*opt(A)*x + beta*y
 * @opt, 转置情况
 * @m， 矩阵A行数
 * @k, 矩阵A列数
 * @alpha, 标量alpha
 * @matdescra, 矩阵的操作属性，具体参考 matdescra说明
 * @val, values数组，存储矩阵A中非零元素的值
 * @indx, columns数组，表示矩阵A中非零元素所在列索引
 * @pntrb, 长度为m的数组，包含矩阵A的行索引，pntrb[i] - pntrb[0]表示第i行第一个非零元素在val和indx数组内的下标。
 * @pntre, 长度为m的数组，包含矩阵A的行索引，pntre[i] - pntrb[0]-1表示第i行最后一个非零元素在val和indx数组内的下标。
 * @x，x向量数组
 * @beta，标量beta
 * @y，y向量数组，计算后会更新y向量的值。
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_scsrmv(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT k,
    const float alpha, const char *matdescra, const float *val, const KML_INT *indx, const KML_INT *pntrb,
    const KML_INT *pntre, const float *x, const float beta, float *y);
kml_sparse_status_t kml_sparse_dcsrmv(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT k,
    const double alpha, const char *matdescra, const double *val, const KML_INT *indx, const KML_INT *pntrb,
    const KML_INT *pntre, const double *x, const double beta, double *y);
kml_sparse_status_t kml_sparse_ccsrmv(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT k,
    const KML_Complex8 alpha, const char *matdescra, const KML_Complex8 *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const KML_Complex8 *x, const KML_Complex8 beta, KML_Complex8 *y);
kml_sparse_status_t kml_sparse_zcsrmv(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT k,
    const KML_Complex16 alpha, const char *matdescra, const KML_Complex16 *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const KML_Complex16 *x, const KML_Complex16 beta, KML_Complex16 *y);

kml_sparse_status_t kml_sparse_scscmv(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT k,
    const float alpha, const char *matdescra, const float *val, const KML_INT *indx, const KML_INT *pntrb,
    const KML_INT *pntre, const float *x, const float beta, float *y);
kml_sparse_status_t kml_sparse_dcscmv(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT k,
    const double alpha, const char *matdescra, const double *val, const KML_INT *indx, const KML_INT *pntrb,
    const KML_INT *pntre, const double *x, const double beta, double *y);
kml_sparse_status_t kml_sparse_ccscmv(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT k,
    const KML_Complex8 alpha, const char *matdescra, const KML_Complex8 *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const KML_Complex8 *x, const KML_Complex8 beta, KML_Complex8 *y);
kml_sparse_status_t kml_sparse_zcscmv(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT k,
    const KML_Complex16 alpha, const char *matdescra, const KML_Complex16 *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const KML_Complex16 *x, const KML_Complex16 beta, KML_Complex16 *y);
/*
 * 求解三角矩阵方程组, opt(A)*y = alpha* x
 * @opt, 矩阵A转置情况
 * @m， 矩阵A行数
 * @alpha, 标量alpha
 * @matdescra, 矩阵的操作属性，具体参考 matdescra说明
 * @val, values数组，存储矩阵A中非零元素的值
 * @indx, columns数组，表示矩阵A中非零元素所在列索引
 * @pntrb, 长度为m的数组，包含矩阵A的行索引，pntrb[i] - pntrb[0]表示第i行第一个非零元素在val和indx数组内的下标。
 * @pntre, 长度为m的数组，包含矩阵A的行索引，pntre[i] - pntrb[0]-1表示第i行最后一个非零元素在val和indx数组内的下标。
 * @x，x向量数组
 * @y，y向量数组，计算后会更新y向量的值。
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_scsrsv(const kml_sparse_operation_t opt, const KML_INT m,
    const float alpha, const char *matdescra, const float *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const float *x, float *y);
kml_sparse_status_t kml_sparse_dcsrsv(const kml_sparse_operation_t opt, const KML_INT m,
    const double alpha, const char *matdescra, const double *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const double *x, double *y);
kml_sparse_status_t kml_sparse_ccsrsv(const kml_sparse_operation_t opt, const KML_INT m,
    const KML_Complex8 alpha, const char *matdescra, const KML_Complex8 *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const KML_Complex8 *x, KML_Complex8 *y);
kml_sparse_status_t kml_sparse_zcsrsv(const kml_sparse_operation_t opt, const KML_INT m,
    const KML_Complex16 alpha, const char *matdescra, const KML_Complex16 *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const KML_Complex16 *x, KML_Complex16 *y);

kml_sparse_status_t kml_sparse_scscsv(const kml_sparse_operation_t opt, const KML_INT m,
    const float alpha, const char *matdescra, const float *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const float *x, float *y);
kml_sparse_status_t kml_sparse_dcscsv(const kml_sparse_operation_t opt, const KML_INT m,
    const double alpha, const char *matdescra, const double *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const double *x, double *y);
kml_sparse_status_t kml_sparse_ccscsv(const kml_sparse_operation_t opt, const KML_INT m,
    const KML_Complex8 alpha, const char *matdescra, const KML_Complex8 *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const KML_Complex8 *x, KML_Complex8 *y);
kml_sparse_status_t kml_sparse_zcscsv(const kml_sparse_operation_t opt, const KML_INT m,
    const KML_Complex16 alpha, const char *matdescra, const KML_Complex16 *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const KML_Complex16 *x, KML_Complex16 *y);
/*
 * 矩阵与矩阵计算, C := alpha*opt(A)*B + beta*C，A为稀疏矩阵，B为稠密矩阵
 * @opt, 转置情况
 * @m， 矩阵A行数
 * @n， 矩阵C列数
 * @k， 矩阵A列数
 * @alpha, 标量alpha
 * @matdescra, 矩阵的操作属性，具体参考 matdescra说明
 * @val, values数组，存储矩阵A中非零元素的值
 * @indx, columns数组，表示矩阵A中非零元素所在列索引
 * @pntrb, 长度为m的数组，包含矩阵A的行索引，pntrb[i] - pntrb[0]表示第i行第一个非零元素在val和indx数组内的下标。
 * @pntre, 长度为m的数组，包含矩阵A的行索引，pntre[i] - pntrb[0]-1表示第i行最后一个非零元素在val和indx数组内的下标。
 * @b，矩阵b的数组
 * @ldb, 表示矩阵B的主/第二维度大小，由indexing决定
 * @beta，标量beta
 * @c，矩阵c数组，计算后会更新c矩阵的值。
 * @ldc，表示矩阵C的主/第二维度大小，由indexing决定
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_scsrmm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_INT k, const float alpha, const char *matdescra, const float *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const float *b, const KML_INT ldb,
    const float beta, float *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_dcsrmm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_INT k, const double alpha, const char *matdescra, const double *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const double *b, const KML_INT ldb,
    const double beta, double *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_ccsrmm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_INT k, const KML_Complex8 alpha, const char *matdescra, const KML_Complex8 *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const KML_Complex8 *b, const KML_INT ldb,
    const KML_Complex8 beta, KML_Complex8 *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_zcsrmm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_INT k, const KML_Complex16 alpha, const char *matdescra, const KML_Complex16 *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const KML_Complex16 *b, const KML_INT ldb,
    const KML_Complex16 beta, KML_Complex16 *c, const KML_INT ldc);

kml_sparse_status_t kml_sparse_scsrsm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const float alpha, const char *matdescra,
    const float *val, const KML_INT *indx, const KML_INT *pntrb, const KML_INT *pntre,
    const float *b, const KML_INT ldb, float *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_dcsrsm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const double alpha, const char *matdescra,
    const double *val, const KML_INT *indx, const KML_INT *pntrb, const KML_INT *pntre,
    const double *b, const KML_INT ldb, double *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_ccsrsm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_Complex8 alpha, const char *matdescra,
    const KML_Complex8 *val, const KML_INT *indx, const KML_INT *pntrb, const KML_INT *pntre,
    const KML_Complex8 *b, const KML_INT ldb, KML_Complex8 *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_zcsrsm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_Complex16 alpha, const char *matdescra,
    const KML_Complex16 *val, const KML_INT *indx, const KML_INT *pntrb, const KML_INT *pntre,
    const KML_Complex16 *b, const KML_INT ldb, KML_Complex16 *c, const KML_INT ldc);

/*
 * 稀疏矩阵与向量计算,y := opt(A)*x，采用CSR三数组存储，仅支持one-based indexing
 * @opt, 转置情况
 * @m， 行数
 * @a, values数组，存储矩阵A中非零元素的值
 * @ia, 包含矩阵A的行索引，ia[i] - 1表示第i行第一个非零元素在val和indx数组内的下标。
 * @ja, 表示矩阵A中非零元素所在的列索引
 * @x，x向量数组
 * @y，y向量数组，计算后会更新y向量的值。
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_scsrgemv(const kml_sparse_operation_t opt, const KML_INT m,
    const float *a, const KML_INT *ia, const KML_INT *ja, const float *x, float *y);
kml_sparse_status_t kml_sparse_dcsrgemv(const kml_sparse_operation_t opt, const KML_INT m, const double *a,
    const KML_INT *ia, const KML_INT *ja, const double *x, double *y);
kml_sparse_status_t kml_sparse_ccsrgemv(const kml_sparse_operation_t opt, const KML_INT m, const KML_Complex8 *a,
    const KML_INT *ia, const KML_INT *ja, const KML_Complex8 *x, KML_Complex8 *y);
kml_sparse_status_t kml_sparse_zcsrgemv(const kml_sparse_operation_t opt, const KML_INT m, const KML_Complex16 *a,
    const KML_INT *ia, const KML_INT *ja, const KML_Complex16 *x, KML_Complex16 *y);

/*
 * 矩阵与向量计算,y := opt(A)*x，A为上三角或下三角对称矩阵，采用CSR三数组存储，仅支持one-based indexing
 * @uplo, 指示矩阵A的上三角或者下三角被使用
 * @m， 矩阵A的行数
 * @a, values数组，存储矩阵A中非零元素的值
 * @ia, 包含矩阵A的行索引，ia[i] - 1表示第i行第一个非零元素在val和indx数组内的下标。
 * @ja, columns数组，用于表示矩阵A中非零元素所在的列索引。
 * @x，x向量数组
 * @y，y向量数组，计算后会更新y向量的值。
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_scsrsymv(const kml_sparse_fill_mode_t uplo, const KML_INT m,
    const float *a, const KML_INT *ia, const KML_INT *ja, const float *x, float *y);
kml_sparse_status_t kml_sparse_dcsrsymv(const kml_sparse_fill_mode_t uplo, const KML_INT m,
    const double *a, const KML_INT *ia, const KML_INT *ja, const double *x, double *y);
kml_sparse_status_t kml_sparse_ccsrsymv(const kml_sparse_fill_mode_t uplo, const KML_INT m,
    const KML_Complex8 *a, const KML_INT *ia, const KML_INT *ja, const KML_Complex8 *x, KML_Complex8 *y);
kml_sparse_status_t kml_sparse_zcsrsymv(const kml_sparse_fill_mode_t uplo, const KML_INT m,
    const KML_Complex16 *a, const KML_INT *ia, const KML_INT *ja, const KML_Complex16 *x, KML_Complex16 *y);

/*
 * 矩阵与向量计算,y := opt(A)*x，采用CSR三数组存储，仅支持zero-based indexing
 * @opt, 转置情况
 * @m， 行数
 * @a, values数组，存储矩阵A中非零元素的值
 * @ia, 包含矩阵A的行索引，ia[i]表示第i行第一个非零元素在val和indx数组内的下标。
 * @ja, 表示矩阵A中非零元素所在的列索引
 * @x，x向量数组
 * @y，y向量数组，计算后会更新y向量的值。
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_csparse_scsrgemv(const kml_sparse_operation_t opt, const KML_INT m,
    const float *a, const KML_INT *ia, const KML_INT *ja, const float *x, float *y);
kml_sparse_status_t kml_csparse_dcsrgemv(const kml_sparse_operation_t opt, const KML_INT m, const double *a,
    const KML_INT *ia, const KML_INT *ja, const double *x, double *y);
kml_sparse_status_t kml_csparse_ccsrgemv(const kml_sparse_operation_t opt, const KML_INT m, const KML_Complex8 *a,
    const KML_INT *ia, const KML_INT *ja, const KML_Complex8 *x, KML_Complex8 *y);
kml_sparse_status_t kml_csparse_zcsrgemv(const kml_sparse_operation_t opt, const KML_INT m, const KML_Complex16 *a,
    const KML_INT *ia, const KML_INT *ja, const KML_Complex16 *x, KML_Complex16 *y);

/*
 * 矩阵与向量计算,y := opt(A)*x，A为上三角或下三角对称矩阵，采用CSR三数组存储，仅支持zero-based indexing
 * @uplo, 指示矩阵A的上三角或者下三角被使用
 * @m， 矩阵A的行数
 * @a, values数组，存储矩阵A中非零元素的值
 * @ia, 包含矩阵A的行索引，ia表示第i行第一个非零元素在val和indx数组内的下标。
 * @ja, columns数组，用于表示矩阵A中非零元素所在的列索引。
 * @x，x向量数组
 * @y，y向量数组，计算后会更新y向量的值。
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_csparse_scsrsymv(const kml_sparse_fill_mode_t uplo, const KML_INT m,
    const float *a, const KML_INT *ia, const KML_INT *ja, const float *x, float *y);
kml_sparse_status_t kml_csparse_dcsrsymv(const kml_sparse_fill_mode_t uplo, const KML_INT m,
    const double *a, const KML_INT *ia, const KML_INT *ja, const double *x, double *y);
kml_sparse_status_t kml_csparse_ccsrsymv(const kml_sparse_fill_mode_t uplo, const KML_INT m,
    const KML_Complex8 *a, const KML_INT *ia, const KML_INT *ja, const KML_Complex8 *x, KML_Complex8 *y);
kml_sparse_status_t kml_csparse_zcsrsymv(const kml_sparse_fill_mode_t uplo, const KML_INT m,
    const KML_Complex16 *a, const KML_INT *ia, const KML_INT *ja, const KML_Complex16 *x, KML_Complex16 *y);

kml_sparse_status_t kml_sparse_scsrtrsv(const kml_sparse_fill_mode_t uplo, const kml_sparse_operation_t opt,
    const kml_sparse_diag_type_t diag, const KML_INT m, const float *a, const KML_INT *ia,
    const KML_INT *ja, const float *x, float *y);
kml_sparse_status_t kml_sparse_dcsrtrsv(const kml_sparse_fill_mode_t uplo, const kml_sparse_operation_t opt,
    const kml_sparse_diag_type_t diag, const KML_INT m, const double *a, const KML_INT *ia,
    const KML_INT *ja, const double *x, double *y);
kml_sparse_status_t kml_sparse_ccsrtrsv(const kml_sparse_fill_mode_t uplo, const kml_sparse_operation_t opt,
    const kml_sparse_diag_type_t diag, const KML_INT m, const KML_Complex8 *a, const KML_INT *ia,
    const KML_INT *ja, const KML_Complex8 *x, KML_Complex8 *y);
kml_sparse_status_t kml_sparse_zcsrtrsv(const kml_sparse_fill_mode_t uplo, const kml_sparse_operation_t opt,
    const kml_sparse_diag_type_t diag, const KML_INT m, const KML_Complex16 *a, const KML_INT *ia,
    const KML_INT *ja, const KML_Complex16 *x, KML_Complex16 *y);
kml_sparse_status_t kml_csparse_scsrtrsv(const kml_sparse_fill_mode_t uplo, const kml_sparse_operation_t opt,
    const kml_sparse_diag_type_t diag, const KML_INT m, const float *a, const KML_INT *ia,
    const KML_INT *ja, const float *x, float *y);
kml_sparse_status_t kml_csparse_dcsrtrsv(const kml_sparse_fill_mode_t uplo, const kml_sparse_operation_t opt,
    const kml_sparse_diag_type_t diag, const KML_INT m, const double *a, const KML_INT *ia,
    const KML_INT *ja, const double *x, double *y);
kml_sparse_status_t kml_csparse_ccsrtrsv(const kml_sparse_fill_mode_t uplo, const kml_sparse_operation_t opt,
    const kml_sparse_diag_type_t diag, const KML_INT m, const KML_Complex8 *a, const KML_INT *ia,
    const KML_INT *ja, const KML_Complex8 *x, KML_Complex8 *y);
kml_sparse_status_t kml_csparse_zcsrtrsv(const kml_sparse_fill_mode_t uplo, const kml_sparse_operation_t opt,
    const kml_sparse_diag_type_t diag, const KML_INT m, const KML_Complex16 *a, const KML_INT *ia,
    const KML_INT *ja, const KML_Complex16 *x, KML_Complex16 *y);
/*
 * 矩阵与矩阵运算C := op(A)*B，AB两个矩阵均为稀疏矩阵，C为稠密矩阵，仅支持one-based indexing
 * @opt, 转置情况
 * @m， 矩阵A行数
 * @n， 矩阵C列数
 * @k， 矩阵A列数
 * @a, values数组，存储矩阵A中非零元素的值
 * @ja, columns数组，用于表示矩阵A中非零元素所在的列索引。
 * @ia, 包含矩阵A的行索引，ia[i] - 1表示第i行第一个非零元素在val和indx数组内的下标。
 * @b，矩阵b的数组
 * @jb，columns数组，用于表示矩阵B中非零元素所在的列索引。
 * @ib, 包含矩阵B的行索引，ib[i] - 1表示第i行第一个非零元素在val和indx数组内的下标。
 * @c，矩阵c数组
 * @ldc，表示矩阵C的主维度大小
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_scsrmultd(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_INT k, const float *a, const KML_INT *ja, const KML_INT *ia,
    const float *b, const KML_INT *jb, const KML_INT *ib, float *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_dcsrmultd(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_INT k, const double *a, const KML_INT *ja, const KML_INT *ia,
    const double *b, const KML_INT *jb, const KML_INT *ib, double *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_ccsrmultd(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_INT k, const KML_Complex8 *a, const KML_INT *ja, const KML_INT *ia,
    const KML_Complex8 *b, const KML_INT *jb, const KML_INT *ib, KML_Complex8 *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_zcsrmultd(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_INT k, const KML_Complex16 *a, const KML_INT *ja, const KML_INT *ia,
    const KML_Complex16 *b, const KML_INT *jb, const KML_INT *ib, KML_Complex16 *c, const KML_INT ldc);

/*
 * 矩阵与矩阵运算C := op(A)*B，三个矩阵均为稀疏矩阵，仅支持one-based indexing
 * @opt, 转置情况
 * @request，表示ic、jc、c的内存申请情况
 * @sort，表示ABC的排序情况
 * @m， 矩阵A行数
 * @n， 矩阵A列数
 * @k， 矩阵B列数
 * @a, values数组，存储矩阵A中非零元素的值
 * @ja, columns数组，用于表示矩阵A中非零元素所在的列索引。
 * @ia, 包含矩阵A的行索引，ia[i] - 1表示第i行第一个非零元素在val和indx数组内的下标。
 * @b，矩阵b的数组
 * @jb，columns数组，用于表示矩阵B中非零元素所在的列索引。
 * @ib, 包含矩阵B的行索引，ib[i] - 1表示第i行第一个非零元素在val和indx数组内的下标。
 * @ic，在request=2时使用，表示矩阵C索引，ic[i] - 1表示第i行第一个非零元素在val和indx数组内的下标。
 * @nzmax，在request=0，2时使用，表示c所能申请的最大内存。
 * @c，矩阵c数组
 * @jc，表示矩阵C非零元素所在的列索引
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_scsrmultcsr(const kml_sparse_operation_t opt,
    const KML_INT request, const KML_INT sort, const KML_INT m, const KML_INT n, const KML_INT k,
    const float *a, const KML_INT *ja, const KML_INT *ia,
    const float *b, const KML_INT *jb, const KML_INT *ib,
    float *c, KML_INT *jc, KML_INT *ic, const KML_INT nzmax);
kml_sparse_status_t kml_sparse_dcsrmultcsr(const kml_sparse_operation_t opt, const KML_INT request,
    const KML_INT sort, const KML_INT m, const KML_INT n, const KML_INT k,
    const double *a, const KML_INT *ja, const KML_INT *ia,
    const double *b, const KML_INT *jb, const KML_INT *ib,
    double *c, KML_INT *jc, KML_INT *ic, const KML_INT nzmax);
kml_sparse_status_t kml_sparse_ccsrmultcsr(const kml_sparse_operation_t opt, const KML_INT request,
    const KML_INT sort, const KML_INT m, const KML_INT n, const KML_INT k,
    const KML_Complex8 *a, const KML_INT *ja, const KML_INT *ia,
    const KML_Complex8 *b, const KML_INT *jb, const KML_INT *ib,
    KML_Complex8 *c, KML_INT *jc, KML_INT *ic, const KML_INT nzmax);
kml_sparse_status_t kml_sparse_zcsrmultcsr(const kml_sparse_operation_t opt, const KML_INT request,
    const KML_INT sort, const KML_INT m, const KML_INT n, const KML_INT k,
    const KML_Complex16 *a, const KML_INT *ja, const KML_INT *ia,
    const KML_Complex16 *b, const KML_INT *jb, const KML_INT *ib,
    KML_Complex16 *c, KML_INT *jc, KML_INT *ic, const KML_INT nzmax);

kml_sparse_status_t kml_sparse_scscmm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_INT k, const float alpha, const char *matdescra, const float *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const float *b, const KML_INT ldb,
    const float beta, float *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_dcscmm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_INT k, const double alpha, const char *matdescra, const double *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const double *b, const KML_INT ldb,
    const double beta, double *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_ccscmm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_INT k, const KML_Complex8 alpha, const char *matdescra, const KML_Complex8 *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const KML_Complex8 *b, const KML_INT ldb,
    const KML_Complex8 beta, KML_Complex8 *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_zcscmm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_INT k, const KML_Complex16 alpha, const char *matdescra, const KML_Complex16 *val, const KML_INT *indx,
    const KML_INT *pntrb, const KML_INT *pntre, const KML_Complex16 *b, const KML_INT ldb,
    const KML_Complex16 beta, KML_Complex16 *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_scscsm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const float alpha, const char *matdescra,
    const float *val, const KML_INT *indx, const KML_INT *pntrb, const KML_INT *pntre,
    const float *b, const KML_INT ldb, float *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_dcscsm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const double alpha, const char *matdescra,
    const double *val, const KML_INT *indx, const KML_INT *pntrb, const KML_INT *pntre,
    const double *b, const KML_INT ldb, double *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_ccscsm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_Complex8 alpha, const char *matdescra,
    const KML_Complex8 *val, const KML_INT *indx, const KML_INT *pntrb, const KML_INT *pntre,
    const KML_Complex8 *b, const KML_INT ldb, KML_Complex8 *c, const KML_INT ldc);
kml_sparse_status_t kml_sparse_zcscsm(const kml_sparse_operation_t opt, const KML_INT m, const KML_INT n,
    const KML_Complex16 alpha, const char *matdescra,
    const KML_Complex16 *val, const KML_INT *indx, const KML_INT *pntrb, const KML_INT *pntre,
    const KML_Complex16 *b, const KML_INT ldb, KML_Complex16 *c, const KML_INT ldc);
/*
 * 计算两个CSR格式稀疏矩阵之和，C := A+beta*op(B)，仅支持one-based indexing
 * @opt, 转置情况
 * @request，表示ic、jc、c的内存申请情况
 * @sort，表示ABC的排序情况
 * @m， 矩阵A行数
 * @n， 矩阵A列数
 * @a, values数组，存储矩阵A中非零元素的值
 * @ja, columns数组，用于表示矩阵A中非零元素所在的列索引。
 * @ia, 包含矩阵A的行索引，ia[i] - 1表示第i行第一个非零元素在val和indx数组内的下标。
 * @beta，标量beta
 * @b，矩阵b的数组
 * @jb，矩阵b的列索引
 * @ib, 包含矩阵B的行索引，ib[i] - 1表示第i行第一个非零元素在val和indx数组内的下标。
 * @c，矩阵c数组
 * @jc，表示矩阵C非零元素所在的列索引
 * @ic，在request=2时使用，表示矩阵C索引，ic[i] - 1表示第i行第一个非零元素在val和indx数组内的下标。
 * @nzmax，在request=0，2时使用，表示c所能申请的最大内存。
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_dcsradd(const kml_sparse_operation_t opt, const KML_INT request, const KML_INT sort,
    const KML_INT m, const KML_INT n, const double *a, const KML_INT *ja, const KML_INT *ia,
    const double beta, const double *b, const KML_INT *jb, const KML_INT *ib, double *c, KML_INT *jc,
    KML_INT *ic, const KML_INT nzmax);
kml_sparse_status_t kml_sparse_scsradd(const kml_sparse_operation_t opt, const KML_INT request, const KML_INT sort,
    const KML_INT m, const KML_INT n, const float *a, const KML_INT *ja, const KML_INT *ia,
    const float beta, const float *b, const KML_INT *jb, const KML_INT *ib, float *c, KML_INT *jc,
    KML_INT *ic, const KML_INT nzmax);
kml_sparse_status_t kml_sparse_ccsradd(const kml_sparse_operation_t opt, const KML_INT request, const KML_INT sort,
    const KML_INT m, const KML_INT n, const KML_Complex8 *a, const KML_INT *ja, const KML_INT *ia,
    const KML_Complex8 beta, const KML_Complex8 *b, const KML_INT *jb, const KML_INT *ib, KML_Complex8 *c,
    KML_INT *jc, KML_INT *ic, const KML_INT nzmax);
kml_sparse_status_t kml_sparse_zcsradd(const kml_sparse_operation_t opt, const KML_INT request, const KML_INT sort,
    const KML_INT m, const KML_INT n, const KML_Complex16 *a, const KML_INT *ja, const KML_INT *ia,
    const KML_Complex16 beta, const KML_Complex16 *b, const KML_INT *jb, const KML_INT *ib, KML_Complex16 *c,
    KML_INT *jc, KML_INT *ic, const KML_INT nzmax);

/*
 * 设置线程数
 * thread_num的取值范围[1, 256]
 * 返回值
 * KML_SPARSE_STATUS_SUCCESS：操作成果
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 */
kml_sparse_status_t kml_set_thread_num(const int thread_num);

/*
 * 标量-向量乘，并加到另一个向量上：y = a*x + y
 * @nz, x及indx数组中元素的个数
 * @a，标量a
 * @x，非零元素数组x
 * @indx，数组x的下标
 * @y，数组y
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_saxpyi(const KML_INT nz, const float a,
    const float *x, const KML_INT *indx, float *y);
kml_sparse_status_t kml_sparse_daxpyi(const KML_INT nz, const double a,
    const double *x, const KML_INT *indx, double *y);
kml_sparse_status_t kml_sparse_caxpyi(const KML_INT nz, const KML_Complex8 a,
    const KML_Complex8 *x, const KML_INT *indx, KML_Complex8 *y);
kml_sparse_status_t kml_sparse_zaxpyi(const KML_INT nz, const KML_Complex16 a,
    const KML_Complex16 *x, const KML_INT *indx, KML_Complex16 *y);

/*
 * 对2个实数类型向量进行旋转操作，x[i] = c*x[i] + s*y[indx[i]]，y[indx[i]] = c*y[indx[i]]- s*x[i]
 * @nz, x及indx数组中元素的个数
 * @x，压缩格式向量x
 * @indx，存储到数组x中的非零元素的下标
 * @y，数组y
 * @c，标量
 * @s，标量
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_sroti(const KML_INT nz, float *x, const KML_INT *indx, float *y,
    const float c, const float s);
kml_sparse_status_t kml_sparse_droti(const KML_INT nz, double *x, const KML_INT *indx, double *y,
    const double c, const double s);

/*
 * 将full-storage格式稀疏向量加载到压缩格式向量中，x[i]=y[indx[i]]
 * @nz, x及indx数组中元素的个数
 * @indx，存储到数组x中的非零元素的下标
 * @y，数组y
 * @x，压缩格式向量x
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_sgthr(const KML_INT nz, const float *y,
    float *x, const KML_INT *indx);
kml_sparse_status_t kml_sparse_dgthr(const KML_INT nz, const double *y,
    double *x, const KML_INT *indx);
kml_sparse_status_t kml_sparse_cgthr(const KML_INT nz, const KML_Complex8 *y,
    KML_Complex8 *x, const KML_INT *indx);
kml_sparse_status_t kml_sparse_zgthr(const KML_INT nz, const KML_Complex16 *y,
    KML_Complex16 *x, const KML_INT *indx);

/*
 * 将full-storage格式稀疏向量加载到压缩格式向量中，x[i]=y[indx[i]]，并将y中对应元素清零
 * @y，数组y
 * @nz, x及indx数组中元素的个数
 * @x，压缩格式向量x
 * @indx，存储到数组x中的非零元素的下标
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_sgthrz(const KML_INT nz, float *y, float *x, const KML_INT *indx);
kml_sparse_status_t kml_sparse_dgthrz(const KML_INT nz, double *y, double *x, const KML_INT *indx);
kml_sparse_status_t kml_sparse_cgthrz(const KML_INT nz, KML_Complex8 *y, KML_Complex8 *x, const KML_INT *indx);
kml_sparse_status_t kml_sparse_zgthrz(const KML_INT nz, KML_Complex16 *y, KML_Complex16 *x, const KML_INT *indx);

/*
 * 将压缩格式稀疏向量加载到full-storage格式向量中，y[indx[i]]=x[i]
 * @nz, x及indx数组中元素的个数
 * @x，压缩格式向量x
 * @indx，数组x的下标
 * @y，存储full-storage格式向量的数组
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_ssctr(const KML_INT nz, const float *x, const KML_INT *indx, float *y);
kml_sparse_status_t kml_sparse_dsctr(const KML_INT nz, const double *x, const KML_INT *indx, double *y);
kml_sparse_status_t kml_sparse_csctr(const KML_INT nz, const KML_Complex8 *x, const KML_INT *indx, KML_Complex8 *y);
kml_sparse_status_t kml_sparse_zsctr(const KML_INT nz, const KML_Complex16 *x, const KML_INT *indx, KML_Complex16 *y);

/*
 * 实数域点积
 * @nz, x及indx数组中元素的个数
 * @x，压缩格式向量x
 * @indx，数组x的下标
 * @y，数组y
 * @doti，返回x与y的点积
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_sdoti(const KML_INT nz, const float* x, const KML_INT* indx,
    const float* y, float *doti);
kml_sparse_status_t kml_sparse_ddoti(const KML_INT nz, const double* x, const KML_INT* indx,
    const double* y, double *doti);

/*
 * 共轭点积，x·y = xˆH * y
 * @nz, x及indx数组中元素的个数
 * @x，压缩格式向量x
 * @indx，数组x的下标
 * @y，数组y
 * @doti，返回x与y的点积
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_cdotci_sub(const KML_INT nz, const KML_Complex8* x, const KML_INT* indx,
    const KML_Complex8* y, KML_Complex8 *dotci);
kml_sparse_status_t kml_sparse_zdotci_sub(const KML_INT nz, const KML_Complex16* x, const KML_INT* indx,
    const KML_Complex16* y, KML_Complex16 *dotci);

/*
 * 复数域非共轭点积
 * @nz, x及indx数组中元素的个数
 * @x，压缩格式向量x
 * @indx，存储数组x的非零元素下标索引
 * @y，数组y
 * @dotui，返回x与y的点积
 * 返回值：
 * KML_SPARSE_STATUS_SUCCESS：操作成功
 * KML_SPARSE_STATUS_NOT_INITIALIZED：矩阵句柄为空或者指向内存的指针为空
 * KML_SPARSE_STATUS_ALLOC_FAILED：内存申请失败
 * KML_SPARSE_STATUS_INVALID_VALUE：非法参数
 * KML_SPARSE_STATUS_EXECUTION_FAILED：执行失败
 * KML_SPARSE_STATUS_INTERNAL_ERROR：内部算法实现发生错误
 * KML_SPARSE_STATUS_NOT_SUPPORTED：当前参数对应的接口不支持
 */
kml_sparse_status_t kml_sparse_cdotui_sub(const KML_INT nz, const KML_Complex8* x, const KML_INT* indx,
    const KML_Complex8* y, KML_Complex8 *dotui);
kml_sparse_status_t kml_sparse_zdotui_sub(const KML_INT nz, const KML_Complex16* x, const KML_INT* indx,
    const KML_Complex16* y, KML_Complex16 *dotui);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
