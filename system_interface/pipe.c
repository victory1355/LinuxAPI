#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	int fd[2];
	char buf[50];
	int stat_loc;
	bzero(buf, sizeof(buf));
	int ret = pipe(fd);
	if(ret ==-1)
	{
		perror("pipe fail");
		return -1;
	}	
	pid_t pid = fork();
	if(pid == -1)
	{
		perror("fork fail\n");
		return -1;
	}
	if(pid > 0)
	{
		close(fd[0]);	
		write(fd[1], "hello", 6);
		printf("parent send %s\n", "hello");
		close(fd[1]);
		
		wait(&stat_loc);
		printf("parent exit\n");
		//exit(0);
		
	}
	else
	{
		sleep(3);
		close(fd[1]);
		read(fd[0], buf, 6);
		printf("chile receive %s\n", buf);
		close(fd[0]);
		printf("chile exit\n");
		exit(0);
	}
	printf("main exit\n");

	return 0;
}
