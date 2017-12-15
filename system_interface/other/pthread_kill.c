#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

void sig_handler(int sig)
{
	printf("enter signal handler, signum = %d\n", sig);
	printf("sig:pid = %d, tid = %lu\n", getpid(), pthread_self());
	return ;
}

void *start(void *arg)
{
	printf("[-]enter thread func\n");
	
	//sleep(5);
	//配置信号处理属性
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	//绑定信号处理方式
	sigaction(SIGQUIT, &act, NULL);
	
	
	//-----------------------------------------------------
	//清空信号集的所有信号
	sigemptyset(&act.sa_mask);
	//在当前的信号掩码中添加信号集set
	pthread_sigmask(SIG_BLOCK, &act.sa_mask, NULL);
	
	sigaddset(&act.sa_mask, SIGQUIT);
	//替换原来的屏蔽信号集
	pthread_sigmask(SIG_SETMASK, &act.sa_mask, NULL);
	
	//sigdelset(&act.sa_mask, SIGQUIT);
	//pthread_sigmask(SIG_SETMASK, &act.sa_mask, NULL);
	
	sigdelset(&act.sa_mask, SIGQUIT);
	//在当前的信号掩码中添加信号集set
	pthread_sigmask(SIG_BLOCK, &act.sa_mask, NULL);
	//----------------------------------------------------------
	
	printf("thread:pid = %d, tid = %lu\n", getpid(), pthread_self());
	while(1)
	{
		printf("[-]thread working...\n");
		sleep(2);
	}
	
	printf("[-]thread exit\n");
	
	pthread_exit((void *)1);
	
}

int main()
{
	pthread_t tid;
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	int errs = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(errs != 0)
	{
		printf("set detach state fail\n");
	}
	else
	{
		int ret = pthread_create(&tid, &attr, (void *)start, NULL);
		if(ret != 0)
		{
			printf("create fail\n");
			return -1;
		}
	}
	pthread_attr_destroy(&attr);
	
	printf("[+]create thread successful\n");

	//void *retval;
	/*int retj = pthread_join(tid, (void **)&retval);
	if(retj != 0)
	{
		printf("join fail\n");
	}
	else
		printf("retval = %p\n", (int *)retval);
	*/
	printf("main:pid = %d, tid = %lu\n", getpid(), pthread_self());
	int err;
	
	struct sigaction act;
	bzero(&act, sizeof(act));
	act.sa_handler = sig_handler;
	
	//------------------------------------------------
	sigemptyset(&act.sa_mask);
	pthread_sigmask(SIG_BLOCK, &act.sa_mask, NULL);
	
	sigaddset(&act.sa_mask, SIGQUIT);
	pthread_sigmask(SIG_BLOCK, &act.sa_mask, NULL);
	
	sigdelset(&act.sa_mask, SIGQUIT);
	pthread_sigmask(SIG_SETMASK, &act.sa_mask, NULL);
	//---------------------------------------------------
	
	sigaction(SIGQUIT, &act, NULL);
	
	while(1)
	{
		printf("[+]main working...\n");
		sleep(3);
		err = pthread_kill(tid, SIGQUIT);
		if(err != 0)
		{
			printf("[+]no thread with the ID thread could be found\n");
			continue;
		}
		sleep(3);
		err = pthread_kill(pthread_self(), SIGQUIT);
		if(err != 0)
		{
			printf("[+]no thread with the ID thread could be found\n");
			continue;
		}
	
	}
	//sleep(10);
	printf("[+]main thread exit\n");
	
	return 0;
}