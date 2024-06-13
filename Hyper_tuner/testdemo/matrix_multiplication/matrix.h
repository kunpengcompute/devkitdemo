/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

#ifdef DOUBLE_TYPE
typedef double real_t;
#else
typedef float real_t;
#endif

real_t* alloc_matrix(size_t n);

void dealloc_matrix(real_t* A);

void fill_matrix_rand(real_t* A, size_t n);

void print_matrix(real_t* A, size_t n);

int check_matrix_eq(real_t* A, real_t* B, size_t n);

#endif
