#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ROW (100*1024)
#define COL (10*1024)
#define N (ROW*COL)

int main(void)
{
	int i, j;
	int *p = (int*) malloc(sizeof(int) * N);
	double msecs = 0
	for (i=0; i<ROW; i++)
	{
		for (j=0; j<COL; j++)
		{
			p[i + j*ROW]++;
		}
	}
	return 0;
}
