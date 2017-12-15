#include "sock.h"
#include <signal.h>
#include <sys/wait.h>

#define MAXSIZE 80

void usage(const char *info)
{
	printf("Usage: %s <srv IP>:<srv PORT>\n", info);
	exit(0);
}

int main(int argc, char **argv)
{
	if(argc != 3)
		usage(argv[0]);
	
	int sockfd;
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);

	struct sockaddr_in srvaddr, cliaddr;
	bzero(&srvaddr, sizeof srvaddr);
	srvaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], (struct sockaddr *)&srvaddr.sin_addr);
	srvaddr.sin_port = htons(atoi(argv[2]));

	Connect(sockfd, (struct sockaddr *)&srvaddr, sizeof srvaddr);

	fd_set rset;
	int maxfd = -1;
	char buf[MAXSIZE];

	while(1)
	{
		// 1, 将描述符集合清零
		FD_ZERO(&rset);

		// 2, 将感兴趣的fd统统加入到集合中
		FD_SET(STDIN_FILENO, &rset);
		FD_SET(sockfd, &rset);

		// 3，丢给select让他慢慢等...
		maxfd = sockfd;
		select(maxfd+1, &rset, NULL, NULL, NULL);

		// 4, 如果select返回，在来检查集合中还残留哪些fd
		// 这些残留的fd，就是就绪的fd，可以操作
		if(FD_ISSET(STDIN_FILENO, &rset) > 0){
			bzero(buf, MAXSIZE);

			read(STDIN_FILENO, buf, MAXSIZE);
			write(sockfd, buf, strlen(buf));
		}

		if(FD_ISSET(sockfd, &rset) > 0){
			bzero(buf, MAXSIZE);

			read(sockfd, buf, MAXSIZE);
			write(STDOUT_FILENO, buf, strlen(buf));
		}
	}
	return 0;
}
