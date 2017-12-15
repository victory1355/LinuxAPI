#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{

	printf("l am the process named hello\n");
	printf("pid = %d, ppid  = %d\n",getpid(), getppid());

	return 0;
}
