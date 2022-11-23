/*
 * Copyright 2022 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http: //www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description:
 * 		KML_BLAS library functions demo demonstrates the use process of matrix multiplication, 
 * 		symmetric matrix multiplication, update the rank of sysmmetric matrix, etc functions.
 * Create: 2022-05-15
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <kblas.h>
#include <mpi.h>

void PrintMatrix(int row, int column, const float* matrix)
{
	int i;
	for (i = 0; i < row; i++)
	{
		int j;
		for (j = 0; j < column; j++)
		{
			printf("%-12.6f", matrix[j * column + i]);
		}
		printf("\n");
	}
	printf("\n");
}

int main()
{
	MPI_Init(NULL, NULL);
	// Rows of matrices of op(A) and C
	int m = 3;
	// Columns of matrices of op(B) and C
	int n = 3;
	// Columns of the matrix op(A) and rows of the matrix op(B)
	int k = 3;
	// If the matrix is column store and TransA = CblasNoTrans, lda is at least max(1, m); otherwise, lda is at least max(1, k).
	// If A is row-store matrix and TransA = CblasNoTrans, lda is at least max(1, k); otherwise, lda is at least max(1, m).
	int lda = 3, ldb = 3, ldc = 3;
	// Multiplication coefficient
	float alpha = 1.0F, beta = 1.0F;

	float a[9] = { 1.0F, 0, 0,
	               0, 1.0F, 0,
	               0, 0, 1.0F };
	printf("Input matrix A(3*3):\n");
	PrintMatrix(3, 3, a);

	float b[9] = { 0, 0, 2.0F,
	               0, 2.0F, 0,
	               2.0F, 0, 0 };
	printf("Input matrix B(3*3):\n");
	PrintMatrix(3, 3, b);

	float c[9] = { 3.0F, 0, 4.0F,
	               0, 0, 0,
	               4.0F, 0, 3.0F };
	printf("Input matrix C(3*3):\n");
	PrintMatrix(3, 3, c);

	float d[9];
	memcpy(d, c, sizeof(d));

	/*
	 * Multiply one matrix by another.
	 * C = alpha * op(A) * op(B) + beta * C
	 * The value of op(X) may be op(X) = X, op(X) = X', op(X) = conjg(X')
	 * alpha and beta are multiplication coefficient.
	 * op(A) is an m*k matrix, op(B) is a k*n matrix, C is an m*n matrix.
	 */
	cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
	printf("cblas_sgemm --> Output matrix C(3*3):\n");
	PrintMatrix(m, n, c);

	/*
	 * Compute the product of matrices in which matrix A is symmetric.
	 * C = alpha * A * B + beta * C  or  C = alpha * B * A + beta * C
	 * alpha and beta are multiplication coefficient.
	 * A is a symmetric matrix, B and C are m*n general matrices.
	 */
	cblas_ssymm(CblasColMajor, CblasLeft, CblasLower, m, n, alpha, a, lda, b, ldb, beta, c, ldc);
	printf("cblas_ssymm --> Output matrix C(3*3):\n");
	PrintMatrix(m, n, c);

	/*
	 * Perform a rank-k matrix-matrix operation for a symmetric matrix.
	 * C = alpha * A * A' + beta * C  or  C = alpha * A' * A + beta * C
	 * alpha and beta are multiplication coefficient.
	 * C is an n*n symmetric matrix.
	 * In the first case, A is an n*k matrix.
	 * In the second case, A is a k*n general matrix.
	 */
	cblas_ssyrk(CblasColMajor, CblasUpper, CblasNoTrans, n, k, alpha, c, lda, beta, d, ldc);
	printf("cblas_ssyrk --> Output matrix C(3*3):\n");
	PrintMatrix(m, n, c);
	MPI_Finalize();
	return EXIT_SUCCESS;
}
