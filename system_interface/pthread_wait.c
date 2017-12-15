#include <stdio.h>
#include <pthread.h>
#include <strings.h>
#include <unistd.h>

char buf[128];

pthread_mutex_t mutex;
pthread_cond_t cond;

void *start1(void *arg)
{
	while(1)
	{
		printf("start1\n");
		int ret = pthread_mutex_lock(&mutex);
		if(ret != 0)
		{
			printf("lock fail\n");
		}
		printf("start1 working...\n");
		bzero(buf, sizeof(buf));
		fgets(buf, sizeof(buf), stdin);
		printf("input data:%s\n", buf);
		int retb = pthread_cond_broadcast(&cond);
		if(retb != 0)
		{
			printf("broadcast fail\n");
		}
		int retu = pthread_mutex_unlock(&mutex);
		if(retu != 0)
		{
			printf("unlock fail\n");
		}
		printf("zhanxuegui\n");
		sleep(3);
	}
}
void *start2(void *arg)
{
	sleep(1);
	while(1)
	{
		printf("start2\n");
		pthread_mutex_lock(&mutex);
		printf("start2 working...\n");
		printf("buf = %s\n", buf);
		pthread_cond_wait(&cond, &mutex);              //pthread_cond_wait不会解锁
		//pthread_mutex_unlock(&mutex);
		sleep(5);
	}
}
void *start3(void *arg)
{
	char buf_start3[128];
	bzero(buf_start3, sizeof(buf_start3));
	sleep(2);
	while(1)
	{
		printf("start3\n");
		pthread_mutex_lock(&mutex);
		printf("start3 working...\n");
		//fgets(buf, stdin);
		sprintf(buf_start3, "change %s", buf);
		printf("buf_start3 = %s\n", buf_start3);
		bzero(buf_start3, sizeof(buf_start3));
		pthread_cond_wait(&cond, &mutex);
		//pthread_mutex_unlock(&mutex);
		sleep(2);
	}
}
int main()
{
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);
	
	pthread_t tid1,tid2,tid3;
	
	pthread_create(&tid1, NULL, (void *)start1, NULL);
	pthread_create(&tid2, NULL, (void *)start2, NULL);
	pthread_create(&tid3, NULL, (void *)start3, NULL);
	
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	
	
	printf("main exit...\n");
	return 0;
}






















