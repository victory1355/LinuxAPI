#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void usage(int argc, char **argv)
{
	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s <PORT>   <IP>\n", argv[0]);
		exit(0);
	}
}

int main(int argc, char **argv) // ./Rose 50001
{
	usage(argc, argv);	

	// 1, 创建了一个TCP套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in srvaddr;
	socklen_t len = sizeof(srvaddr);
	bzero(&srvaddr, len);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(atoi(argv[1]));
	srvaddr.sin_addr.s_addr = inet_addr(argv[2]);
	//srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);	
	//printf("")

	// 2, 绑定地址（IP + PORT）
	bind(fd, (struct sockaddr *)&srvaddr, len);

	// 3, 设置监听状态
	listen(fd, 3);

	// 4, 坐等对方的连接请求
	struct sockaddr_in cliaddr;
	len = sizeof(cliaddr);
	bzero(&cliaddr, len);

	int connfd = accept(fd, (struct sockaddr *)&cliaddr, &len);

	char *msg = calloc(1, 1024);
	while(1)
	{
		bzero(msg, 1024);
	//	if(read(connfd, msg, 1024) == 0)
	//		break;
		read(connfd, msg, 1024);	
	//	printf("flag\n");
		printf("from Jack: %s\n", msg);
	//	bzero(msg, 1024);
	//	printf("please input some msg\n");
	//	scanf("%s", msg);
	//	write(connfd, msg, 1024);
	}

	close(connfd);
	close(fd);

	return 0;
}
