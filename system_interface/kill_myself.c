#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()

{
	printf("pid = %d\n", getpid());
	//alarm(9);
	while(1)
	{
		//printf("10 seconds later,you will expose\n");
		//sleep(10);
		//kill(getpid(), 9);
		//raise(9);
	}
	printf("hello world\n");
		
	return 0;
}
