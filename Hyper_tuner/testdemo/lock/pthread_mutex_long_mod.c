#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/timeb.h>
#include <math.h>

#define N    100000

static long g_num = 1;
static long g_count = 100000000;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

long long GetSystemTime()
{
    struct timeb t;
    ftime(&t);
    return 1000 * t.time + t.millitm;
}

void* Func(void *arg)
{
    int i = 1;
    for (; i <= g_count; ++i) {
        pthread_mutex_lock(&mutex);
        g_num += 1;
        printf("tid = %u, num = %d\n", pthread_self(), g_num);
        pthread_mutex_unlock(&mutex);

        float s = 0;
        int j;
        for (j = 0; j < N; j++) {
            s += sqrt(j);
        }    
    }
}

int main()
{
    int err;
    pthread_t thread1;
    pthread_t thread2;

    long long start = GetSystemTime();

    //create thread
    err = pthread_create(&thread1, NULL, Func, NULL);
    if (err != 0) {
        perror("can't create thread1\n");
        exit(EXIT_FAILURE);
    }

    err = pthread_create(&thread2, NULL, Func, NULL);
    if (err != 0) {
        perror("can't create thread2\n");
        exit(EXIT_FAILURE);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    long long end = GetSystemTime();

    printf("The num is %d, pay %lld ms\n", g_num, (end - start));

    return 0;
}