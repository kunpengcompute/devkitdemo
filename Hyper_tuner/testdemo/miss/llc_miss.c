/*
gcc -g llc_miss.c -o llc_miss
*/
#include <stdio.h>
#include <malloc.h>

void llcMiss3Array(int *** pArr, int x, int y, int z, int m);

int main(void)
{
	int x, y, z, m;
	int *** array = NULL;
	x = 1000*1024;
	y = 100*1024;
	z = 100*1024;
	m = 5*1024;
	llcMiss3Array(array, x, y, z, m);
	array = NULL;
	return 0;
}
void llcMiss3Array(int *** pArr, int x, int y, int z, int m){
    int i, j, k;
    pArr = (int ***)malloc(m * sizeof(int **));
    for(i = 0;i < x;i++){
        pArr[i] = (int **)malloc(y * sizeof(int **));
        for (j = 0;j < y;j++){
            pArr[i][j] = (int *)malloc(y * sizeof(int));
            for(k = 0; k < z; k++){
                pArr[i][j][k] = 100 * (i + j + k + 1) + 1000*x + 100*y + 10*z + 101*m + 19999999999;
                printf("%d ", pArr[i][j][k]);
            }
            printf("\n");
        }
    printf("\n");
    }
}
