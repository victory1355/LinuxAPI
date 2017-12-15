#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

//#include <pthread.h>

void usage(int argc, char **argv)
{
	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s <IP> <PORT>\n", argv[0]);
		exit(0);
	}
}

void *routine(void *arg);

int main(int argc, char **argv) // ./client 192.168.10.xxx 50001
{
	usage(argc, argv);	

	// 1, 创建了一个TCP套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1)
	{
		perror("socket fail:");
		return -1;
	}
	else
		printf("socket ok\n");
	
	struct sockaddr_in srvaddr;
	socklen_t len = sizeof(srvaddr);
	bzero(&srvaddr, len);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(atoi(argv[2]));
	srvaddr.sin_addr.s_addr = inet_addr(argv[1]);
	
	//printf("IP:%s, PORT:%hu", inet_ntoa(inet_addr((argv[1]))), htons(atoi(argv[2])))
	
	
	// 2, 连接对端（IP + PORT）
	if( !connect(fd, (struct sockaddr *)&srvaddr, len) )
		printf("connect ok\n");
	else
	{
		perror("connect:");
		return -1;
	}
	
	
	pid_t pid = fork();
	if(pid == -1)
	{
		perror("fork fail");
		return -1;
	}
	// 3, 创建一个进程专门用来接收服务端发来的消息
	//int len;
	if(pid == 0)
	{

		char buf[128];
		//bzero(buf, sizeof(buf));
		
		while(1)
		{
			printf("[%d]\n", __LINE__);
			bzero(buf, sizeof(buf));
			//接受服务器发来的消息
			while( (len = read(fd, buf, sizeof(buf)) ) == -1 && errno == EINTR);
			
			if(len == -1)
			{
				perror("read fail");
				return -1;
			}
			if(len >0)
				printf("get message: %s\n", buf);
			if(len == 0)
			{
				printf("exit\n");
				break;
			}
		}
		printf("exit process\n");
		printf("[%d]\n", __LINE__);
		exit(0);
		
	}
	// 4, 父进程专门负责监听标准输入，将数据及时发给服务端
	if(pid > 0)
	{
		char *msg = calloc(1, 1024);
		if(msg != NULL)
			return -1;
		int statloc;
		while(1)
		{
			//关闭文件描述符
			close(fd);
			
			waitpid(pid, &statloc,  WNOHANG);     //非阻塞
			bzero(msg, 1024);
			printf("[%d]\n", __LINE__);
			printf("input some data:");
			if(fgets(msg, 1024, stdin) == NULL)
				break;
			
			if(write(fd, msg, strlen(msg)) == -1)
				break;
			else
				printf("send:%s\n", msg);
		}
	}

	close(fd);
	return 0;
}



































