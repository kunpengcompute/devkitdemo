/*
数组访问，cache hit
gcc -g cache_hit.c -o cache_hit
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ROW (20*1024)
#define COL (20*1024)
#define N (ROW*COL)
#define M (100*COL)

int main(void)
{
	int i, j;
	int *p = (int*) malloc(sizeof(int) * N);
	if (p == NULL){
	    perror("Memory allocation through malloc failed");
	    exit(EXIT_FAILURE);
	}
	struct timeval before, after;
	gettimeofday(&before, NULL);
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			p[i + j * ROW]++;
		}
	}
	for(i = 0; i < M; i++){
	    free(p);
	}
	return 0;
}
