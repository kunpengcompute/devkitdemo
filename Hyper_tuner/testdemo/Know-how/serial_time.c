#include "utils.h"

#define MAX_ARRAY_SIZE	4096000
#define LOOP_SIZE		4096

void SerialTimeBench(Point *pointA, Point *pointB, double *pointDis, int n)
{
	assert(pointA != NULL && pointB != NULL && pointDis != NULL);
	int i;
	for (i = 0; i < n; i++) {
		pointDis[i] = ComputPointDistance(&pointA[i], &pointB[i]);
	}
}

void SerialTimeBench_OPT(Point *pointA, Point *pointB, double *pointDis, int n)
{
	assert(pointA != NULL && pointB != NULL && pointDis != NULL);
	int i;
	#pragma omp parallel for shared (n) private (i)
	for (i = 0; i < n; i++) {
		pointDis[i] = ComputPointDistance(&pointA[i], &pointB[i]);
	}
}

int main()
{
	int i;
	Point *pointA = InitPointsVector(MAX_ARRAY_SIZE);
	Point *pointB = InitPointsVector(MAX_ARRAY_SIZE);
	
	double *pointDis = (double*)malloc(MAX_ARRAY_SIZE * sizeof(double));
	if (pointDis == NULL) {
		printf("ERROR: Memory Allocate Failed!\n");
		exit(1);
	}
	
	uint64_t ts_start = GetTime_ns();
	for (i = 0; i < LOOP_SIZE; i++) {
		SerialTimeBench(pointA, pointB, pointDis, MAX_ARRAY_SIZE);
		//SerialTimeBench_OPT(pointA, pointB, pointDis, MAX_ARRAY_SIZE);
	}
	uint64_t ts_end = GetTime_ns();
	printf("Total Cost Time = %f ms\n", (ts_end - ts_start) / 1000000.0);
	return 0;
}