/*
 * Copyright 2023 Huawei Technologies Co., Ltd
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <omp.h>

#define COUNT 10000

void Test(int n)
{
  for (int i = 0; i < 5000; i++){}
  printf("%d, 线程编号: %d", n, omp_get_thread_num());
}

void openmp_test()
{
  int sum = 0;
  #pragma omp parallel for reduction(+:sum)
  for (int i = 0; i < COUNT; i++)
  {
    Test(i);
    sum += i;
    printf("sum: %d\n", sum);
  }
  printf("\n\n");
  printf("end sum: %d\n", sum);
}

int main(int argc, char **argv){
  MPI_Init(NULL, NULL);
  
  // Get the rank and size in the original communicator
  int world_rank, world_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  
  int color = world_rank % 4; // Determine color based on row

  // Split the communicator based on the color and use the original rank for ordering
  MPI_Comm row_comm;
  MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &row_comm);

  int row_rank, row_size;
  MPI_Comm_rank(row_comm, &row_rank);
  MPI_Comm_size(row_comm, &row_size);

  openmp_test();

  printf("WORLD RANK/SIZE: %d/%d --- ROW RANK/SIZE: %d/%d\n",
    world_rank + 1, world_size, row_rank + 1, row_size);

  MPI_Comm_free(&row_comm);

  MPI_Finalize();
}
