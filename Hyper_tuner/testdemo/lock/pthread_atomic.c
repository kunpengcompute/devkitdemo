# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <sys/timeb.h>
# include <stdatomic.h>

static atomic_int num = 0;
static int count = 800000000;

void Perror(const char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

long long getSystemTime()
{
	struct timeb t;
	ftime(&t);
	return 1000 * t.time + t.millitm;
}

void* fun2(void *arg){
	pthread_t thread_id = pthread_self();
	printf("the thread2 id is %1d\n",(long)thread_id);
	int i = 1;
	for (i;i <= count; i++){
		num += 1;
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
	for (i;i <= count; i++){
		num += 1;
	}
	pthread_join(thread2, NULL);
	long long end = getSystemTime();
	printf("the num is %d,pay %11d ms\n", num, (end-start));
	return 0;
}
