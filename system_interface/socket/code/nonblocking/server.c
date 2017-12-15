#include "head4sock.h"
#include <string.h>
#include <stdbool.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAXSIZE 80
#define MAXCLI 20

typedef struct
{
	int clifd[MAXCLI];
	int count;
}client;

void init_cli(client *cli)
{
	int i;
	for(i=0; i<MAXCLI; i++)
		(cli->clifd)[i] = -1;

	cli->count == 0;
}

bool add_cli(client *cli, int fd)
{
	if(cli->count >= MAXCLI -1)
		return false;

	cli->clifd[cli->count++] = fd;
	return true;
}

int main(int argc, char **argv)
{
	if(argc != 2)
		exit(1);

	client *cli = (client *)malloc(sizeof(client));
	init_cli(cli);

	// TCP套接子
	int sockfd;
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	// 使得本程序在退出之后立刻释放关联的地址
	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);

	struct sockaddr_in srvaddr, cliaddr;
	bzero(&srvaddr, sizeof srvaddr);
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(atoi(argv[1]));

	Bind(sockfd, (struct sockaddr *)&srvaddr,
					sizeof srvaddr);
	Listen(sockfd, 5);

	int connfd;
	socklen_t len = sizeof cliaddr;
	bzero(&cliaddr, len);

	// 将套接子sockfd设置为非阻塞
	long flag = fcntl(sockfd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(sockfd, F_SETFL, flag);


	char buf[MAXSIZE];
	int nread, i;
	while(1)
	{
		if((connfd=Accept(sockfd,
				(struct sockaddr *)&cliaddr, &len)) > 0)
		{
			// set connfd as NON-blocking fd
			flag = fcntl(connfd, F_GETFL);
			flag |= O_NONBLOCK;
			fcntl(connfd, F_SETFL, flag);

			if(!add_cli(cli, connfd)){
				printf("a new client has been refused:"
					"%s:%d\n",
					inet_ntoa(cliaddr.sin_addr),
					ntohs(cliaddr.sin_port));
				close(connfd);
				break;
			}

			printf("new connection: %s:%hu\n",
					inet_ntoa(cliaddr.sin_addr),
					ntohs(cliaddr.sin_port));
		}

		// 逐个查看这些已连接套接字有没有数据到达
		for(i=0; i<MAXCLI; i++)
		{
			bzero(buf, MAXSIZE);
			if((cli->clifd)[i] == -1)
				break;
			if((nread=read(cli->clifd[i], buf, MAXSIZE)) > 0)
				write(STDOUT_FILENO, buf, strlen(buf));
		}
	}

	return 0;
}
