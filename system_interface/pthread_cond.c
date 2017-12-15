#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

unsigned int count = 0;

pthread_mutex_t mutex;
pthread_cond_t cond;


//消费者进程
void *decrement_count(void *arg)
{
	printf("welcome to decrement_count\n");
	
	int rete = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	int rets = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if(rete != 0 || rets != 0)
	{
		printf("set fail\n");
	}
	/*while(1)
	{
		printf("decrement_count\n");
		sleep(2);
	}*/
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(count == 0)
		{
			printf("count == 0\n");
			usleep(300000);
			pthread_cond_wait(&cond, &mutex);
		}
		count = count-1;
		printf("tid1:count = %d\n", count);
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
	
	//sleep(3);
	pthread_exit((void *)0);
}

void increment_count(void *arg)
{
	printf("welcome to increment_count\n");

	int rets = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	int retc = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if(rets != 0 || retc != 0)
	{
		printf("set fail\n");
	}
	/*while(1)
	{
		printf("increment_count\n");
		sleep(2);
	}*/
	while(1)
	{
		pthread_mutex_lock(&mutex);
		if(count == 0)
		{
			count = count+1;
			printf("tid2:count = %d\n", count);
			pthread_cond_signal(&cond);
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	//sleep(10);
	//while(1);
	//sleep(3);
	pthread_exit((void *)0);
}

int main()
{
	pthread_t tid1, tid2;
	
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int reta = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(reta != 0)
	{
		printf("set attr fail\n");
	}
	
	int ret11 = pthread_create(&tid1, &attr, (void *)decrement_count, NULL);
	printf("[+]success to create decrement_count\n");
	//sleep(2);
	int ret22 = pthread_create(&tid2, &attr, (void *)increment_count, NULL);
	printf("[+]success to create increment_count\n");
	if(ret11 != 0 || ret22 != 0)
	{
		printf("create thread fail\n");
		return -1;
	}
	
	//if(tid1)
	//sleep(10);
	
	//sleep(2);
	
	//pthread_mutex_destroy(&mutex);
	//pthread_cond_destroy(&cond);
	
	sleep(10);
	int ret1 = pthread_cancel(tid1);
	int ret2 = pthread_cancel(tid2);
	
	if(ret1==0 && ret2==0)
	{
		printf("normal cancel\n");
	}
	
	/*int retj1 = pthread_join(tid1, NULL);
	int retj2 = pthread_join(tid2, NULL);
	if(retj1 != 0 || retj2 != 0)
	{
		printf("join error\n");
	}*/
	//printf("hello\n");
	while(1)
	{
		printf("main\n");
		sleep(20);
		break;
	}
	printf("[+]main thread exit....\n");
	pthread_mutex_destroy(&mutex);
	int err = pthread_cond_destroy(&cond);
	if(err != 0)
	{
		printf("destroy fail\n");
	}
	//pthread_exit((void *)0);
	
	return 0;
}



























