#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>


int main(int argc ,char **argv)
{

	int fd = socket(AF_INET, SOCK_STREAM, 0);	
	if(fd == -1)
	{
		perror("socket fail\n");
		return -1;
	}
	struct sockaddr_in client_addr;
	bzero(&client_addr, sizeof(client_addr));

	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(atoi(argv[1]));
	client_addr.sin_addr.s_addr = inet_addr(argv[2]);

	int len = sizeof(client_addr);
	int ret = connect(fd,(struct sockaddr *) &client_addr, len);

	char buf[128];
	bzero(buf, sizeof(buf));
//	strcpy(buf, "hello");
//	printf("buf = %s\n", buf);

	if(ret == 0)
	{		
		printf("connecting......\n");
		while(1)
		{
			printf("input data:");
			scanf("%s", buf);
			write(fd, buf, strlen(buf));
			bzero(buf, sizeof(buf));

			//read(fd, buf, sizeof(buf));
			//printf("form rose: %s\n", buf);
		}
	}	
	


	return 0;
}
