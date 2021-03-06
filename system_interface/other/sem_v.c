
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

#define PATH "/home/sem_key"

int main(int argc ,char *argv[])
{
	struct sembuf mysbuf;
	int rets1;
	int rets2;
	//共享内存
	key_t key_m = ftok("/home/shm_key", 20);
	printf("key_m = %d\n", key_m);
	if(key_m == -1)
	{
		perror("ftok fail\n");
		return -1;
	}
	int shmid = shmget(key_m, 512, IPC_CREAT | 0777);
	if(shmid == -1)
	{
		perror("shmget fail\n");
		return -1;
	}
	printf("get shm ok\n");
	
	char *p = shmat(shmid, NULL, 0);
	if(p == NULL)
	{
		perror("shmat fail\n");
		return -1;
	}
	//信号量
	key_t key = ftok(PATH, 20);
	if(key == -1)
	{
		perror("ftok fail\n");
		return -1;
	}
	printf("sem key = %d\n", key);
	//获取信号量
	int semid = semget(key, 2, 0666);
	if(semid == -1)
	{
		perror("semget fail\n");
		return -1;
	}
	printf("get sem ok\n");
	
	union semun mysun;
	mysun.val = 0;
	//初始化信号量
	int ret1 = semctl(semid, 0, SETVAL, mysun);
	if(ret1 == -1)
	{
		perror("semctl fail\n");
		return -1;
	}
	int ret2 = semctl(semid, 1, SETVAL, mysun);
	if(ret2 == -1)
	{
		perror("semctl fail\n");
		return -1;
	}
	
	//文件IO
	FILE *fp = fopen("./b.txt", "w+");
	if(fp == NULL)
	{
		perror("fopen fail\n");
		return -1;
	}
	while(1)
	{		
		mysbuf.sem_num = 0;         //信号量编号
		mysbuf.sem_flg = 0;         //阻塞操作
		mysbuf.sem_op = -1;        
		rets1 = semop(semid, &mysbuf, 1);
		if(rets1 == -1)
		{
			perror("semop fail\n");
			return -1;
		}
		
		fwrite(p, sizeof(char), sizeof(p), fp);
		//v操作
		mysbuf.sem_num = 1;         //信号量编号
		mysbuf.sem_flg = 0;         //阻塞操作
		mysbuf.sem_op =  1;        
		rets2 = semop(semid, &mysbuf, 1);
		if(rets2 == -1)
		{
			perror("semop fail\n");
			return -1;
		}
		
	}
	int retde = semctl(semid,200222, IPC_RMID);
	if(retde == -1)
	{
		perror("semctl fail\n");
		return -1;
	}
	else
		printf("delete the sem ok\n");
	
	return 0;
}


























