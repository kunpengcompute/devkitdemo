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
 * 		KML_FFT library functions demo demonstrates the use process of C2C transforms, etc functions.
 * Create: 2022-05-15
 */

#include <stdio.h>
#include <stdlib.h>

#include <kfft.h>
#include <mpi.h>

#ifdef SINGLE
#define FFT_COMPLEX kml_fftf_complex
#define FFT_PLAN kml_fftf_plan
#define FFT_MALLOC kml_fftf_malloc
#define FFT_PLAN_DFT kml_fftf_plan_dft
#define FFT_PLAN_DFT_1D kml_fftf_plan_dft_1d
#define FFT_PLAN_DFT_2D kml_fftf_plan_dft_2d
#define FFT_PLAN_DFT_3D kml_fftf_plan_dft_3d
#define FFT_DESTROY_PLAN kml_fftf_destroy_plan
#define FFT_FREE kml_fftf_free
#define FFT_EXECUTE_DFT kml_fftf_execute_dft
#else
#define FFT_COMPLEX kml_fft_complex
#define FFT_PLAN kml_fft_plan
#define FFT_MALLOC kml_fft_malloc
#define FFT_PLAN_DFT kml_fft_plan_dft
#define FFT_PLAN_DFT_1D kml_fft_plan_dft_1d
#define FFT_PLAN_DFT_2D kml_fft_plan_dft_2d
#define FFT_PLAN_DFT_3D kml_fft_plan_dft_3d
#define FFT_DESTROY_PLAN kml_fft_destroy_plan
#define FFT_FREE kml_fft_free
#define FFT_EXECUTE_DFT kml_fft_execute_dft
#endif

void PrintComplex(FFT_COMPLEX* complex, const int length)
{
	printf("{\n");
	int i;
	for (i = 0; i < length; i++)
	{
		printf("    {%.6f, ", complex[i].r);
		printf("%.6f}", complex[i].i);
		if (i != length - 1)
		{
			printf(",");
		}
		printf("\n");
	}
	printf("}\n");
}

int main()
{
	MPI_Init(NULL, NULL);
	// Dimension of FFT. The constraint is 1 ≤ rank ≤ 3.
	int rank = 2;
	// Indicates an array whose dimension is rank, including the size of each dimension in the FFT sequence. 
	// The constraint is n[i] ≥ 1, for i in 0 to rank - 1.
	int* n;
	n = (int*)FFT_MALLOC(sizeof(int) * rank);
	n[0] = 2;
	n[1] = 3;
	double init[6][2] = {{ 120, 0 }, { 8, 8 }, { 0, 0 }, { 0, 16 }, { 0, 16 }, { -8, 8 }};

	FFT_COMPLEX* in;
	in = (FFT_COMPLEX*)FFT_MALLOC(sizeof(FFT_COMPLEX) * n[0] * n[1]);
	int i;
	for (i = 0; i < n[0] * n[1]; i++)
	{
		in[i].r = init[i][0];
		in[i].i = init[i][1];
	}

	printf("Input complex:\n");
	PrintComplex(in, n[0] * n[1]);

	FFT_COMPLEX* out;
	out = (FFT_COMPLEX*)FFT_MALLOC(sizeof(FFT_COMPLEX) * n[0] * n[1]);
	FFT_PLAN plan;

	/*
	 * Create a plan for the n-dimensional complex-to-complex (C2C) transform of a single contiguous data sequence.
	 */
	plan = FFT_PLAN_DFT(rank, n, in, out, KML_FFT_FORWARD, KML_FFT_ESTIMATE);
	FFT_EXECUTE_DFT(plan, in, out);

	printf("FFT_PLAN_DFT --> OutPut complex:\n");
	PrintComplex(out, n[0] * n[1]);

	/*
	 * Create a plan for the one-dimensional C2C transform of a single contiguous data sequence.
	 */
	plan = FFT_PLAN_DFT_1D(n[0], in, out, KML_FFT_FORWARD, KML_FFT_ESTIMATE);
	FFT_EXECUTE_DFT(plan, in, out);

	printf("FFT_PLAN_DFT_1D --> OutPut complex:\n");
	PrintComplex(out, n[0] * n[1]);

	/*
	 * Create a plan for the two-dimensional C2C transform of a single contiguous data sequence.
	 */
	plan = FFT_PLAN_DFT_2D(n[0], n[1], in, out, KML_FFT_FORWARD, KML_FFT_ESTIMATE);
	FFT_EXECUTE_DFT(plan, in, out);

	printf("FFT_PLAN_DFT_2D --> OutPut complex:\n");
	PrintComplex(out, n[0] * n[1]);

	/*
	 * Create a plan for the there-dimensional C2C transform of a single contiguous data sequence.
	 */
	plan = FFT_PLAN_DFT_3D(n[0], n[1], 1, in, out, KML_FFT_FORWARD, KML_FFT_ESTIMATE);
	FFT_EXECUTE_DFT(plan, in, out);

	printf("FFT_PLAN_DFT_3D --> OutPut complex:\n");
	PrintComplex(out, n[0] * n[1]);

	FFT_DESTROY_PLAN(plan);
	FFT_FREE(n);
	FFT_FREE(in);
	FFT_FREE(out);
	MPI_Finalize();
	return EXIT_SUCCESS;
}
