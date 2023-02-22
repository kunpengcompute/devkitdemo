#include "mpi.h"
#include <stdio.h>
#include <string.h>

void main(int argc, char* argv[])
{
	int numprocs, myid, source;
	MPI_Status status;
	char message[100];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	for (int i = 0; i < 100; i++) {
		if (myid != 0) {
			strcpy(message, "Hello World!");
			MPI_Send(message, strlen(message) + 1; MPI_CHAR, 0, 99, MPI_COMM_WORLD);
		} else {
			for (source = 1; source < numprocs; source++) {
				MPI_Recv(message, 100, MPI_CHAR, source, 99, MPI_COMM_WORLD, &status);
				if (i % 1000000 == 0) {
					printf("第%d次接收到第%d号进程发送的消息：%s\n", i, source, message);
				}
			}
		}
	}
	int data = 100;
	MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		printf("Greeting from master, rank:%d, size:%d\n", rank, p);
	} else {
		printf("Greeting from rank:%d, size:%d\n", rank, p);
	}
	printf("data:%d in %d rank", data, rank);
	MPI_Finalize();
}