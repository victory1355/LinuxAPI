#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "kernel_list.h"

typedef struct 
{
	unsigned short ID;
	int connfd;

	struct list_head list;
}clinode, *clilist;

void usage(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s <PORT>\n", argv[0]);
		exit(0);
	}
}

clilist head ;

//初始化链表
clilist init_list();
//创建新的用户节点
clilist newclient(int connfd, unsigned short port);

//打印链表内容
void show_list(clilist head1);
//私聊
void private_talk(unsigned short ID, char *msg);
//群聊
void broadcast(unsigned short ID, char *msg);

//子进程
void process_connfd(unsigned short ID, int connfd);

int main(int argc, char **argv) // ./Server 50001
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
	srvaddr.sin_port = htons(atoi(argv[1]));
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 2, 绑定地址（IP + PORT）
	if( !bind(fd, (struct sockaddr *)&srvaddr, len) )
		printf("bind ok\n");
	else
	{
		perror("bind:");
		return -1;
	}

	// 3, 设置监听状态
	if( !listen(fd, 3) )
		printf("listen ok\n");
	else
	{
		perror("listen:");
		return -1;
	}

	struct sockaddr_in cliaddr;
	len = sizeof(cliaddr);
	bzero(&cliaddr, len);

	// 4, 创建一个空的用户链表
	head = init_list();
	if(head != NULL)
		printf("creat head ok\n");

	// 5, 坐等客户端的连接请求
	int statloc;
	while(1)
	{
		
		int connfd = accept(fd, (struct sockaddr *)&cliaddr, &len);
		if(connfd == -1)
		{
			perror("accept fail:");
			return -1;
		}
		else
			printf("accept ok\n");
		
		printf("Welcome: %s:%hu\n", inet_ntoa(cliaddr.sin_addr),
					    ntohs(cliaddr.sin_port));

		// 更新当前用户列表
		clilist new = newclient(connfd, ntohs(cliaddr.sin_port));
		list_add_tail(&new->list, &head->list);
		
		show_list(head);
		
		//创建一个专门处理这个客户端的进程
		process_connfd(new->ID, connfd);
		printf("[%d]\n", __LINE__);
		
		//非阻塞轮询检测进程退出状态
		waitpid(-1, &statloc,  WNOHANG);
	}

	return 0;
}

clilist init_list()
{
	head = malloc(sizeof(clinode));
	if(head == NULL)
		return NULL;
	bzero(head, sizeof(clinode));
	
	INIT_LIST_HEAD(&head->list);
	
	
	return head;
}
clilist newclient(int connfd, unsigned short port)
{
	clilist new = malloc(sizeof(clinode));
	if(new == NULL)
		return NULL;
	
	INIT_LIST_HEAD(&new->list);
	new->ID = port;
	new->connfd = connfd;     //没有赋值
	
	return new;
}

void show_list(clilist head1)
{
	struct list_head *pos;
	clilist node;
	
	//遍历用户链表
	list_for_each(pos, &head1->list)
	{
		node = list_entry(pos, clinode, list);
		printf("[%d]:%hu\n", __LINE__, node->ID);
	}
	
	return ;
}

//子进程
void process_connfd(unsigned short ID, int connfd)
{

	printf("[%d] ID =%hu, connfd = %d\n", __LINE__, ID, connfd);
	pid_t pid = fork();
	if(pid == -1)
	{
		perror("fork fail");
		return ;
	}
	char buf[128];	
	int len;
	
	if(pid == 0)
	{
		while(1)
		{
			bzero(buf, sizeof(buf));
			printf("connfd = %d\n", connfd);
			
			len = read(connfd, buf, sizeof(buf));
			if(len == -1)
			{
				perror("read fail");
					break;
			}
			else
				printf("msg:%s", buf);
			
			if(len == 0)
				break;
			char *p = NULL;
			
			if(len > 0)
			{
				if( p = strstr(buf, ":"))
				{
					//私聊
					private_talk(atoi(buf), p+1);
				}
				else
				{
					//群聊
					broadcast(ID, buf);
				}
			}
		}
	}
	//关闭文件描述符
	close(connfd);
	
	//退出进程
	exit(0);
}

//私聊
void private_talk(unsigned short ID, char *msg)
{
	struct list_head *pos;
	clilist node;
	//遍历用户链表
	list_for_each(pos, &head->list)     //向前遍历时注意第二个参数
	{
		//printf("[%d]\n", __LINE__);
		node = list_entry(pos, clinode, list);
		//发送消息给ID
		if(node->ID == ID)
		{
			if( write(node->connfd, msg, strlen(msg)) == -1)
			{
				perror("write fail");
				break;
			}
			else
				printf("server sended %hu msg:%s\n", node->ID, msg);
		}

	}
	printf("[%d]\n", __LINE__);
}

//群聊
void broadcast(unsigned short ID, char *msg)
{
	struct list_head *pos;
	clilist node;
	
	//遍历用户链表
	list_for_each(pos, &head->list)     //向前遍历时注意第二个参数
	{
	//	printf("[%d]\n", __LINE__);
		node = list_entry(pos, clinode, list);
		//群发消息
		if(node->ID == ID)
			continue;
		
		if( write(node->connfd, msg, strlen(msg)) == -1)
		{
			//printf("[%d]\n", __LINE__);
			perror("write fail");
			break;
		}
		else
			printf("server msg to everybody:%s\n", msg);
	
	}
	printf("[%d]\n", __LINE__);
}


























