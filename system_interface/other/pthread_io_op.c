#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>

void *read_func(void *arg)
{
	char buf[128];
	bzero(buf, sizeof(buf));
	
	FILE *fp = fopen("./read.txt", "w+");
	if(fp == NULL)
	{
		printf("fopen fail\n");
		pthread_exit((void *)1);
	}
	while(1)
	{
		scanf("%s", buf);
		fwrite(buf, sizeof(char), strlen(buf), fp);
		fflush(fp);
		if(!strncasecmp(buf, "quit", 4))
			break;
		//sleep(1);
		bzero(buf, sizeof(buf));
	}
	
	printf("read_func exit\n");
	pthread_exit((void *)0);
}

void *write_func(void *arg)
{
	char buf[128];
	bzero(buf, sizeof(buf));
	
	FILE * fp1 = fopen("./read.txt", "r+");
	if(fp1 == NULL)
	{
		printf("fopen fail\n");
		pthread_exit((void *)1);
	}
	FILE *fp2 = fopen("./log.txt", "w+");
	if(fp2 == NULL)
	{
		printf("fopen fail\n");
		pthread_exit((void *)1);
	}
	
	while(1)
	{
		fread(buf, sizeof(char), sizeof(buf), fp1);
		if(strlen(buf))
		{
			fwrite(buf, sizeof(char), strlen(buf), fp2);
			fflush(fp2);
			if(!strncasecmp(buf, "quit", 4))
			{
				break;
			}
			bzero(buf, sizeof(buf));
		}
		sleep(1);
		//printf("buf = %s\n", buf);
	}
	
	printf("write_func exit\n");
	pthread_exit((void *)0);
}

int main()
{
	//memset(buf, 0, sizeof(buf));
	pthread_t p_wrtie, p_read;
	int retw = pthread_create(&p_wrtie, NULL, (void *)write_func, NULL);
	if(retw != 0)
	{
		perror("pthread create");
		return -1;
	}
	int retr = pthread_create(&p_read, NULL, (void *)read_func, NULL);
	if(retr != 0)
	{
		perror("pthread create");
		return -1;
	}
	void *ret_w_val, *ret_r_val;
	
	pthread_join(p_wrtie, (void **)&ret_w_val);
	pthread_join(p_read, (void **)&ret_r_val);
	
	printf("main exit...\n");
	
	return 0;
}