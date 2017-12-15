#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
	
	int i=0;
	printf("alarm before\n");

	alarm(9);
			
	printf("alarm after\n");
	
	while(i<15)
	{
		printf("cnt = %d\n", ++i);
		sleep(1);
	}
	return 0;	
	
}
