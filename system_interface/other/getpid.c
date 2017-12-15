#include <unistd.h>
#include <stdio.h>

int main()
{
	pid_t pid_p1, pid_p2;
	printf("pid_p1 = %d, pid_p2 = %d\n", getpid(), getppid());

	pid_t p = fork();
	if(p== -1)
		return -1;
	if(p == 0)
	{
		printf("parent ID = %d, child ID = %d\n", getppid(), getpid());
	}
	else
	{
		sleep(1);
		printf(" mypid = %d, p = %d\n", getpid(), p);
	}
	
		

	return 0;
}
