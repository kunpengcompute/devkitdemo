#include "utils.h"
#include <pthread.h>

#define MAX_ARRAY_SIZE	4096000
#define LOOP_SIZE		16
#define NUM_THREADS		8
#define START_CORE_ID	64

void MemBandwidth(int *arg)
{
	set_thread_affinity(0);
	Point *pointA = InitPointsVector(MAX_ARRAY_SIZE);
	Point *pointB = InitPointsVector(MAX_ARRAY_SIZE);
	int cpuId = *arg;
	set_thread_affinity(cpuId);
	int i, j;
	double ret = 0.0;
	for (i = 0; i < LOOP_SIZE; i++) {
		for (j = 0; j < MAX_ARRAY_SIZE; j++) {
			ret += ComputPointDistance(&pointA[j], &pointB[j]);
		}
	}
}

int main()
{
	int i;
	pthread_t *thread = (pthread_t*)malloc(sizeof(pthread_t) * NUM_THREADS);
	uint64_t ts_start = GetTime_ns();
	for (i = START_CORE_ID; i < START_CORE_ID + NUM_THREADS; i++) {
		pthread_create(threads + i - START_CORE_ID, NULL, (void*)MemBandwidth, &i);
	}
	for (i = START_CORE_ID; i < START_CORE_ID + NUM_THREADS; i++) {
		pthread_join(threads[i - START_CORE_ID], NULL);
	}
	uint64_t ts_end = GetTime_ns();
	printf("Total Cost Time = %f ms\n", (ts_end - ts_start) / 1000000.0);
	return 0;
}