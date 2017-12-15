#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>


void *start(void *arg)
{
	int err;
	
	pthread_detach(pthread_self());
	
	
	//err = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);  /*关闭取消功能*/
	//if(err != 0)
	//{
	//	printf("set cancel state fail\n");
		//return -1;
	//}
	
	//printf("[-]enter thread start func\n");
	//sleep(2);
	
	//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);    /*启动取消功能*/
	//pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);   /*立即有效*/
	
	/*新增内容，改为延时有效*/
	//pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	
	//printf("[-]thread exit\n");
	//pthread_cancel(pthread_self());
	printf("hello world\n");
	int i= 10;
	while(i--)
	{
		printf("gogogo\n");
		sleep(1);
	}
	pthread_exit((void *)1);
	//return (void *)0;
}
int main()
{
	pthread_t tid;
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	/* 设置为分离状态*/
	int reta = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	//int reta = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(reta != 0)
	{
		printf("set jionable fail\n");
	}
	int err = pthread_create(&tid, &attr, (void *)start, NULL);
	if(err !=0 )
	{
		printf("create fail\n");
		return -1;
	}
	
	
	printf("[+]create thread successful\n");
	
	//pthread_exit((void *)1);
	
	
	void *retval;
	int retj = pthread_join(tid, (void **)&retval);
	if(retj != 0)
	{
		printf("join fail\n");
		return -1;
	}
	else
		printf("jion ok\n");
	
	printf("[+]retval = %p\n", (int *)retval);
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);    /*关闭取消功能*/
	//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);    /*启动取消功能*/
	//pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);   /*立即有效*/
	
	/*新增内容，改为延时有效*/
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	/*err = pthread_cancel(pthread_self());             /*发送取消信号*/
	//if(err != 0)
	//{
	//	printf("cancel thread fail\n");
	//	return -1;
	//}
	
	sleep(1);
	
	int i= 10;
	while(i--)
	{
		printf("main:move....\n");
		sleep(1);
	}
	
	printf("[+]main thread exit\n");
	printf("[+]main thread exit\n");
	printf("[+]main thread exit\n");
	printf("[+]main thread exit\n");
	printf("[+]main thread exit\n");
	
	return 0;
}