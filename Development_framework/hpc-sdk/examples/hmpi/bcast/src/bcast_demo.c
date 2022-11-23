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
 * Description: KML blas library demo
 * Refrence: www.mpitutorial.com
 * Create: 2022-05-17
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int InputParameterParse(int argc, char** argv)
{
	if (argc == 1)
	{
		return 0;
	}
	// Parameter must be 1
	printf("Invalid parameter.\n");
	return -1;
}

double BcastData(int trialsNum, int elementsNum, int* sendData)
{
	if (trialsNum == 0)
	{
		return 0;
	}
	double totalMPIBcastTime = 0.0;

	int i;
	for (i = 0; i < trialsNum; i++)
	{
		// An MPI barrier completes after all group members have entered the barrier.
		MPI_Barrier(MPI_COMM_WORLD);
		// MPI_Wtime returns a floating-point number of seconds, representing elapsed wall-clock time since some time in the past.
		totalMPIBcastTime -= MPI_Wtime();
		// MPI_Bcast broadcasts a message from the process with rank root to all processes of the group, itself included.
		MPI_Bcast(sendData, elementsNum, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		totalMPIBcastTime += MPI_Wtime();
	}
	return totalMPIBcastTime;
}

int main(int argc, char** argv)
{
	if (InputParameterParse(argc, argv) != 0)
	{
		return -1;
	}
	// Send data numbers.
	int elementsNum = 10000;
	int trialsNum = 10;
	int rankNum;
	int sizeOfInt = sizeof(int);
	double totalMPIBcastTime = 0.0;
	int* sendData = (int*)malloc(sizeof(int) * elementsNum);
	if (sendData == NULL)
	{
		printf("Out of memory.\n");
		return -1;
	}
	memset(sendData, 0, elementsNum);

	// Init.
	MPI_Init(NULL, NULL);
	// This function gives the rank of the process in the particular communicator’s group.
	MPI_Comm_rank(MPI_COMM_WORLD, &rankNum);

	totalMPIBcastTime = BcastData(trialsNum, elementsNum, sendData);

	if (rankNum == 0)
	{
		printf("Data size = %d, Trials = %d\n", elementsNum * sizeOfInt, trialsNum);
		printf("Average of MPI_Bcast time = %lf seconds\n", totalMPIBcastTime / trialsNum);
	}
	free(sendData);
	MPI_Finalize();
}