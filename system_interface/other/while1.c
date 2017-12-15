#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int main()
{
	printf("pid = %d\n", getpid());
	pid_t p =  getpid();
	int i;
	sleep(60);
	printf("after 10s ,you are going to die\n");
	for(i = 10; i>0; i--)
	{
		printf("%d second passed, be careful\n", i);
		sleep(1);
	}
	//sleep(10);
	kill(p, 9);

	return 0;
}
