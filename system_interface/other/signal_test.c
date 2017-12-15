#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

void handler1(int sig)
{
	printf("afsl got a signal %d\n", sig);
}

void handler2(int sig)
{
	int stat_loc;
	printf("l got a signal %d\n", sig);
	wait();
	printf("finish\n");
	exit(0);
}
int main(int argc ,char *argv[])
{
	pid_t pid;
	pid = fork();
	if(pid == -1)
	{
		perror("fork fail");
		return -1;
	}
	if(pid == 0)
	{
		sleep(10);
		kill(getppid(), 65);
		sleep(5);
		kill(getppid(), 14);
	}
	if(pid > 0)
	{
		signal(65, handler1);
		signal(14, handler2);
		while(1)
		{
			printf("welcome !\n");
			sleep(5);
		}
	}
	
}



















