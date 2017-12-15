#include "thread_pool.h"

void handler(void *arg)
{
	pthread_mutex_unlock((pthread_mutex_t *)arg);
}

void *routine(void *arg)
{
	thread_pool *pool = (thread_pool *)arg;
	struct task *p;
	
	while(1)
	{
		pthread_cleanup_push(handler, (void *)&pool->lock);
		
		pthread_mutex_lock(&pool->lock);
		
		while(pool->waiting_tasks == 0 && !pool->shutdown)
		{
			pthread_cond_wait(&pool->cond, &pool->lock);
		}
		
		if(pool->waiting_tasks == 0 && pool->shutdown == true)
		{
			pthread_mutex_unlock(&pool->lock);
			pthread_exit(NULL);
		}
		
		p = pool->task_list->next;
		pool->task_list->next = p->next;
		pool->waiting_tasks--;
		
		pthread_mutex_unlock(&pool->lock);
		
		pthread_cleanup_pop(0);
		
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
		(p->task)(p->arg);
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		
		free(p);
	}
	
	pthread_exit(NULL);
	
}

bool init_pool(thread_pool *pool, unsigned int threads_number)
{
	pthread_mutex_init(&pool->lock, NULL);
	pthread_cond_init(&pool->cond, NULL);
	
	pool->shutdown = false;
	//任务头节点
	pool->task_list = malloc(sizeof(struct task));
	pool->tids = malloc(sizeof(pthread_t) * MAX_ACTIVE_THREADS);
	
	if(pool->task_list == NULL || pool->tids == NULL)
	{
		perror("allocate memory error");
		return false;
	}
	
	pool->task_list->next = NULL;
	
	pool->waiting_tasks = 0;
	pool->active_threads = threads_number;
	
	int i;
	for(i=0;i<pool->active_threads; i++)
	{
		if(pthread_create(&((pool->tids)[i]), NULL, routine, (void *)pool) != 0)
		{
			perror("create threads error");
			return false;
		}
	}
	
	return true;
}

bool add_task(thread_pool *pool, void *(*task)(void *arg), void *arg)
{
	struct task *new_task = malloc(sizeof(struct task));
	if(new_task == NULL)
	{
		perror("allocate memory error");
		return  false;
	}
	
	new_task->task = task;
	new_task->arg = arg;
	new_task->next = NULL;
	
	pthread_mutex_lock(&pool->lock);
	if(pool->waiting_tasks >= MAX_WAITING_TASKS)
	{
		pthread_mutex_unlock(&pool->lock);
		
		fprintf(stderr, "too many task.\n");
		free(new_task);
		
		return false;
	}
	
	struct task *tmp = pool->task_list;
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	//尾插法添加任务
	tmp->next = new_task;
	pool->waiting_tasks++;

	pthread_mutex_unlock(&pool->lock);
	pthread_cond_signal(&pool->cond);
	
	return true;
}

int add_thread(thread_pool *pool, unsigned int additional_threads)
{
	if(additional_threads == 0)
	{
		return  0;
	}
	unsigned int total_threads = pool->active_threads + additional_threads;
	
	int i, actual_increment = 0;
	for(i=pool->active_threads; i<total_threads && i<MAX_ACTIVE_THREADS; i++)
	{
		if(pthread_create(&((pool->tids)[i]), NULL, routine, (void *)pool) != 0)
		{
			perror("add threads error");
			if(actual_increment == 0)
			{
				return -1;
			}
			break;
		}
		actual_increment++;
	}
	pool->active_threads += actual_increment;
	
	return actual_increment;
}

int remove_thread(thread_pool *pool, unsigned int removing_threads)
{
	if(removing_threads == 0)
	{
		return pool->active_threads;
	}
	
	int remain_threads = pool->active_threads - removing_threads;
	remain_threads = remain_threads >0 ? remain_threads : 1;
	
	int i;
	for(i=pool->active_threads-1; i>remain_threads-1; i--)
	{
		errno = pthread_cancel(pool->tids[i]);
		if(errno != 0)
			break;
	}
	if(i == pool->active_threads-1)
		return -1;
	else
	{
		pool->active_threads = i+1;
		return i+1;
	}
}

bool destroy_pool(thread_pool *pool)
{
	if(pool == NULL)
		return false;
	pool->shutdown = true;
	pthread_cond_broadcast(&pool->cond);
	
	int i;
	//printf("active_threads = %d\n", pool->active_threads);
	for(i=0; i<pool->active_threads; i++)
	{
		if(pool != NULL)
		{
			//printf("i=%d,tid = %u\n",i,(unsigned)pool->tids[i]);
			if(pool->tids != NULL)
			{
				errno = pthread_join(pool->tids[i], NULL);
				if(errno != 0)
				{
					printf("join tids[%d] errrno:%s\n", i, strerror(errno));
				}
				else
					printf("[%u] is joined\n", (unsigned)pool->tids[i]);
			}	
		}
	}
	//printf("active_threads = %d\n", pool->active_threads);	
	free(pool->task_list);
	free(pool->tids);
	free(pool);
	
	return true;
}








































