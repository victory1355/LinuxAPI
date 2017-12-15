#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define PATH "/home/msg1"


struct message
{
	long mtype;
	char buf[50];
}msg;

int main()
{
	int ret;
	int retr;
	//char rev_buf[200];
	
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
	
	//狗
	printf("hi ,l am a simgle dog\n");
	//消息类型
	
	while(1)
	{
		//接受消息
		retr = msgrcv(msgid, (void *)&msg, sizeof(msg.buf), 100, 0);
		if(retr == -1)
		{
			printf("recv fail\n");
			continue;
		}
		printf("cat:%s\n", msg.buf);
		bzero(msg.buf, sizeof(msg.buf));
		printf("---------------------------------\n");
		
		printf("input message :");
		scanf("%s", msg.buf);
		if(!strcmp(msg.buf, "quit"))
		{
			break;
		}
		msg.mtype = 200;
		//发送消息
		ret = msgsnd(msgid, (void *)&msg, strlen(msg.buf), 0);
		if(ret == -1)
		{
			perror("msgsnd fail\n");
			return -1;
		}
		else
			printf("send message ok\n");
		
		bzero(msg.buf, sizeof(msg.buf));
		printf("---------------------------------\n");
	}	
	printf("welcome back\n");

	return -1;
}

























