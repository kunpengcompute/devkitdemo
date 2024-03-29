#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

long long *mem_leak(long long *n0, long long *n1)
{

   long long *v = (long long *) calloc(1024,sizeof(long long));
   *v = *n0 + *n1;
   return v;
}

void *child(void *arg)
{
   long long n0 = 0;
   long long n1 = 1;
   long long *v = NULL;
   int n = 2;
   for (n = 2; n > 0; n++) {
       v = mem_leak(&n0, &n1);
       n0 = n1;
       n1 = *v;
       free(v);
       printf("%dth => %11d\n", n, *v);
       sleep(1);
   }
}


int main(void)
{
   pthread_t tid;
   pthread_create(&tid, NULL, child, NULL);
   pthread_join(tid, NULL);
   printf("main thread exit\n");
   return 0;
}

