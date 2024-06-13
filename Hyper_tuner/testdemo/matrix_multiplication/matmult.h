/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 */

#ifndef MATMULT_H
#define MATMULT_H
#include "matrix.h"

int base_matmult(const real_t* A, const real_t* B, real_t* C, size_t n);

int parallel_matmult(const real_t* A, const real_t* B, real_t* C, size_t n);

int change_loop_order_matmult(const real_t* A, const real_t* B, real_t* C, size_t n);

int transpose_B_matmult(const real_t* A, const real_t* B, real_t* C, size_t n);

int block_matmult(const real_t* A, const real_t* B, real_t* C, size_t n);

int block_transpose_B_matmult(const real_t* A, const real_t* B, real_t* C, size_t n);

int intrinsics_transpose_B_matmult(const real_t* A, const real_t* B, real_t* C, size_t n);

#ifndef BLOCK_SIZE_I

#define BLOCK_SIZE_I 8
#define BLOCK_SIZE_K 8
#define BLOCK_SIZE_J 8

#endif

#endif // MATMULT_H
