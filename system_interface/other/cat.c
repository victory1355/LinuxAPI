#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define PATH "/home/msg1"


struct message
{
	long mtype;
	char buf[50];
}read_msg;

int main()
{
	int ret;
	int retr;
	key_t key = ftok(PATH, 78);
	if(key == -1)
	{
		printf("ftok fail\n");
		return -1;
	}
	int msgid = msgget(key, IPC_CREAT |0666);
	if(msgid == -1)
	{
		printf("msgget fail\n");
		return -1;
	}
	printf("msg get ok\n");
	printf("-----------annimal chat room---------------\n");
	
	//猫
	printf("hi ,l am a simgle cat\n");
	//消息类型
	
	while(1)
	{
		printf("input message:");
		scanf("%s", read_msg.buf);
		if(!strcmp(read_msg.buf, "quit"))
		{
			break;
		}
		read_msg.mtype = 100;
		//发送消息
		ret = msgsnd(msgid, (void *)&read_msg, strlen(read_msg.buf), 0);
		if(ret == -1)
		{
			perror("msgsnd fail\n");
			return -1;
		}
		else 
			printf("send message ok\n");
		printf("---------------------------------\n");
		bzero(read_msg.buf, sizeof(read_msg.buf));
		//接受消息
		retr = msgrcv(msgid, (void *)&read_msg, sizeof(read_msg.buf), 200, 0);
		if(retr == -1)
		{
			printf("recv fail\n");
			continue;
		}
		printf("dog:%s\n", read_msg.buf);
		bzero(read_msg.buf, sizeof(read_msg.buf));
		printf("---------------------------------\n");
	}
	
	
	printf("welcome back\n");

	return -1;
}
