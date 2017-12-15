#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

void handler(int sig)
{
	printf("recv finished\n");
	exit(0);
}

int main(int argc ,char *argv[])
{	
	char w_buf[20];
	int fd, i;
	int read_len;
	FILE *fp;
	printf("pid = %d\n", getpid());
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
	
	fp = fopen("./recv.txt", "w+");
	if(fp == NULL)
	{
		perror("fopen fail\n");
		return -1;
	}
	signal(18, handler);
	while(1)
	{
		bzero(w_buf, sizeof(w_buf));
		read_len = read(fd, w_buf, sizeof(w_buf));
		fwrite(w_buf,sizeof(char), read_len, fp);
		printf("recv: %s\n", w_buf);
	}
	close(fd);
	printf("bye\n");
	
	
	return 0;
}

