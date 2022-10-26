#include "stdio.h"
#include "omp.h"

#define NUM_THREADS 128
static long num_steps = 6000000000;
double step;

int main()
{
    long i, nthreads;
    double x, pi, sum[NUM_THREADS];
    step = 1.0/(double)num_steps;
    omp_set_num_threads(NUM_THREADS);
    double start = omp_get_wtime();
    printf("start caculate!\n");
    #pragma omp parallel
    {
        long i, id, nthrds;
        double x;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if (if == 0) nthreads = nthrds;
        for (i = id, sum[id] = 0.0; i<num_steps; i+=nthrds*4){
            x = (i+0.5)*step;
            sum[id] += 4.0/(1.0+x*x);
            x = (i + nthrds + 0.5)*step;
            sum[id] += 4.0/(1.0+x*x);
            x = (i + nthrds*2 + 0.5)*step;
            sum[id] += 4.0/(1.0+x*x);            
            x = (i + nthrds*3 + 0.5)*step;
            sum[id] += 4.0/(1.0+x*x);            
        }
        for (i = 0, pi=0.0; i < nthreads; i++)
    {
        pi += step * sum[i];

    }
    }
    for (i = 0, pi=0.0; i < nthreads; i++)
    {
        pi += step * sum[i];

    }
    double end = omp_get_wtime();
    printf("pi = %f, number of threads = %d, time = %f s\n",pi, NUM_THREADS, end-start);
    return 0;
}
