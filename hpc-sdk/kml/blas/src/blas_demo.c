// =========================================================================
// Copyright 2022 Huawei Technologies Co., Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ==========================================================================

//
// Description: KML blas library demo
// Create: 2022-05-15
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <kblas.h>

void PrintMatrix(int row, int column, const float* matrix)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			printf("%-12.6f", matrix[j * column + i]);
		}
		printf("\n");
	}
	printf("\n");
}

int main()
{
	int m = 3, n = 3, k = 3, lda = 3, ldb = 3, ldc = 3;
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

	cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, a, lda, b, ldb, beta, c, ldc);
	printf("cblas_sgemm --> Output matrix C(3*3):\n");
	PrintMatrix(m, n, c);

	cblas_ssymm(CblasColMajor, CblasLeft, CblasLower, m, n, alpha, a, lda, b, ldb, beta, c, ldc);
	printf("cblas_ssymm --> Output matrix C(3*3):\n");
	PrintMatrix(m, n, c);

	cblas_ssyrk(CblasColMajor, CblasUpper, CblasNoTrans, n, k, alpha, c, lda, beta, d, ldc);
	printf("cblas_ssyrk --> Output matrix C(3*3):\n");
	PrintMatrix(m, n, c);

	return EXIT_SUCCESS;
}
