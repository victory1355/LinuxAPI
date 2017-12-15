#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define PATH "/home/msg"

//自定义消息类型
struct msg
{
	int size;
	char buf[20];
}my_msg;

int main()
{
	int cnt;
	int key = ftok(PATH, 45);
	if(key == -1)
	{
		perror("ftok fail\n");
		return -1;
	}
	int msgid = msgget(key, IPC_CREAT | 0666);
	if(msgid == -1)
	{
		perror("msgget fail\n");
		return -1;
	}
	else 
	{
		printf("get msg ok\n");
	}
	printf("welcome to the super chat\n");
	while(1)
	{
		int cnt = msgrcv(msgid, (void *)&my_msg, sizeof(my_msg.buf), 0, 0);
		printf("here\n");
		if(cnt == -1)
		{
			perror("msgrcv fail\n");
			return -1;
		}
		else
			printf("收到%s\n", my_msg.buf);
	}

	
	return 0;
}






























