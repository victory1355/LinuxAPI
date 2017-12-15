#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

int main(int argc ,char *argv[])
{	
	char w_buf[50];
	int fd, i;
	if(access(argv[1], F_OK) != 0)
	{
		int ret = mkfifo(argv[1], 0777);
		if(ret == -1)
		{
			printf("mkfifo fail\n");
			return -1;
		}
	}
	fd = open(argv[1], O_RDWR);
	if(fd == -1)
	{
		perror("open fail");
		return -1;
	}
	FILE * fp = fopen("./a.txt", "r+");
	if(fp == NULL)
	{
		perror("fopen fail");
		return -1;
	}
	while(!feof(fp))
	{
		bzero(w_buf, sizeof(w_buf));
		fread(w_buf,sizeof(char), sizeof(w_buf), fp);
		write(fd, w_buf, strlen(w_buf));
	}
	sleep(5);
	kill(atoi(argv[2]), 18);
	
	printf("send finish\n");
	
	
	close(fd);
		
	return 0;
}

