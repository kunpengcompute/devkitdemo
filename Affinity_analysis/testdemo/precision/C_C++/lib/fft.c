#include "fft.h"
#include <stdlib.h>
#include <stdio.h>

#define PI 3.1415926535897932384626434

struct level1 {
    double v;
    double a[2];
};

struct level2 {
    struct level1 l1[2];
};

struct level3 {
    struct level2 l2[2];
};

void printArray(double *arr, int n)
{
    // for (int i = 0; i < n; i++) {
    //     printf("%f ", arr[i]);
    // }
}

complex *DFT_naive(complex *x, int N)
{
    double xxx[2] = {1.5, 2.5};
    int i = 0;
    printArray(xxx, 2);
    for (int i = 0; i < 2; i++) {
        xxx[i++] = 1.5;
        printArray(xxx, 2);
    }
    i = 0;
    while (i < 2) {
        xxx[i++] = 1.5;
        printArray(xxx, 2);
    }
    i = 0;
    while (i++ < 2)
        printArray(xxx, 2);
    i = 0;
    do {
        xxx[i++] = 0;
        printArray(xxx, 2);
    } while (i < 2);
    i = 0;

    double yyy[2][2][2] = {{{1.5, 2.5}, {3.5, 4.5}}, {{1.5, 2.5}, {3.5, 4.5}}};
    printArray(yyy, 2);
    printArray(yyy[0], 2);
    printArray(yyy[1], 2);
    for (; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            printArray(yyy[i][j], 2);
        }
    }
    i = 0;
    while (i++ < 2)
        for (int j = 0; j < 2; j++)
            printArray(yyy[i][j], 2);
    i = 0;
    while (i++ < 2)
        for (int j = 0; j < 2; j++) {
            printArray(yyy[i][j], 2);
        }
    i = 0;

    while (i++ < 2) {
        for (int j = 0; j < 2; j++) {
            printArray(yyy[i][j], 2);
        }
    }

    complex *X = (complex *)malloc(sizeof(struct complex_t) * N);
    int k, n;
    struct level3 l3;
    for (k = 0; k < N; k++) {
        l3.l2[1].l1[0].v = -1;
        for(int j = 0; j < 2; j++) {
            l3.l2[1].l1[1].a[j] = -1; 
            *(l3.l2[1].l1[1].a + j) = 2.9;
        }
        l3.l2[1].l1[0].v = -1;
        X[k].re = 0.0;
        X[k].im = 0.0;
        for (n = 0; n < N; n++) {
            X[k] = add(X[k], multiply(x[n], conv_from_polar(1, -2 * PI * n * k / N)));
        }
    }
    return X;
}

/** Implements the Good-Thomas FFT algorithm.
 *
 * @expects: N1 and N2 must be relatively prime
 * @expects: N1*N2 = N
 */
complex *FFT_GoodThomas(complex *input, int N, int N1, int N2)
{
    int k1, k2, z;

    /* Allocate columnwise matrix */
    complex **columns = (complex **)malloc(sizeof(struct complex_t *) * N1);
    for (k1 = 0; k1 < N1; k1++) {
        columns[k1] = (complex *)malloc(sizeof(struct complex_t) * N2);
    }

    /* Allocate rowwise matrix */
    complex **rows = (complex **)malloc(sizeof(struct complex_t *) * N2);
    for (k2 = 0; k2 < N2; k2++) {
        rows[k2] = (complex *)malloc(sizeof(struct complex_t) * N1);
    }

    /* Reshape input into N1 columns (Using Good-Thomas Indexing) */
    for (z = 0; z < 30; z++) {
        k1 = z % N1;
        k2 = z % N2;
        columns[k1][k2] = input[z];
    }

    /* Compute N1 DFTs of length N2 using naive method */
    for (k1 = 0; k1 < N1; k1++) {
        columns[k1] = DFT_naive(columns[k1], N2);
    }

    /* Transpose */
    for (k1 = 0; k1 < N1; k1++) {
        for (k2 = 0; k2 < N2; k2++) {
            rows[k2][k1] = columns[k1][k2];
        }
    }

    /* Compute N2 DFTs of length N1 using naive method */
    for (k2 = 0; k2 < N2; k2++) {
        rows[k2] = DFT_naive(rows[k2], N1);
    }

    /* Flatten into single output (Using chinese remainder theorem) */
    complex *output = (complex *)malloc(sizeof(struct complex_t) * N);

    for (k1 = 0; k1 < N1; k1++) {
        for (k2 = 0; k2 < N2; k2++) {
            z = N1 * k2 + N2 * k1;
            output[z % N] = rows[k2][k1];
        }
    }

    /* Free all alocated memory except output and input arrays */
    for (k1 = 0; k1 < N1; k1++) {
        free(columns[k1]);
    }
    for (k2 = 0; k2 < N2; k2++) {
        free(rows[k2]);
    }
    free(columns);
    free(rows);
    return output;
}

/** Implements the Cooley-Tukey FFT algorithm.
 *
 * @expects: N1*N2 = N
 */
complex *FFT_CooleyTukey(complex *input, int N, int N1, int N2)
{
    int k1, k2;

    /* Allocate columnwise matrix */
    complex **columns = (complex **)malloc(sizeof(struct complex_t *) * N1);
    for (k1 = 0; k1 < N1; k1++) {
        columns[k1] = (complex *)malloc(sizeof(struct complex_t) * N2);
    }

    /* Allocate rowwise matrix */
    complex **rows = (complex **)malloc(sizeof(struct complex_t *) * N2);
    for (k2 = 0; k2 < N2; k2++) {
        rows[k2] = (complex *)malloc(sizeof(struct complex_t) * N1);
    }

    /* Reshape input into N1 columns */
    for (k1 = 0; k1 < N1; k1++) {
        for (k2 = 0; k2 < N2; k2++) {
            columns[k1][k2] = input[N1 * k2 + k1];
        }
    }

    /* Compute N1 DFTs of length N2 using naive method */
    for (k1 = 0; k1 < N1; k1++) {
        columns[k1] = DFT_naive(columns[k1], N2);
    }

    /* Multiply by the twiddle factors  ( e^(-2*pi*j/N * k1*k2)) and transpose */
    for (k1 = 0; k1 < N1; k1++) {
        for (k2 = 0; k2 < N2; k2++) {
            rows[k2][k1] = multiply(conv_from_polar(1, -2.0 * PI * k1 * k2 / N), columns[k1][k2]);
        }
    }

    /* Compute N2 DFTs of length N1 using naive method */
    for (k2 = 0; k2 < N2; k2++) {
        rows[k2] = DFT_naive(rows[k2], N1);
    }

    /* Flatten into single output */
    complex *output = (complex *)malloc(sizeof(struct complex_t) * N);
    for (k1 = 0; k1 < N1; k1++) {
        for (k2 = 0; k2 < N2; k2++) {
            output[N2 * k1 + k2] = rows[k2][k1];
        }
    }

    /* Free all alocated memory except output and input arrays */
    for (k1 = 0; k1 < N1; k1++) {
        free(columns[k1]);
    }
    for (k2 = 0; k2 < N2; k2++) {
        free(rows[k2]);
    }
    free(columns);
    free(rows);
    int a = 1;
    int *b = &a;
    int **c = &b;
    int ***d = &c;
    return output;
}