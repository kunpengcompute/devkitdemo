/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 */

#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "matmult.h"

#include "roofline_events.h"

int block_matmult(const real_t* A, const real_t* B, real_t* C, size_t n)
{
    if (A == NULL || B == NULL || C == NULL) {
        return EXIT_FAILURE;
    }

    memset(C, 0, n * n * sizeof(real_t));

    #pragma omp parallel
    {
        size_t i = 0;
        size_t j = 0;
        size_t k = 0;
        size_t i_bl = 0;
        size_t j_bl = 0;
        size_t k_bl = 0;
        #pragma omp for
        for (i_bl = 0; i_bl < n / BLOCK_SIZE_I; i_bl++) {
            for (j_bl = 0; j_bl < n / BLOCK_SIZE_J; j_bl++) {
                for (k_bl = 0; k_bl < n / BLOCK_SIZE_K; k_bl++) {
                    for (i = 0; i < BLOCK_SIZE_I; i++) {
                        for (j = 0; j < BLOCK_SIZE_J; j++) {
                            real_t accum = 0;
                            size_t base_ind_i = i_bl * BLOCK_SIZE_I + i;
                            size_t base_ind_j = j_bl * BLOCK_SIZE_J + j;
                            for (k = 0; k < BLOCK_SIZE_K; k++) {
                                size_t base_ind_k = k_bl * BLOCK_SIZE_K + k;
                                accum += A[base_ind_i * n + base_ind_k] *
                                         B[base_ind_k * n + base_ind_j];
                            }
                            C[base_ind_i * n + base_ind_j] += accum;
                        }
                    }
                }
            }
        }
    }

    return 0;
}

int block_transpose_B_matmult(const real_t* A, const real_t* B, real_t* C, size_t n)
{
    if (A == NULL || B == NULL || C == NULL) {
        return EXIT_FAILURE;
    }

    real_t* transpose_B = alloc_matrix(n);

    #pragma omp parallel
    {
        ROOFLINE_EVENTS_START_REGION(__func__);
        size_t i = 0;
        size_t j = 0;
        size_t k = 0;
        size_t i_bl = 0;
        size_t j_bl = 0;
        size_t k_bl = 0;
        #pragma omp for
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
               transpose_B[i * n + j] = B[j * n + i];
               C[i * n + j] = 0;
            }
        }
        #pragma omp for
        for (i_bl = 0; i_bl < n / BLOCK_SIZE_I; i_bl++) {
            for (j_bl = 0; j_bl < n / BLOCK_SIZE_J; j_bl++) {
                for (k_bl = 0; k_bl < n / BLOCK_SIZE_K; k_bl++) {
                    for (i = 0; i < BLOCK_SIZE_I; i++) {
                        for (j = 0; j < BLOCK_SIZE_J; j++) {
                            size_t base_ind_i = i_bl * BLOCK_SIZE_I + i;
                            size_t base_ind_j = j_bl * BLOCK_SIZE_J + j;

                            real_t accum = 0;
                            for (k = 0; k < BLOCK_SIZE_K; k++) {
                                size_t base_ind_k = k_bl * BLOCK_SIZE_K + k;
                                accum += A[base_ind_i * n + base_ind_k] * transpose_B[base_ind_j * n + base_ind_k];
                            }
                            C[base_ind_i * n + base_ind_j] += accum;
                        }
                    }
                }
            }
        }
        ROOFLINE_EVENTS_STOP_REGION(__func__);
    }
    dealloc_matrix(transpose_B);

    return 0;
}
