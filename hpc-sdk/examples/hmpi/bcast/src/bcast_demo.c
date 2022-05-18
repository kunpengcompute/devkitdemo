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
#include <mpi.h>

int InputParamParse(int argc, char** argv)
{
	if (argc == 1)
	{
		return 0;
	}
	// Parameter must be 1
	printf("Invalid parameter.");
	return -1;
}

void BcastSend(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator)
{
	int worldRank;
	int worldSize;
	MPI_Comm_rank(communicator, &worldRank);
	MPI_Comm_size(communicator, &worldSize);

	for (int i = 0; i < worldSize; i++)
	{
		if (i != worldRank)
		{
			MPI_Send(data, count, datatype, i, 0, communicator);
		}
	}
}

void BcastRecv(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator)
{
	MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
}

int main(int argc, char** argv)
{
	if (InputParamParse(argc, argv) != 0)
	{
		return -1;
	}

	MPI_Init(NULL, NULL);

	int rankNum;
	int data = 10000;
	MPI_Comm_rank(MPI_COMM_WORLD, &rankNum);

	if (rankNum == 0)
	{
		printf("Process 0 send data %d\n", data);
		BcastSend(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
	}
	else
	{
		BcastRecv(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Process %d received data %d from root process\n", rankNum, data);
	}

	MPI_Finalize();
}