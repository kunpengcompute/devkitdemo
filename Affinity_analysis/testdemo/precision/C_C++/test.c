#include "fft.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    complex * input1 = (complex*) malloc(sizeof(struct complex_t) * 30);
    complex * input2 = (complex*) malloc(sizeof(struct complex_t) * 30);
    complex * result1, * result2;
    
    /* Init inputs */
    for (int i=0; i < 30; i++) {
        input1[i].re = (double) i;
        input1[i].im = 0.0;
        input2[i].re = (double) i;
        input2[i].im = 0.0;
    }
    
    /* Do FFT */
    result1 = FFT_CooleyTukey(input1 = (complex*) malloc(sizeof(struct complex_t) * 30), 30, 6, 5);
    result2 = FFT_GoodThomas(input2, 30, 6, 5);

    struct ap {
        double *p;
        double *pa[2];
    };

    double array1[3] = {1,2,3};
    double *array_p = array1;
    array_p[0] = 2.2;
    *array_p = 2.3;
    *(array_p + 2) = 3.3;

    struct ap p;
    p.p = array1;
    // printf("p.p szie = %d\n", sizeof(p.p));
    p.p[0] = 2.2;
    *p.p = 2.3;
    *(p.p + 2) = 3.3;

    p.pa[0] = array1;
    // printf("p.pa[0] szie = %d\n", sizeof(p.pa[0]));
    p.pa[0][1] = 2.3;
    *p.pa[0] = 4.4;
    *(p.pa[0] + 1) = 3.3;

    double array2[3] = {4,5,6};
    double last = 0;
    for(int i = 0; i < 3; i++) {
        array1[i]++;
        ++array1[i];
        array1[i] += array2[i] + array2[i];
        array1[i] -= array2[i];
        array1[i]--;
        array1[i] = 12.3;
        --array1[i];
    }

    
    last++;
    ++last;
    last--;
    --last;
    
    complex cp1 = {1,2};
    double res;
    res = abs2(cp1);

    return 0;
}