#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

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
	printf("IP:%s, PORT:%hu", inet_ntoa(htons(atoi(argv[2]))), htons(atoi(argv[2])))
	// 2, 连接对端（IP + PORT）
	if( !connect(fd, (struct sockaddr *)&srvaddr, len) )
		printf("connect ok\n");
	else
	{
		perror("connect:");
		return -1;
	}

	// 3, 创建一条线程专门用来接收服务端发来的消息
	pthread_t tid;
	int ret = pthread_create(&tid, NULL, routine, (void *)fd);
	if(ret == -1)
	{
		fprintf(stderr, "%s", strerror(errno));
		return -1;
	}
	else
		printf("create threads ok\n");
	// 4, 主线程专门负责监听标准输入，将数据及时发给服务端
	char *msg = calloc(1, 1024);
	if(msg != NULL)
		return -1;
		//printf("calloc ok\n");
	
	while(1)
	{
		//printf("flag\n");
		bzero(msg, 1024);
		printf("input some data:");
		if(fgets(msg, 1024, stdin) == NULL)
			break;
		//printf("msg :%s\n", msg);
		if(write(fd, msg, strlen(msg)) == -1)
			break;
		else
			printf("send:%s\n", msg);
	}

	close(fd);
	return 0;
}

void *routine(void *arg)
{
	int fd = (int)arg;
	char buf[128];
	
	while(1)
	{
		printf("[%d]\n", __LINE__);
		bzero(buf, sizeof(buf));
		//接受服务器发来的消息
		while(read(fd, buf, sizeof(buf)) == 0)
			break;
		//printf("[%d]\n", __LINE__);
		printf("get message: %s\n", buf);
	}
	printf("exit routine\n");
	
	pthread_exit(NULL);
	
}


































