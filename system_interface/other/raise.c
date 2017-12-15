#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{

	printf("test raise\n");
	//raise(32);
	raise(63);
	printf("continue....\n");
	return 0;
}
