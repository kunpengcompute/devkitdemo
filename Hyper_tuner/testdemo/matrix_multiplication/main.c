/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 */

#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "matmult.h"

#ifdef ENABLE_KML
#include "kblas.h"
#endif // ENABLE_KML

#include "roofline_events.h"


int main(int argc, char** argv)
{
    size_t n = 1024;
    size_t i, j;
    real_t *A, *B, *C_base, *C;
    double start_time, end_time;
    double start_time1, end_time1;
    int mult_case = 5;
    int enable_correctness = 0;

    if (argc >= 2) {
        n = strtoul(argv[1], NULL, 0);
        if (n > (1ull << 32)) {
            printf("To big size of matrix\n");
            return EXIT_FAILURE;
        }
        if (n == 0) {
            n = 1024;
            printf("n cannot be 0, set n to %lu\n", n);
        } else {
            if ((n & (n - 1)) != 0) {
                n = (1ull << (64 - __builtin_clzl(n) - 1));
                printf("n is not a power of two, set n to %lu\n", n);
            }
        }
    }

    if (argc >= 3) {
        mult_case = strtoul(argv[2], NULL, 0);
    }

    if (argc >= 4) {
        if (strcmp(argv[3], "true") == 0) {
            enable_correctness = 1;
        }
    }

    printf("Size is %lu, Matrix multiplication method is: %d, Check correctness is: %d\n", n, mult_case, enable_correctness);

    start_time = omp_get_wtime();
    int error = 0;
    A = alloc_matrix(n);
    B = alloc_matrix(n);
    C = alloc_matrix(n);
    C_base = alloc_matrix(n);

    if (A == NULL || B == NULL || C == NULL || C_base == NULL) {
        printf("Failed to allocate matrix with size = %lu\n", n);
        return EXIT_FAILURE;
    }

    fill_matrix_rand(A, n);
    fill_matrix_rand(B, n);
    end_time = omp_get_wtime();
    printf("Initialization time = %lfs\n", end_time - start_time);

    char* num_threads;
    start_time = omp_get_wtime();
    ROOFLINE_EVENTS_INIT;
    switch (mult_case) {
        case 0:
            error |= base_matmult(A, B, C, n);
            break;
        case 1:
            error |= parallel_matmult(A, B, C, n);
            break;
        case 2:
            error |= transpose_B_matmult(A, B, C, n);
            break;
        case 3:
            error |= change_loop_order_matmult(A, B, C, n);
            break;
        case 4:
            error |= block_transpose_B_matmult(A, B, C, n);
            break;
        case 5:
            error |= intrinsics_transpose_B_matmult(A, B, C, n);
            break;

#ifdef ENABLE_KML
        case 6:
            #pragma omp parallel
            {
                ROOFLINE_EVENTS_START_REGION("kml_matmult");
                #pragma omp for
                for (i = 0; i < n; i++) {
                    for (j = 0; j < n; j++) {
                        C[i * n + j] = 0;
                    }
                }
            }
#ifdef DOUBLE_TYPE
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A, n, B, n, 0.0, C, n);
#else
            cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A, n, B, n, 0.0, C, n);
#endif
            #pragma omp parallel
            {
                ROOFLINE_EVENTS_STOP_REGION("kml_matmult");
            }
            break;
#endif // ENABLE_KML
        case 9:
            start_time1 = omp_get_wtime();
            error |= parallel_matmult(A, B, C, n);
            end_time1 = omp_get_wtime();
            printf("Matrix multiplication time(parallel_matmult) = %lfs\n", end_time1 - start_time1);

            start_time1 = omp_get_wtime();
            error |= transpose_B_matmult(A, B, C, n);
            end_time1 = omp_get_wtime();
            printf("Matrix multiplication time(transpose_B_matmult) = %lfs\n", end_time1 - start_time1);

            start_time1 = omp_get_wtime();
            error |= block_transpose_B_matmult(A, B, C, n);
            end_time1 = omp_get_wtime();
            printf("Matrix multiplication time(block_transpose_B_matmult) = %lfs\n", end_time1 - start_time1);

            start_time1 = omp_get_wtime();
            error |= intrinsics_transpose_B_matmult(A, B, C, n);
            end_time1 = omp_get_wtime();
            printf("Matrix multiplication time(intrinsics_transpose_B_matmult) = %lfs\n", end_time1 - start_time1);

#ifdef ENABLE_KML
            start_time1 = omp_get_wtime();
            #pragma omp parallel
            {
               ROOFLINE_EVENTS_START_REGION("kml_matmult");
               #pragma omp for
               for (i = 0; i < n; i++) {
                   for (j = 0; j < n; j++) {
                       C[i * n + j] = 0;
                   }
               }
            }
#ifdef DOUBLE_TYPE
            cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A, n, B, n, 0.0, C, n);
#else
            cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A, n, B, n, 0.0, C, n);
#endif
            #pragma omp parallel
            {
               ROOFLINE_EVENTS_STOP_REGION("kml_matmult");
            }
            end_time1 = omp_get_wtime();
            printf("Matrix multiplication time(kml_matmult) = %lfs\n", end_time1 - start_time1);
#endif // ENABLE_KML
            break;
        default:
            fprintf(stderr, "Wrong optimized number\n");
            return EXIT_FAILURE;
    }
    ROOFLINE_EVENTS_FINALIZE;
    end_time = omp_get_wtime();

    if (error != 0) {
        printf("Failed to multiply matrix\n");
        return EXIT_FAILURE;
    }

    printf("Matrix multiplication time = %lfs\n", end_time - start_time);

    int exit_code = EXIT_SUCCESS;
    if (enable_correctness) {
        start_time = omp_get_wtime();
#ifdef ENABLE_KML
#ifdef DOUBLE_TYPE
        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A, n, B, n, 0.0, C_base, n);
#else
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, A, n, B, n, 0.0, C_base, n);
#endif
#else
        intrinsics_transpose_B_matmult(A, B, C_base, n);
#endif // ENABLE_KML
        end_time = omp_get_wtime();
        printf("Parallel matrix multiplication time = %lfs\n", end_time - start_time);

        if (check_matrix_eq(C, C_base, n) == 0) {
            printf("Correctness test: Failed\n");
            exit_code = EXIT_FAILURE;
        } else {
            printf("Correctness test: Successful\n");
        }

    }
    if (n <= 16) {
        print_matrix(A, n);
        printf("------------------------\n");
        print_matrix(B, n);
        printf("------------------------\n");
        print_matrix(C, n);
        printf("------------------------\n");
        print_matrix(C_base, n);
    }

    dealloc_matrix(A);
    dealloc_matrix(B);
    dealloc_matrix(C);
    dealloc_matrix(C_base);

    return exit_code;
}
