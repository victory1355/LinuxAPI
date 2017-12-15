#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <strings.h>
#include <unistd.h>

#define PATH "/home/shm"

char *p;
int pid;

void continu(int sig)
{
	printf("got the %d signal from %d process\n", sig, pid);
	fgets(p, 128, stdin);
	bzero(p, 128);	
	kill(pid, 45);
	printf("send the %d signal to %d process\n", 45, pid);
}


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
	sleep(5);
	printf("pid = %d\n", getpid());
	char id[10];
	bzero(id, sizeof(id));
	getpid_byname(argv[1], id);
	pid = atoi(id);
	
	
	//注册信号
	signal(60, continu);
	//printf("%d\n", pid);
	//while(1);
	//新建一个关键字
	key_t key = ftok(PATH, 20);
	if(key == -1)
	{
		perror("ftok fail\n");
		return -1;
	}
	//申请内存空间
	int shmid = shmget(key, 128, IPC_CREAT | 0666);
	if(shmid == -1)
	{
		perror("shmget fail\n");
		return -1;
	}
	//映射到进程空间
	p = shmat(shmid, NULL, 0);
	if(p == NULL)
	{
		perror("shmat fail\n");
		return -1;
		
	}
	else
		printf("shmat ok\n");
	
	//清空申请的内存
	bzero(p, 128);	
	
	
	
	//对内存进行读写操作
	kill(pid, 40);
	printf("send the %d signal to %d process\n", 40, pid);
	fgets(p, 128, stdin);
	kill(pid, 45);
	printf("send the %d signal to %d process\n", 45, pid);
	
	
	while(1)
	{
		if(!strncasecmp(p, "quit", 4))
		{
			break;
		}	
	}
	
	//取消对本进程的内存映射
	int retd = shmdt(p);
	if(retd == -1)
	{
		perror("shmdt fail\n");
		return -1;
	}
	else
		printf("shmdt ok\n");
	//删除内核空间申请的共享内存
	int retc = shmctl(shmid, IPC_RMID, NULL);
	if(retc == -1)
	{
		perror("shmdt fail\n");
		return -1;
	}
	else
		printf("deleted the shm\n");
	
	return 0;
}



























