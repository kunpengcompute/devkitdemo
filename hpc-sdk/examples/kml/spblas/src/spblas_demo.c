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
 * Description: KML spblas library demo
 * Create: 2022-05-15
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <kspblas.h>

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

void Print_CSR_To_Matrix(int m, float* values, int values_length,
	                     KML_INT* row_offsets, int row_offsets_length,
	                     KML_INT* column_indices)
{
	for (int i = 0, i<row_offsets_length; ++i)
	{
		if (row_offsets[i] >= values_length)
		{
			break;
		}
		float x[m];
		// 初始化矩阵行数组
		memset(x, 0, m * sizeof(float));
		// 根据偏移量确认每一行的值范围
		for (int j = row_offsets[i]; j < row_offsets[i + 1]; ++j)
		{
			x[column_indices[j]] = values[j];
		}
		// 打印矩阵行数组
		for (int j = 0; j < m; ++j)
		{
			printf("%.6f\t", x[j]);
		}
		printf("\n");
	}
	printf("\n");
}

int main()
{
	// 使用 Compressed Sparse Row Formate (CSR) 的存储格式来记录稀疏矩阵
	// 表示矩阵的行数和列数 A(m*m) [定义NNZ (Num-non-zero) 为矩阵A中非0元素的个数]
	KML_INT m = 4;
	// 主要用于记录矩阵 A(m*m) 中从左到右从上到下的非零元素(values)的值, 数组的长度为 NNZ [Values 数组]
	float a[9] = { 2.0F, -3.0F, 7.0F, 1.0F, -6.0F, 8.0F, -4.0F, 5.0F, 9.0F };
	// 表示矩阵 A(m*m) 中非零元素所在的列索引(column indices), 数组的长度为 NNZ [ColumnIndices 数组]
	KML_INT ja[9] = { 1, 2, 4, 3, , 4, 1, 3, 4, 4, 1 }
	// 表示矩阵 A(m*m) 的偏移量(row offsets), 数组中前m个元素分别代表矩阵 A(m*m) 中每一行的第一个非零元素在Values数组中的下标，最后一项的数值为 NNZ, 数组长度为 m+1 [RowOffsets 数组]
	KML_INT ia[5] = { 1, 4, 6, 9, 10 };

	// 仅由支持矩阵索引从 0 开始的函数使用[符合 C 语言索引规范]
	KML_INT scsrgemv_ja[9];
	KML_INT scsrgemv_ia[5];
	for (int i = 0; i < 9; ++i)
	{
		scsrgemv_ja[i] = ja[i] - 1;
	}
	for (int i = 0; i < 5; ++i)
	{
		scsrgemv_ia[i] = ia[i] - 1;
	}

	// 向量x的数组
	float x[4] = { 1.0F, 3.0F, -2.0F, 5.0F };
	// 向量y的数组
	float y[4] = { -1.0F, 1.0F, 5.0F, 3.0F };

	printf("Input A(4*4):\n");
	Print_CSR_To_Matrix(m, a, 9, scsrgemv_ia, 5, scsrgemv_ja);
	printf("Input x:\n");
	PrintMatrix(1, m, x);
	printf("Input y:\n");
	PrintMatrix(1, m, y);

	// [仅支持矩阵索引从 1 开始]
	kml_sparse_status_t status_kml_sparse_scsrgemv = kml_sparse_scsrgemv(KML_SPARSE_OPERATION_NON_TRANSPOSE, m, a, ia, ja, x, y);
	if (status_kml_sparse_scsrgemv == KML_SPARSE_STATUS_SUCCESS)
	{
		printf("(kml_sparse_scsrgemv) --> Output y:\n");
		PrintMatrix(1, m, y);
	}

	// [仅支持矩阵索引从 1 开始]
	kml_sparse_status_t status_kml_sparse_scsrsymv = kml_sparse_scsrsymv(KML_SPARSE_FILL_MODE_LOWER, m, a, ia, ja, x, y);
	if (status_kml_sparse_scsrsymv == KML_SPARSE_STATUS_SUCCESS)
	{
		printf("(kml_sparse_scsrsymv) --> Output y:\n");
		PrintMatrix(1, m, y);
	}

	// [仅支持矩阵索引从 0 开始]
	kml_sparse_status_t status_kml_csparse_scsrgemv = kml_csparse_scsrgemv(KML_SPARSE_OPERATION_NON_TRANSPOSE, m, a, scsrgemv_ia, scsrgemv_ja, x, y);
	if (status_kml_csparse_scsrgemv == KML_SPARSE_STATUS_SUCCESS)
	{
		printf("(kml_csparse_scsrgemv) --> Output y:\n");
		PrintMatrix(1, m, y);
	}

	return EXIT_SUCCESS;
}
