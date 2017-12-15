#include "thread_pool.h"

void *mytask(void *arg)
{
	int n = (int )arg;
	
	printf("[%u][%s] == >job will be done in %d sec...\n", (unsigned)pthread_self(), __FUNCTION__, n);
	sleep(n);
	printf("[%u][%s] == >job done\n", (unsigned )pthread_self(), __FUNCTION__);
	
	return NULL;
}

void *count_time(void *arg)
{
	int i= 0;
	while(1)
	{
		sleep(1);
		printf("sec:%d\n", ++i);
	}
}



int main()
{
	pthread_t a;
	pthread_create(&a, NULL, count_time, NULL);
	
	thread_pool *pool = malloc(sizeof(thread_pool));
	init_pool(pool, 2);
	
	
	printf("throwing 3 task....\n");
	add_task(pool, mytask, (void *)(rand()%10));
	add_task(pool, mytask, (void *)(rand()%10));
	add_task(pool, mytask, (void *)(rand()%10));
	
	return 0;
}