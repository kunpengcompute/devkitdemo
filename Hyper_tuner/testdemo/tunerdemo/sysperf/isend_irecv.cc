#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <omp.h>
#include <sched.h>
#include <string.h>
#include <mpi.h>

#define TASK_TIME 1
#define LONG_TASK_TIME 10
#define LOOP_SIZE 10
#define ARRAY_SIZE 102400

void print_list(int* list, int count) {
    for (int i = 0; i < count; i++) {
        printf("%d ", list[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    printf("===============Start===============");
    int flag;
    MPI_Initialized(&flag);
    printf("flag: %d\n", flag);
    if (flag == 0) {
        printf("No initialized\n");
    } else {
        printf("Initialized\n");
    }
    int ierr = MPI_Init(&argc, &argv);
    assert(ierr == MPI_SUCCESS);
    MPI_Initialized(&flag);
    printf("flag: %d\n", flag);
    if (flag == 0) {
        printf("No initialized\n");
    } else {
        printf("Initialized\n");
    }
    int world_rank;
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    assert(ierr == MPI_SUCCESS);
    int world_size;
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    assert(ierr == MPI_SUCCESS);
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    printf("processor %s, rank %d out of %d processors\n", processor_name, world_rank, world_size);
    if (world_size < 2) {
        fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    int send_len = ARRAY_SIZE;
    int recv_len = ARRAY_SIZE;
    int number[ARRAY_SIZE] = {0};
    int tag = 111;
    MPI_Status status;
    MPI_Request request;
    for (int loop_count = 0; loop_count < LOOP_SIZE; loop_count++) {
        if (world_rank == 0) {
            for (int from_rank = 1; from_rank != world_size; ++from_rank) {
                sleep(TASK_TIME);
                ierr = MPI_Irecv(&number, recv_len, MPI_INT, from_rank, tag, MPI_COMM_WORLD, &request);
                assert(ierr == MPI_SUCCESS);
                printf("Processor %d received number from process %d, status.MPI_SOURCE:%d, status_MPI_TAG:%d, status.MPI_ERROR:%d.\n",
                world_rank, from_rank, status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);
                
                ierr = MPI_Isend(&number, send_len, MPI_INT, from_rank, tag, MPI_COMM_WORLD, &request);
                assert(ierr == MPI_SUCCESS);
                printf("Process %d send number tag %d to process %d\n", world_rank, tag, from_rank);
            }
        } else {
            for (int i = 0; i < ARRAY_SIZE; i++) {
                number[i] += world_rank;
            }
            int to_rank = 0;
            ierr = MPI_Isend(&number, send_len, MPI_INT, to_rank, tag, MPI_COMM_WORLD, &request);
            assert(ierr == MPI_SUCCESS);
            printf("Process %d send number tag %d to process %d\n", world_rank, tag, to_rank);
            print_list(number, send_len > 10 ? 10 : send_len);
            if (loop_count == 0) {
                sleep(LONG_TASK_TIME);
            }
            
            ierr = MPI_Irecv(&number, recv_len, MPI_INT, to_rank, tag, MPI_COMM_WORLD, &request);
            assert(ierr == MPI_SUCCESS);
            printf("Processor %d received number from process %d, status.MPI_SOURCE:%d, status_MPI_TAG:%d, status.MPI_ERROR:%d.\n",
            world_rank, to_rank, status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);
        }
    }
    MPI_Wait(&request, &status);
    MPI_Finalize();
    return 0;
}
