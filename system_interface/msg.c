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
	printf("输入你要发送的消息\n");
	fgets(my_msg.buf, sizeof(my_msg.buf), stdin);
	my_msg.size = strlen(my_msg.buf);
	
	int ret = msgsnd(msgid, (void *)&my_msg, strlen(my_msg.buf), 0);
	if(ret == -1)
	{
		perror("msg send fail\n");
		return -1;
	}
	else
		printf("消息发送成功\n");
	
	
	
	
	
	
	
	return 0;
}