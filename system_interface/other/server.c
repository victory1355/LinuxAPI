
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
//服务器端
#define PATH "/home/myfifo"
int main()
{
	int buf[10];
	if(access(PATH, F_OK)!= 0)
	{
		int ret = mkfifo(PATH, 0777);
		if(ret == -1)
		{
			perror("mkfifo fial\n");
			return -1;
			
		}
	}
	
	int fd = open(PATH, O_RDWR);
	if(fd == -1)
	{
		perror("open fail");
		return -1;
	}
	bzero(buf, sizeof(buf));
	printf("welcome to super server\n");
	int c;
	while(read(fd, buf, 8))
	{
		//printf("buf[1] = %d, buf[2] = %d\n", buf[0], buf[1]);
		//while(1);
		
		c = buf[0]  +  buf[1];
		//printf("c = %d\n", c);
		printf("working........\n");
		write(fd, (int *)&c, 4);
		bzero(buf, sizeof(buf));
	}
	
	return 0;
}
































