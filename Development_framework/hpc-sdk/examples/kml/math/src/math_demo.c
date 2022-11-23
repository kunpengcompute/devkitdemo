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
 * 		KML_MATH library functions demo demonstrates the use process of trigonometric functions.
 * Create: 2022-05-15
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <km.h>
#include <mpi.h>

int main()
{
	MPI_Init(NULL, NULL);
	double sinx, cosx;
	sincos(M_PI_4, &sinx, &cosx);
	printf("Input:\npi/4 = %.14f\n\n", M_PI_4);
	printf("Output:\nsincos { sin(pi/4) = %.14f, cos(pi/4) = %.14f }\n\n", sinx, cosx);
	printf("Output:\nasin(sin(pi/4)) = %.14f\n\n", asin(sinx));
	printf("Output:\nacos(cos(pi/4)) = %.14f\n\n", acos(cosx));
	MPI_Finalize();
	return EXIT_SUCCESS;
}

