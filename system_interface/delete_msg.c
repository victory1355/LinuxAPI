#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define PATH "/home/msg"


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
	int ret = msgctl(msgid, IPC_RMID, NULL);
	if(ret == -1)
	{
		perror("delete msg fail\n");
		return -1;
	}
	else
		printf("delete msg ok\n");
	
	
	return 0;
}






























