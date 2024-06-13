/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 */

#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matmult.h"
#include "arm_neon.h"

#include "roofline_events.h"

#define CACHE_LINE_SIZE 64

#ifdef DOUBLE_TYPE

#define reg_t float64x2_t
#define set_reg_to(val) vmovq_n_f64(val)
#define load_reg(addr) vld1q_f64(addr)
#define fma(accum, a, b) vmlaq_f64(accum, a, b)
#define reduce_add(reg) vaddvq_f64(reg)

#define VECTOR_LENGTH 2

#else

#define reg_t float32x4_t
#define set_reg_to(val) vmovq_n_f32(val)
#define load_reg(addr) vld1q_f32(addr)
#define fma(accum, a, b) vmlaq_f32(accum, a, b)
#define reduce_add(reg) vaddvq_f32(reg)

#define VECTOR_LENGTH 4

#endif

int intrinsics_transpose_B_matmult(const real_t* A, const real_t* B, real_t* C, size_t n)
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
        size_t i_bl;
        size_t j_bl;
        size_t k_bl;
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

                            for (k = 0; k < BLOCK_SIZE_K; k += CACHE_LINE_SIZE / sizeof(real_t)) {
                                size_t base_ind_k = k_bl * BLOCK_SIZE_K + k;
                            }

                            reg_t a, b, accum;
                            accum = set_reg_to(0.0);
                            for (k = 0; k < BLOCK_SIZE_K; k += VECTOR_LENGTH) {
                                size_t base_ind_k = k_bl * BLOCK_SIZE_K + k;
                                a = load_reg(&A[base_ind_i * n + base_ind_k]);
                                b = load_reg(&transpose_B[base_ind_j * n + base_ind_k]);
                                accum = fma(accum, a, b);
                            }
                            C[base_ind_i * n + base_ind_j] += reduce_add(accum);
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


//int intrinsics_transpose_B_matmult(const real_t* A, const real_t* B, real_t* C, size_t n)
//{
//    if (A == NULL || B == NULL || C == NULL) {
//        return EXIT_FAILURE;
//    }
//
//    real_t* transpose_B = alloc_matrix(n);
//    #pragma omp parallel
//    {
//        size_t i;
//        size_t j;
//        size_t k;
//        size_t i_bl;
//        size_t j_bl;
//        size_t k_bl;
//        #pragma omp for
//        for (i = 0; i < n; i++) {
//            for (size_t j = 0; j < n; j++) {
//                transpose_B[i * n + j] = B[j * n + i];
//                C[i * n + j] = 0;
//            }
//        }
//
//        #pragma omp for
//        for (i_bl = 0; i_bl < n / BLOCK_SIZE_I; i_bl++) {
//            for (j_bl = 0; j_bl < n / BLOCK_SIZE_J; j_bl++) {
//                for (k_bl = 0; k_bl < n / BLOCK_SIZE_K; k_bl++) {
//                    for (i = 0; i < BLOCK_SIZE_I; i++) {
//                        for (j = 0; j < BLOCK_SIZE_J; j++) {
//                            size_t base_ind_i = i_bl * BLOCK_SIZE_I + i;
//                            size_t base_ind_j = j_bl * BLOCK_SIZE_J + j;
//
//                            for (k = 0; k < BLOCK_SIZE_K; k += CACHE_LINE_SIZE / sizeof(real_t)) {
//                                size_t base_ind_k = k_bl * BLOCK_SIZE_K + k;
//                            }
//
//                            reg_t a, b, accum;
//                            accum = set_reg_to(0.0);
//                            for (k = 0; k < BLOCK_SIZE_K; k += VECTOR_LENGTH) {
//                                size_t base_ind_k = k_bl * BLOCK_SIZE_K + k;
//                                a = load_reg(&A[base_ind_i * n + base_ind_k]);
//                                b = load_reg(&transpose_B[base_ind_j * n + base_ind_k]);
//                                accum = fma(accum, a, b);
//                            }
//                            C[base_ind_i * n + base_ind_j] += reduce_add(accum);
//                        }
//                    }
//                }
//            }
//        }
//    }
//    dealloc_matrix(transpose_B);
//    return 0;
//}