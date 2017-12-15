#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	pid_t pd;
	int i = 10;
	pd = fork();
	if(pd == -1)
	{
		perror("fork fail");
		return -1;
	}
	if(pd == 0)
	{
		printf("child pd = %d\n", pd);
		printf("child:PID = %d, PPID= %d\n", getpid(), getppid());
	}
	if(pd>0)
	{
		pid_t return_pid;
		int stat_loc;
		printf("parent pd = %d\n", pd);
		printf("parent:PID =%d, PPID = %d\n", getpid(), getppid());

		return_pid = wait(&stat_loc);
		//return_pid = waitpid(-1, &stat_loc, 0);
                if(WIFEXITED(stat_loc))
			printf("return_pid = %d\n", return_pid);
	}

	
	return 0;
}
