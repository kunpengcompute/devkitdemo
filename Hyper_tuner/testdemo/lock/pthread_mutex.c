// gcc -g pthread_mutex.c -o pthread_mutex -lpthread
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <sys/timeb.h>

static int num=0;
static int count = 200000000;
static int millisecond_coefficient = 1000;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void Perror(const char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

long long getSystemTime()
{
	struct timeb t;
	ftime(&t);
	return millisecond_coefficient * t.time + t.millitm;
}

void* fun2(void *arg)
{
	pthread_t thread_id = pthread_self();
	printf("the thread2 id is %1d\n",(long)thread_id);
	int i = 1;
	for (i; i <= count; i++){
		pthread_mutex_lock(&mutex);
		num += 1;
		pthread_mutex_unlock(&mutex);
	}
}

int main()
{
	int err;
	pthread_t thread1;
	pthread_t thread2;
	thread1 = pthread_self();
	printf("the thread1 id is %1d\n",(long)thread1);
	long long start = getSystemTime();
	err = pthread_create(&thread2, NULL, fun2, NULL);
	if(err != 0){
		Perror("cant create thread2\n");
	}
	int i = 1;
	for (i;i <= count;i++){
		pthread_mutex_lock(&mutex);
		num += 1;
		pthread_mutex_unlock(&mutex);
	}
	pthread_join(thread2, NULL);
	long long end = getSystemTime();
	printf("The num is %d,pay %11d ms\n", num, (end-start));
	return 0;
}
