#include "utils.h"

#define MAX_ARRAY_SIZE    409600
#define LOOP_SIZE         1
#define BLOCK_SIZE        4096

void MemoryBoundBench(Point *pointA, double *ret, int n)
{
    if (pointA == NULL || ret == NULL) {
        return;
    }
    
    int i, j;
    #pragma omp parallel for
    for (i = 0; i < n; i ++) {
        ret[i] = 0.0;
        for (j = 0; j < n; j++) {
            ret[i] += pointA[i].x + pointA[j].y;
        }
    }
}

void MemoryBoundBench_OPT(Point *pointA, double *ret, int n)
{
    if (pointA == NULL || ret == NULL) {
        return;
    }
    
    int i, j, k;
    for (k = 0; k< n; k += BLOCK_SIZE) {
        #pragma omp parallel for
        for (i = 0; i < n; i ++) {
            ret[i] = 0.0;
            for (j = 0; j < BLOCK_SIZE; j++) {
                ret[i] += pointA[i].x + pointA[j + k].y;
            }
        }
    }
}

int main()
{
    Point *pointA = InitPointsVector(MAX_ARRAY_SIZE);
    
    double *ret = (double*)malloc(MAX_ARRAY_SIZE * sizeof(double));
    if (ret == NULL) {
        printf("ERROR: Memory Allocate Failed!\n");
        exit(1);
    }
    
    uint64_t ts_start = GetTime_ns();
    int i;
    for (i = 0; i < LOOP_SIZE; i++) {
        MemoryBoundBench(pointA, ret, MAX_ARRAY_SIZE);
        //MemoryBoundBench_OPT(pointA, ret, MAX_ARRAY_SIZE);
    }
    uint64_t ts_end = GetTime_ns();
    printf("Total Cost Time = %f ms\n", (ts_end - ts_start) / 1000000.0);
    return 0;
}