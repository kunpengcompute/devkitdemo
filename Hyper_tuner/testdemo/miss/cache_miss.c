/*
gcc -g cache_miss.c -o cache_miss
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ROW (20*1024)
#define COL (20*1024)
#define N (ROW*COL)

int main(void)
{
	int i, j;
	int *p = (int*) malloc(sizeof(int) * N);
	double msecs = 0;
	struct timeval before, after;
	gettimeofday(&before, NULL);
	for (i = 0; i < COL; i++)
	{
		for (j = 0; j < ROW; j++)
		{
			p[i + j * COL]++;
		}
	}
	gettimeofday(&after, NULL);
	msecs = (after.tv_sec - before.tv_sec) * 1000.0+(after.tv_sec - before.tv_sec)/1000.0;
	printf("execution time = %2.3lf ms\n", msecs);
	return 0;
}
