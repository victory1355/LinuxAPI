#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *p;
	char buf[128] = {0};
	int len;
	p = fopen(argv[1], "r");
	if(p == NULL)
	{
		perror("open fail");
		return -1;
	}
	
	while(len = fread(buf, sizeof(char), 10, p))
	{
	//	printf("len = %d\n", len);
		printf("%s", buf);
		memset(buf, 0, 10);
		//fflush(stdout);
	}//？？？？

	int fc = fclose(p);
	if(fc == -1)
	{
		perror("fclose fail");
		return -1;
	}
	printf("\nthe end");
	
	
	return 0;
}