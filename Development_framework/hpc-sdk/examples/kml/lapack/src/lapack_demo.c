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
 * 		KML_LAPACK library functions demo demonstrates the use process of matrix factorization, 
 * 		matrix inversion, etc functions
 * Create: 2022-05-15
 */

#include <stdio.h>
#include <stdlib.h>

#include <klapack.h>
#include <mpi.h>

void PrintArray(const double* array, const int row, const int column)
{
	int i;
	for (i = 0; i < row; i++)
	{
		int j;
		for (j = 0; j < column; j++)
		{
			printf("%-12.6f", array[j * column + i]);
		}
		printf("\n");
	}
	printf("\n");
}

int TestDgetrf(double* a, int* ipiv, const int m, const int n, const int lda)
{
	int info = 0;
	/*
	 * Compute the LU factorization of matrix A. This function uses partial pivoting, allowing row interchanges. 
	 * The operation is defined as A = PLU, where P is a permutation matrix, L is a lower triangular matrix or 
	 * a lower echelon matrix and the diagonal is 1, and U is an upper triangular matrix or an upper echelon matrix.
	 */
	dgetrf_(&m, &n, a, &lda, ipiv, &info);
	if (info != 0)
	{
		if (info > 0)
		{
			printf("Error: The value of the -%d-th parameter is invalid.", info);
		}
		else
		{
			printf("Error: The %d-th element on the diagonal of matrix U is 0. "
			       "The matrix factorization is complete, but U is singular. As a result, "
			       "an error of dividing by zero occurs when a system of linear equations is solved.", info);
		}
	}
	return info;
}

int TestDgetri(double* a, const int* ipiv, const int n, const int lda)
{
	/*
	 * Compute the inverse matrix based on the LU factorization result obtained using ?getrf. 
	 */
	int info = 0;
	// Temporary storage space. After lwork=-1 is called, work[0] is the optimal lwork value.
	double* work = NULL;
	double qwork;
	// Length of the work array
	int lwork = -1;
	/* Query optimal work size */
	dgetri_(&n, a, &lda, ipiv, &qwork, &lwork, &info);
	if (info != 0)
	{
		if (info > 0)
		{
			printf("Error: The value of the -%d-th parameter is invalid.", info);
		}
		else
		{
			printf("Error: The %d-th element on the diagonal of U is 0, and the matrix cannot be inverted.", info);
		}
		return info;
	}
	lwork = (int)qwork;
	work = (double*)malloc(sizeof(double) * lwork);
	/* Calculate inversion */
	dgetri_(&n, a, &lda, ipiv, work, &lwork, &info);
	free(work);
	if (info != 0)
	{
		if (info > 0)
		{
			printf("Error: The value of the -%d-th parameter is invalid.", info);
		}
		else
		{
			printf("Error: The %d-th element on the diagonal of U is 0, and the matrix cannot be inverted.", info);
		}
		return info;
	}
	return info;
}

int main()
{
	MPI_Init(NULL, NULL);
	// Number of rows in matrix A
	int m = 4;
	// Number of columns in matrix A
	int n = 4;
	// Leading dimension of the matrix A. lda >= max(1, n).
	int lda = 4;
	// An array containing pivot indices obtained from ?getrf. Its length is min(m, n). 
	// For 1 <= ipiv <= min(m, n), row i and row ipiv[i-1] of the matrix are interchanged during factorization.
	int ipiv[4];

	/*
	 * A (stored in column-major):
	 *  1.80   2.88   2.05  -0.89
	 *  5.25  -2.95  -0.95  -3.08
	 *  1.58  -2.69  -2.90  -1.04
	 * -1.11  -0.66  -0.59   0.80
	 */
	double a[16] = { 1.80, 5.25, 1.58, -1.11,
	                 2.88, -2.95, -2.69, -0.66,
	                 2.05, -0.95, -2.90, -0.59,
	                 -0.89, -3.80, -1.04, 0.80 };
	printf("Input matrix A(4*4):\n");
	PrintArray(a, m, n);

	if (TestDgetrf(a, ipiv, m, n, lda) != 0)
	{
		return EXIT_FAILURE;
	}

	printf("dgetrf --> Matrix A:\n");
	PrintArray(a, m, n);

	if (TestDgetri(a, ipiv, n, lda) == 0)
	{
		printf("dgetri --> Matrix A:\n");
		PrintArray(a, m, n);
	}
	MPI_Finalize();
	return EXIT_SUCCESS;
}
