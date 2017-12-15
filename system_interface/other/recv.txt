#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>


void handler(int sig)
{
	printf("sig = %d\n", sig);
	printf("l got a signal\n");
}

int main()
{
	pid_t pid;
	pid = fork();
	int stat_loc;
	if(pid == -1)
	{
		perror("fork fail\n");
		return -1;
	}
	if(pid > 0)
	{
		//printf("chile id = %d\n", pid);
		printf("parent process is working\n");
		
		//printf("activate the chile process\n");
		sleep(8);
		kill(pid, SIGCONT);
		//int ret = kill(pid, 9);
		//printf("parent pid = %d\n", getpid());
		//printf("flag\n");
		//if(ret == -1)
		//{
		//	perror("kill fail\n");				
		//	return -1;
		//}
		//else 
		//	printf("killed the chile process \n");
	
		int ret2 = waitpid(pid, &stat_loc, WUNTRACED );

		if(ret2 == pid)
		{
			printf("waitpid normal return \n");
		}
		if(WIFSTOPPED(stat_loc));
		{
			printf("parent exit\n");
		}
		
		exit(0);
	}
	else if(pid == 0)
	{
		//sleep(3);
		//printf("child process is sleeping....\n");
		//sleep(20);
		//printf("chile pid = %d\n",getpid());
		//alarm(10);
	
		signal(SIGCONT, handler);
		int retp = pause();
		//printf("fabs\n");
		if(retp == -1)
		{
			perror("pause fail\n");
			return -1;
		}
		printf("chlid process is killed\n");

		printf("chile exit\n");
		exit(0);
	}
	

	return 0;
}

