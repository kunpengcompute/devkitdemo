/*
gcc -g llc_miss.c -o llc_miss
*/
#include <stdio.h>
#include <malloc.h>

void Malloc3Array(int *** pArr, int x, int y, int z);
void Free3Array(int *** pArr, int x, int y);

int main(void)
{
	int x, y, z, m;
	int *** array = NULL;
	x = 1000 * 1024;
	y = 100 * 1024;
	z = 100 * 1024;
	m = 5 * 1024;
	Malloc3Array(array, x, y, z);
	Free3Array(array,x,y);
	array = NULL;
	return 0;
}
void Malloc3Array(int *** pArr, int x, int y, int z){
    int i, j, k;
    pArr = (int ***)malloc(y * sizeof(int **));
    for(i = 0;i < x;i++){
        pArr[i] = (int **)malloc(y * sizeof(int **));
        for (j = 0;j < y;j++){
            pArr[i][j] = (int *)malloc(y * sizeof(int));
            for(k = 0; k < z; k++){
                pArr[i][j][k] = i + j + k + 1;
                printf("%d ", pArr[i][j][k]);
            }
            printf("\n");
        }
    printf("\n");
    }
}
void Free3Array(int *** pArr, int x, int y){
    int i, j;
    for (i = 0; i < x; i++){
        for (j = 0 ;j < y;j++){
            free(pArr[i][j]);
            pArr[i][j] = 0;
        }
        free(pArr[i]);
        pArr[i] = 0;
    }
    free(pArr);
}