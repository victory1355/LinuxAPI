#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
//客户端
#define PATH "/home/myfifo"
int main()
{
	if(access(PATH, F_OK) != 0)
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
	int a, b, c;
	printf("welcome to chat\n");
	while(1)
	{
		scanf("%d%d",&a, &b);
		write(fd, (int *)&a, 4);
		write(fd, (int *)&b, 4);
		sleep(2);
		read(fd, (int *)&c, 4);
		printf("a+b = %d\n", c);
		//printf("read %s\n", buf);
	}
	
	return 0;
}




























