#include <unistd.h>
#include <stdio.h>

int main()
{
	pid_t pd;
	int i = 10;


	printf("test0 for fork\n");
	pd = fork();
	printf("test2 for fork\n");
	if( pd == -1)
		printf("fork child PID failure\n");
	else  if(pd == 0)
	{
		//printf("child PID\n");
		//return 0;
		printf("child pd =  %d\n", pd);
		printf("child process\n");
		i = 30;	
		printf("child i =  %d\n", i);
		while(1);
	}
	else if(pd > 0)
	{	
		sleep(1);
		printf("parent pd = %d\n", pd);

		printf("parent process\n");
		printf("parent i = %d\n", i);
		while(1);
		//printf("parent PID\n");
	}
	
	printf("parent exit\n");




	return 0;
}
