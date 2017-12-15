#include <stdio.h>
#include <pthread.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>


void *start(void *arg)
{
	printf("hello world\n");
	sleep(3);
	while(1);
}


int main()
{
	int i = 1;
	pthread_t tid[1000] = {1};
	bzero(tid, sizeof(tid));
	int count = 0;
	
	while(1)
	{	
		int ret =pthread_create(&tid[i], NULL, start, NULL);
	    if(ret != 0)
		{
			perror("create fail\n");
			break;
		}
		i++;
		count++;
	}
	printf("count = %d\n", count);
	
	
	return 0;
}