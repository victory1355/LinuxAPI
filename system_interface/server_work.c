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
//计算器服务器
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
	
	//计算器
	while(1)
	{
		bzero(msg.buf, sizeof(msg.buf));
		//接受任意类型的消息
		int ret = msgrcv(msgid, (void *)&msg, 8, 0, 0);
		if(ret == -1)
		{
			perror("recvice message fail\n");
			continue;
		}
		//加法运算
		num = msg.buf[1] + msg.buf[2];
		type = msg.mtype;
		bzero(msg.buf, sizeof(msg.buf));
		
		msg.buf[0] = num;
		msg.mtype = type;
		int retm = msgsnd(msgid, (void *)&msg, 4, 0);
		if(retm == -1)
		{
			perror("send message fail\n");
			continue;
		}
		printf("work result is %d\n", msg.buf[0]);
	}
	
	return 0;
}