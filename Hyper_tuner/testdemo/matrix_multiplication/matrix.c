/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 */

#include "matrix.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

real_t* alloc_matrix(size_t n)
{
    const size_t CACHE_LINE_SIZE = 64;
    return aligned_alloc(CACHE_LINE_SIZE, n * n * sizeof(real_t));
}

void dealloc_matrix(real_t* A)
{
    free(A);
}

static real_t rand_real()
{
#ifdef DOUBLE_TYPE
    const int mod = 1024;
    const double divider = 16.0;
#else
    const int mod = 256;
    const float divider = 16.0f;
#endif
    return ((rand() % mod) - mod / 2) / divider;
}

void fill_matrix_rand(real_t* A, size_t n)
{
    size_t i;
    size_t j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            A[i * n + j] = rand_real();
        }
    }
}

void print_matrix(real_t* A, size_t n)
{
    size_t i;
    size_t j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%f\t", A[i * n + j]);
        }
        printf("\n");
    }
}

int check_matrix_eq(real_t* A, real_t* B, size_t n)
{
#ifdef DOUBLE_TYPE
    const double eps = DBL_EPSILON;
    size_t i;
    for (i = 0; i < n * n; i++) {
        if (fabs(A[i] - B[i]) >= eps) {
            printf("Error: |A[%lu]=%15lf - B[%lu]=%15lf| = %15lf\n", i, A[i], i, B[i], fabs(A[i] - B[i]));
            return 0;
        }
    }
#else
    const float eps = FLT_EPSILON;
    size_t i;
    for (i = 0; i < n * n; i++) {
        if (fabsf(A[i] - B[i]) >= eps) {
            printf("Error: |A[%lu]=%8f - B[%lu]=%8f| = %8f\n", i, A[i], i, B[i], fabsf(A[i] - B[i]));
            return 0;
        }
    }
#endif

    return 1;
}
