#include <stdio.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//消息数据结构
struct message
{
	long mtype;
	int buf[10];
}msg;


#define PATH "/home/msg_work"

int main()
{
	int num;
	long type;
	
	key_t key = ftok(PATH, 20);
	if(key == -1)
	{
		printf("ftok fail\n");
		return -1;
	}
	int msgid = msgget(key, IPC_CREAT |0666);
	if(msgid == -1)
	{
		perror("msgget fail\n");
		return -1;
	}
	printf("服务器搭建成功\n");
	int a, b;
	
	//计算器
	while(1)
	{
		bzero(msg.buf, sizeof(msg.buf));
		
		printf("input two number for working:");
		scanf("%d%d", &a, &b);
		
		msg.buf[0] = a;
		msg.buf[1] = b;
		msg.mtype = getpid();
		
		int ret = msgsnd(msgid, (void *)&msg, 8, 0);
		if(ret == -1)
		{
			perror("send message fail\n");
			continue;
		}
		int ret1 = msgrcv(msgid, (void *)&msg, 4, getpid(), 0);
		if(ret1 == -1)
		{
			perror("recvice message fail\n");
			continue;
		}
		else 
			printf("the result is %d\n", msg.buf[0]);
	}
	
	return 0;
}