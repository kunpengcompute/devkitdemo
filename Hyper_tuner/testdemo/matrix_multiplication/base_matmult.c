/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 */

#include <assert.h>
#include <stdlib.h>
#include <omp.h>
#include "matmult.h"

#include "roofline_events.h"


int base_matmult(const real_t* A, const real_t* B, real_t* C, size_t n)
{
    if (A == NULL || B == NULL || C == NULL) {
        return EXIT_FAILURE;
    }

    ROOFLINE_EVENTS_START_REGION(__func__);
    size_t i;
    size_t j;
    size_t k;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            real_t accum = 0;
            for (k = 0; k < n; k++) {
                accum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = accum;
        }
    }
    ROOFLINE_EVENTS_STOP_REGION(__func__);
    return 0;
}

int parallel_matmult(const real_t* A, const real_t* B, real_t* C, size_t n)
{
    if (A == NULL || B == NULL || C == NULL) {
        return EXIT_FAILURE;
    }
    #pragma omp parallel
    {
        ROOFLINE_EVENTS_START_REGION(__func__);
        size_t i;
        size_t j;
        size_t k;
        #pragma omp for
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
               C[i * n + j] = 0;
            }
        }
        #pragma omp for
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                for (k = 0; k < n; k++) {
                    C[i * n + j] += A[i * n + k] * B[k * n + j];
                }
            }
        }
        ROOFLINE_EVENTS_STOP_REGION(__func__);
    }
    return 0;
}

int change_loop_order_matmult(const real_t* A, const real_t* B, real_t* C, size_t n)
{
    if (A == NULL || B == NULL || C == NULL) {
        return EXIT_FAILURE;
    }
    #pragma omp parallel
    {
        ROOFLINE_EVENTS_START_REGION(__func__);
        size_t i;
        size_t j;
        size_t k;
        #pragma omp for
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
               C[i * n + j] = 0;
            }
        }
        #pragma omp for
        for (i = 0; i < n; i++) {
            for (k = 0; k < n; k++) {
                // real_t accum = 0;
                for (j = 0; j < n; j++) {
                    // accum += ;
                    C[i * n + j] += A[i * n + k] * B[k * n + j];
                }
            }
        }
        ROOFLINE_EVENTS_STOP_REGION(__func__);
    }
    return 0;
}

int transpose_B_matmult(const real_t* A, const real_t* B, real_t* C, size_t n)
{
    if (A == NULL || B == NULL || C == NULL) {
        return EXIT_FAILURE;
    }
    real_t* transpose_B = alloc_matrix(n);
    #pragma omp parallel
    {
        ROOFLINE_EVENTS_START_REGION(__func__);
        size_t i;
        size_t j;
        size_t k;
        #pragma omp for
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
               transpose_B[j * n + i] = B[i * n + j];
               C[i * n + j] = 0;
            }
        }
        #pragma omp for
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                for (k = 0; k < n; k++) {
                    C[i * n + j] += A[i * n + k] * transpose_B[j * n + k];
                }
            }
        }
        ROOFLINE_EVENTS_STOP_REGION(__func__);
    }
    dealloc_matrix(transpose_B);

    return 0;
}
 