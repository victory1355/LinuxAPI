#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

int main()
{
	printf("pid = %d\n", getpid());
	pause();
	printf("pause done\n");


	return 0;
}
