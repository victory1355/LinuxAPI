#include "thread_pool.h"

//计时
void *count_time(void *arg)
{
	//int i= 0;
	while(1)
	{
		//毫秒级
		usleep(1000);
		//printf("ms <-----> %d\n", ++count);
		++count;
	}
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Usage:./xxx.elf src_path  dest_path\n");
		return -1;
	}
	
	//初始化进程时间
	count = 0;
	
	 //复制后的实际数量
	directory_count = 0;
	file_count = 0;
	
	//源路径的文件数
	real_file_count = 0;
	//源路径的目录数
	real_directory_count = 0;
	
	pthread_t time;
	//计时线程
	pthread_create(&time, NULL, count_time, NULL);
	
	pool = malloc(sizeof(thread_pool));
	//初始化线程池
	
	assert(pool);
	
	pthread_mutex_init(&(pool->lock), NULL);
	init_pool(pool, 20);
	
	//------------------------------------------------
	directory_head = create_directory_head();
	if(directory_head == NULL)
	{
		return -1;
	}
	file_head = create_file_head();
	if(file_head == NULL)
	{
		return -1;
	}
	
	//printf("[%d]\n", __LINE__);
	
	if(argc != 3)
	{
		printf("usage: ./xxx.elf src_path dest_path\n");
		return -1;
	}
	
	//printf("[%d]\n", __LINE__);
	
	lenght = strlen(argv[1]);
	//判断是否为根目录
	if( !strcasecmp(argv[1], "/"))
	{
		flag_root = 1;
	}
	//遍历源路径
	
	//printf("[%d]\n", __LINE__);
	
	if(traverse_src(argv[1]))
	{
		printf("traverse successful\n");
	}
	else
		printf("traverse fail\n");
	
	//printf("[%d]\n", __LINE__);
	
	
	printf("-------------------------------\n");
	
	copy_directory(directory_head, argv[2]);
	//复制文件
	printf("--------------------------------------------------------------\n");
	copy_file(file_head, argv[2]);
	
	//打印时间
	printf("\n---------------------------------------\n");
	printf("total consumed %d ms\n" , count);
	printf("---------------------------------------\n");
	
	//打印当前的任务数和活跃线程数
	printf("current task number :%d\n", pool->waiting_tasks);
	printf("current thread number :%d\n", remove_thread(pool, 0));
	
	//打印遍历的文件数和目录数
	printf("\n----------------------------------------------\n");
	printf("there are total %d files and %d directorys\n", real_file_count, real_directory_count);
	printf("------------------------------------------------\n");
	
	//打印复制后的文件数和目录数
	printf("\n----------------------------------------\n");
	printf("total %d files , %d directorys\n", (int)file_count, (int)directory_count);
	printf("----------------------------------------\n");

	//销毁链表
    if(destory_list(file_head, directory_head))
	{
		printf("two list has been destroied\n");
	}
	
	//销毁线程池
	if(destroy_pool(pool))
	{
		printf("the pool has been destroied\n");
		pthread_cancel(time);
	}
	printf("the count time threads exit\n");
	
	int ret = pthread_join(time, NULL);
	if(ret != 0)
	{
		printf("join fail\n");
		
	}
	printf("exit....\n");
	
	return 0;
}






























