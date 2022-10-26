#include "stdio.h"
#include "omp.h"
static long num_steps = 6000000000;
double step;

int main()
{
    long i;
    double x, pi, sum = 0.0;
    step = 1.0/(double)num_steps;
    double start = omp_get_wtime();
    for (i=0; i<num_steps; i++){
        x = (i+0.5)*step;
        sum = sum + 4.0/(1.0+x*x);
    }
    pi = step * sum;
    double end = omp_get_wtime();

    printf("pi = %f, time = %f s\n", pi, end-start);
    return 0;
    
}