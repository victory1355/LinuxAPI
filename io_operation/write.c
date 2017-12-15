#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
		
	FILE *fp;
	char buf[128] = "hello";
	fp = fopen(argv[1], "w+");
	if(fp == NULL)
	{	
		perror("open fail");
		return -1;
	}
	int w_cnt = fwrite(buf ,sizeof(char), strlen(buf), fp);
	if(w_cnt != -1)
	{
		printf("has writen %d data\n", w_cnt);
	}

	int fc = fclose(fp);
	if(fc == -1)
	{
		perror("fclose fail");
		return -1;
	}
	return 0;
}
