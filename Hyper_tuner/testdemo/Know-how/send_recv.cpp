#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int numprocs, myid, source;
	MPI_Status status;
	MPI_Request request;
	char message[100];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	for (int i = 0; i < 100; i++) {
		if (myid != 0) {
			strcpy(message, "Hello World!");
			MPI_Send(message, strlen(message) + 1; MPI_CHAR, 0, 99, MPI_COMM_WORLD);
			//MPI_Isend(message, strlen(message) + 1; MPI_CHAR, 0, 99, MPI_COMM_WORLD, &request);
		} else {
			for (source = 1; source < numprocs; source++) {
				MPI_Recv(message, 100, MPI_CHAR, source, 99, MPI_COMM_WORLD, &status);
				//MPI_Irecv(message, 100, MPI_CHAR, source, 99, MPI_COMM_WORLD, &request);
				if (i % 1000000 == 0) {
					printf("No:%d, receive message from %d: %s\n", i, source, message);
				}
			}
		}
	}
	MPI_Finalize();
	return 0;
}