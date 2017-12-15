#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

struct pthread_param
{
	int id;
	char parambuf[128];
};

void *end(void *arg)
{
	//分离状态
	int ret1 = pthread_detach(pthread_self());
	if(ret1 != 0)
	{
		perror("pthread detach fail");
		return NULL;
	}
	struct pthread_param param;
	param.id = 100;
	memcpy(param.parambuf,"hello mosij", 12);
	
	printf("second pthread:pid = %d, tid = %lu\n", getpid(), pthread_self());
	printf("second:id = %d, buf = %s\n", param.id, param.parambuf);
	int i =0;
	for(i ; i<10; i++)
	{
		printf("i = %d\n", i);
		sleep(1);
	}
	int ret = 1;
	pthread_exit((void *)&ret);
}

void *start(void *arg)
{
	struct pthread_param param;
	param.id = 100;
	memcpy(param.parambuf,"hello linux", 12);
	
	//while(1);
	printf("first pthread:pid = %d, tid = %lu\n", getpid(), pthread_self());
	//sleep(10);
	//printf("id = %d, buf = %s\n", ((struct pthread_param *)arg)->id, ((struct pthread_param *)arg)->parambuf);
	printf("first:id = %d, buf = %s\n", param.id, param.parambuf);
	//while(1);
	int i =0;
	for(i ; i<10; i++)
	{
		printf("i = %d\n", i);
		sleep(1);
	}
	int ret = 1;
	pthread_exit((void *)&ret);
	//exit(0);
	//i=1;
	//return (void *)i;
	//return ;
	//printf("hello\n");
}

int main()
{
	pid_t pid;
	pthread_t tid;
	
	pid = getpid();
	tid = pthread_self();
	
	printf("main pthread:pid = %d, tid = %lu\n", (int)pid, (unsigned long)tid);
	
	pthread_t fpthread, pthread2;
	
	struct pthread_param param;
	param.id = 100;
	memcpy(param.parambuf,"hello world", 12);
	
	printf("main:id = %d, buf = %s\n", param.id, param.parambuf);
	
	int err = pthread_create(&fpthread, NULL ,(void *)start, NULL);
	if(err != 0)
	{
		perror("fail to creat");
		return -1;
	}
	int err2 = pthread_create(&pthread2, NULL, (void *)end, NULL);
	if(err2 != 0)
	{
		perror("fail to creat");
		return -1;
	}
	printf("fpthread = %lu, pthread2 = %lu\n", fpthread, pthread2);
	int  a = 11;
	/*while(a--)
	{
		printf("main\n");
		sleep(1);
	}*/
	//while(1);
	//sleep(10);
	//pthread_exit(NULL);
	//void **i = NULL;
	pthread_join(pthread2,   NULL);
	//printf("i = %d\n", **(int **)i);
	/*if(rete != 0)
	{
		perror("join fail\n");
		return -1;
	}*/
	printf("exit......\n");
	
	return 0;
}




























