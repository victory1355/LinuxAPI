#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void *first_func(void *arg)
{
	/*int ret = pthread_detach(pthread_self());
	if(ret != 0)
	{
		printf("detach fail\n");
		exit(0);
	}*/
	int i;
	//pthread_exit((void *)1);
	for(i= 0; i<5; i++)
	{
		printf("first thread...\n");
		sleep(1);
	}
	
	printf("child thread done\n");
	
	
	//pthread_detach(pthread_self());
	
	pthread_exit((void *)3);
	//return NULL;
	//exit(0);
}
int main()
{
	//return 0;
	int err, i;
	void *retval1,*retval2;
		
	pthread_t tid1, tid2;
	err = pthread_create(&tid1, NULL, (void *)first_func, NULL);
	//pthread_exit((void *)1);
	if(err != 0)
	{
		char *p = strerror(err);
		printf("%s\n", p);
		//perror("creat");
		return -1;
	}
	
	sleep(1);
	//pthread_detach(tid1);
	void *retval;
	int errn = pthread_join(tid1, (void **)&retval);
	printf("retval = %d\n", (int *)retval);          //有问题,存放指针变量的地址的变量就是二级指针
	if(errn != 0)
	{
		
		char *p = strerror(errno);
		printf("p = %s\n", p);
		//printf("join errno = %d,  errn %d\n", errno, errn);
		return -1;
	}
	//printf("ret code = %p\n", (int *)retval1);
	
	for(i=0; i<10; i++)
	{
		printf("main thread......\n");
		sleep(1);
	}
	printf("exit....\n");
	
	return 0;
}






































