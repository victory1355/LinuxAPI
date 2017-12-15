#include <stdio.h>
#include <unistd.h>

int main()
{
	//char buf[1028] = {0};
	int i = 1025;
	while(i--)
		printf("%c", 'a');
	//fflush(1);

	while(1);

	return 0;
}
