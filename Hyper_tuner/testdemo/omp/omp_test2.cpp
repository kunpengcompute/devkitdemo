#include "stdio.h"
#include "omp.h"

#define THREAD_NUMS 4
static long MAX_N = 6000000000;
double step;
int main()
{
    long i, tid;
    double step = 1. 0 /(double)MAX_N;
    double sum[THREAD_NUMS];
    double pi = 0.0;
    omp_set_num_threads(THREAD_NUMS);
    double start = omp_get_wtime();
#pragma omp parallel private(tid)
    {
        double x;
        tid = omp_get_thread_num();
        for (i = tid, sum[tid] = 0.0; i<MAX_N; i+=THREAD_NUMS)
        {
            x = (i+0.5)*step;
            sumt[tid] += 4.0/(1.0+x*x);     
        }
        printf("tid=%d,sum[%d=%lf\n",tid,tid,sum[tid*step]);

    }
    for (i = 0; i < THREAD_NUMS; i++)
        pi += sum[i];
    pi *= step;
    printf("pi=%lf\n", pi);
    double end = omp_get_wtime();
    printf("pi = %f, number of threads = %d, time = %f s\n",pi, THREAD_NUMS, end-start);
    return 0;
}
