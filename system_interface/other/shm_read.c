#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

#define PATH "/home/shm"

char *p;
int pid;

//信号40
void stop(int sig)
{
	printf("got the %d signal\n", sig);
	
}
//信号45
void continu(int sig)
{
	char buf[128];
	bzero(buf, sizeof(buf));
	printf("got the %d signal from %d process\n", sig, pid);
	sscanf(p,"%s", buf);
	printf("buf:%s\n", buf);
	
	//发送信号
	kill(60, pid);
	printf("send signal %d to %d process\n", 60, pid);
}
//获取其他进程的ID
int getpid_byname(char *argv, char *pid)
{
	if(argv == NULL)
		return -1;
	struct dirent *pd;
	char buf[100];
	char buf_read[100];
	
	DIR * dir = opendir("/proc/");
	
	if(dir == NULL)
	{
		printf("open dir fail\n");
		return -1;
	}
	//printf("argv[1] = %s\n",argv[1]);
	
		
	while((pd = readdir(dir)) != NULL)
	{
		if(!strcmp(pd->d_name, ".") || !strcmp(pd->d_name, ".."))
			continue;
		
		if(pd->d_type == DT_DIR)
		{
			
			bzero(buf, sizeof(buf));
			bzero(buf_read, sizeof(buf_read));
			sprintf(buf, "%s%s/%s", "/proc/", pd->d_name, "comm");
			int fd = open(buf, O_RDONLY);
			if(fd == -1)
			{
				//perror("open fail\n");
				continue;
			}
			else
				read(fd, buf_read, strlen(argv));
			if(!strcmp(buf_read, argv))
			{
				strcpy(pid, pd->d_name);
				return 0;
			}
		}
	}
	//printf("getpid = %d\n", getpid());
	return -1;
}
int main(int argc ,char *argv[])
{
	printf("pid = %d\n", getpid());
	
	char id[10];
	bzero(id, sizeof(id));
	getpid_byname(argv[1], id);
	pid = atoi(id);
	//printf("%d\n", pid);
	//while(1)；
	
	//注册信号
	signal(40, stop);
	signal(45, continu);
	
	key_t key = ftok(PATH, 20);
	if(key == -1)
	{
		perror("ftok fail\n");
		return -1;
	}
	int shmid = shmget(key, 128, IPC_CREAT | 0666);
	if(shmid == -1)
	{
		perror("shmget fail\n");
		return -1;
	}
	
	p = shmat(shmid, NULL, 0);
	if(p == NULL)
	{
		perror("shmat fail\n");
		return -1;
		
	}
	else
		printf("shmat ok\n");
	//判断输入的字符串	
	while(1)
	{
		if(!strncasecmp(p, "quit", 4))
		{
			break;
		}	
	}
	
	return 0;
}






















