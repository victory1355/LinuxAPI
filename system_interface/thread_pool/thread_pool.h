#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <signal.h>

#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#include <dirent.h>
#include <assert.h>

#include "list.h"


//------------------------------------------------------------------
typedef struct directory
{
	char dir_name[300];    //存放目录的绝对路径
	struct list_head list;
}directory;

typedef struct file
{
	char file_name[300];    //存放文件的绝对路径
	struct list_head list;
}file;

//全局变量的声明
file *file_head;
directory *directory_head;


//纪录原路径的长度
unsigned int lenght;

//根目录标志
int flag_root;

//进程消耗时间
unsigned int count;

//pthread_mutex_t lock_protect;

//统计目录数
int directory_count;
//统计文件数
int file_count;

//源路径的文件数
int real_file_count;
//源路径的目录数
int real_directory_count;

#define MODE S_IRWXU | S_IRWXG | S_IRWXO

//普通文件的头节点
file * create_file_head();
//目录的头节点
directory * create_directory_head();
//创建文件信息链表
bool add_file_node(file *head, char *node_info);
//创建目录链表
bool add_directory_node(directory *head, char *node_info);
//销毁链表
bool destory_list(file *file_head, directory *directory_head);
//递归遍历源路径，提取目录位置和文件信息
bool traverse_src(char *src_path);

//线程处理函数，参数为目标路径
void *copy_directory_task(void *arg);
//复制目录
bool copy_directory(directory *head, char *dest_path);
//复制文件
bool copy_file(file *head, char *dest_path);

//递归遍历目标路径，提取目录位置信息
bool traverse_dest(char *src_path);
//打印链表内容
bool show_directory_node(directory *head);
bool show_file_node(file *head);

//复制的任务
void *copy_task(void *arg);

//------------------------------------------------------------------



#define MAX_WAITING_TASKS 1000
//最大线程数
#define MAX_ACTIVE_THREADS 300

typedef struct thread_pool
{
	pthread_mutex_t lock;
	pthread_cond_t cond;
	struct task *task_list;
	
	pthread_t *tids;
	unsigned waiting_tasks;
	unsigned active_threads;
	
	bool shutdown;
}thread_pool;

struct task
{
	void *(*task)(void *arg);  //自定义函数列表
	void *arg;
	
	struct task *next;
};

bool init_pool(thread_pool *pool, unsigned int threads_number);

bool add_task(thread_pool *pool, void *(*task)(void *arg), void *arg);

int add_thread(thread_pool *pool, unsigned int additional_threads_number);

int remove_thread(thread_pool *pool, unsigned int removing_threads_number);

bool destroy_pool(thread_pool *pool);

void *routine(void *arg);
//设置为全局变量
thread_pool *pool;

#endif
