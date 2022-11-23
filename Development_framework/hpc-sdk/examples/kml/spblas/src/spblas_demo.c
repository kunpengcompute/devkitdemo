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
 * 		KML_SPBLAS library functions demo demonstrates the use process of sparse matrix and vector calculation function.
 * Create: 2022-05-15
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <kspblas.h>
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

void Print_CSR_To_Matrix(int m, float* values, int values_length,
	KML_INT* row_offsets, int row_offsets_length,
	KML_INT* column_indices)
{
	int i;
	for (i = 0; i < row_offsets_length; ++i)
	{
		if (row_offsets[i] >= values_length)
		{
			break;
		}
		float x[m];
		// Initialize the matrix row array
		memset(x, 0, m * sizeof(float));

		// Confirm the range of values for each row based on the offset
		int j;
		for (j = row_offsets[i]; j < row_offsets[i + 1]; ++j)
		{
			x[column_indices[j]] = values[j];
		}

		// print matrix row array
		int y;
		for (y = 0; y < m; ++y)
		{
			printf("%.6f\t", x[y]);
		}

		printf("\n");
	}
	printf("\n");
}

int main()
{
	MPI_Init(NULL, NULL);
	// Record sparse matrices using the 'Compressed Sparse Row Formate (CSR)' storage format
	// Represents the number of rows and columns of the matrix A(m*m) [Define NNZ (Num-non-zero) as the number of non-zero elements in matrix A]
	KML_INT m = 4;
	// Mainly used to record the values of non-zero elements (values) from left to right and top to bottom in the matrix A(m*m), the length of the array is NNZ [Values array]
	float a[9] = { 2.0F, -3.0F, 7.0F, 1.0F, -6.0F, 8.0F, -4.0F, 5.0F, 9.0F };
	// Indicates the column indices of the non-zero elements in the matrix A(m*m), the length of the array is NNZ [ColumnIndices array]
	KML_INT ja[9] = { 1, 2, 4, 3, 4, 1, 3, 4, 1 };
	// Represents the offset (row offsets) of the matrix A(m*m), the first m elements in the array represent the subscripts of the first non-zero element of each row in the matrix A(m*m) in the Values array, The value of the last item is NNZ, the length of the array is m+1 [RowOffsets array]
	KML_INT ia[5] = { 1, 4, 6, 9, 10 };

	// Only used by functions that support 0-based matrix indexing [conforms to C indexing specification]
	KML_INT scsrgemv_ja[9];
	KML_INT scsrgemv_ia[5];

	int ix;
	for (ix = 0; ix < 9; ++ix)
	{
		scsrgemv_ja[ix] = ja[ix] - 1;
	}

	int iy;
	for (iy = 0; iy < 5; ++iy)
	{
		scsrgemv_ia[iy] = ia[iy] - 1;
	}

	// array of vectors x
	float x[4] = { 1.0F, 3.0F, -2.0F, 5.0F };
	// array of vectors y
	float y[4] = { -1.0F, 1.0F, 5.0F, 3.0F };

	printf("Input A(4*4):\n");
	Print_CSR_To_Matrix(m, a, 9, scsrgemv_ia, 5, scsrgemv_ja);
	printf("Input x:\n");
	PrintMatrix(1, m, x);
	printf("Input y:\n");
	PrintMatrix(1, m, y);

	// [only supports matrix indices starting from 1]
	kml_sparse_status_t status_kml_sparse_scsrgemv = kml_sparse_scsrgemv(KML_SPARSE_OPERATION_NON_TRANSPOSE, m, a, ia, ja, x, y);
	if (status_kml_sparse_scsrgemv == KML_SPARSE_STATUS_SUCCESS)
	{
		printf("(kml_sparse_scsrgemv) --> Output y:\n");
		PrintMatrix(1, m, y);
	}

	// [only supports matrix indices starting from 1]
	kml_sparse_status_t status_kml_sparse_scsrsymv = kml_sparse_scsrsymv(KML_SPARSE_FILL_MODE_LOWER, m, a, ia, ja, x, y);
	if (status_kml_sparse_scsrsymv == KML_SPARSE_STATUS_SUCCESS)
	{
		printf("(kml_sparse_scsrsymv) --> Output y:\n");
		PrintMatrix(1, m, y);
	}

	// [only supports matrix indices starting from 0]
	kml_sparse_status_t status_kml_csparse_scsrgemv = kml_csparse_scsrgemv(KML_SPARSE_OPERATION_NON_TRANSPOSE, m, a, scsrgemv_ia, scsrgemv_ja, x, y);
	if (status_kml_csparse_scsrgemv == KML_SPARSE_STATUS_SUCCESS)
	{
		printf("(kml_csparse_scsrgemv) --> Output y:\n");
		PrintMatrix(1, m, y);
	}
	MPI_Finalize();
	return EXIT_SUCCESS;
}
