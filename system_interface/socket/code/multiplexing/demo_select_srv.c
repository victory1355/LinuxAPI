#include "sock.h"
#include <signal.h>
#include <sys/wait.h>

#define MAXSIZE 80

void usage(const char *info)
{
	printf("Usage: %s  <srv PORT>\n", info);
	exit(0);
}

int main(int argc, char **argv)
{
	if(argc != 2)
		usage(argv[0]);

	// TCP
	int listenfd;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	// 地址复用
	const int on = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);

	struct sockaddr_in srvaddr, cliaddr;
	bzero(&srvaddr, sizeof srvaddr);
	
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(atoi(argv[1]));

	Bind(listenfd, (struct sockaddr *)&srvaddr, sizeof srvaddr);

	Listen(listenfd, 3);

	
	int connfd;
	

	// 定义了一个fd集合
	fd_set rset;
	int maxfd = -1;
	char buf[MAXSIZE];

	while(1)
	{
		// 1，无限等待对方的连接，一直等... ...
		connfd = Accept(listenfd, NULL, NULL);
		
		// 2，添加节点，添加fd集合
		add_fd_node(&rset, connfd);
		
		
		// 3，检测fd集合的状态
		scan_rset(head, &rset);
		
		// 操作fd集合
		FD_ZERO(&rset);
		FD_SET(STDIN_FILENO, &rset); // 将标准输入（键盘）的fd加入rset中
		FD_SET(connfd, &rset); // 将已连接套接字connfd加入rset中
		maxfd = connfd;

		// 静静地等待rset中的一个或多个fd读就绪
		select(maxfd+1, &rset, NULL, NULL, NULL);
	
		// 看一看标准输入是否读就绪，如果确实就绪，那么就
		// 把键盘键入的数据读出来并发送给对方
		if(FD_ISSET(STDIN_FILENO, &rset) > 0){
			bzero(buf, MAXSIZE);

			if(read(STDIN_FILENO, buf, MAXSIZE) == 0)
				exit(0);
			write(connfd, buf, strlen(buf));
		}
	
		// 看一看TCP通信端点是否读就绪，如果确实就绪，说明
		// 对方已经发来数据，那么就读出来并且显示在屏幕上
		else if(FD_ISSET(connfd, &rset) > 0){
			bzero(buf, MAXSIZE);

			read(connfd, buf, MAXSIZE);
			write(STDOUT_FILENO, buf, strlen(buf));
		}
	}
	return 0;
}
