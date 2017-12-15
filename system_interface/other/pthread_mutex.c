#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int count = 1;
pthread_mutex_t mutex;

void *start1(void *arg)
{
	int i;
	sleep(1);
	printf("start1 ...\n");
	
	for(; ; )
	{
		pthread_mutex_lock(&mutex);
		count++;
		pthread_mutex_unlock(&mutex);
		printf("start1 count = %d\n", count);
		sleep(2);
	}

	printf("start1 thread done\n");
	
	pthread_exit((void *)0);
}
void *start2(void *arg)
{
	int i;
	sleep(1);
	printf("start2 ...\n");
	
	
	for(; ;)
	{
		pthread_mutex_lock(&mutex);
		count--;
		pthread_mutex_unlock(&mutex);
		printf("start2 count = %d\n", count);
		
		sleep(3);
	}
	
	printf("start2 thread done\n");
	
	pthread_exit((void *)0);
}


int main()
{
	pthread_mutex_init(&mutex, NULL);
	
	pthread_t tid1, tid2;
	int ret1 = pthread_create(&tid1, NULL, (void *)start1, NULL);
	int ret2 = pthread_create(&tid2, NULL, (void *)start2, NULL);
	if(ret1 != 0 || ret2 != 0)
	{
		printf("create thread fail\n");
		return -1;
	}
	
	void *retval1, *retval2;
	pthread_join(tid1, (void **)&retval1);
	pthread_join(tid2, (void **)&retval2);
	
	pthread_mutex_destroy(&mutex);
	
	printf("[+]exit....\n");
	
	return 0;
}






























