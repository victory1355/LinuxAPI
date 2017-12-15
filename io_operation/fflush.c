#include <stdio.h>


int main(int argc , char *argv[])
{	
	FILE *p;
	p = fopen(argv[1], "a+");
	if(p == NULL)
	{
		perror("open fail");
		return -1;
	}	
	//fprintf(p, "%s", "hello");
	fwrite("hello", sizeof(char), 6, p);
	//fflush(p);

	//while(1);
	fclose(p);

	return 0;
}
