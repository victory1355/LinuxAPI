#include "thread_pool.h"

//普通文件的头节点
file * create_file_head()
{
	file *p = malloc(sizeof(file));
	if(p == NULL)
	{
		perror("allocate memory fail\n");
		return NULL;
	}
	INIT_LIST_HEAD(&p->list);
	
	return p;
}
//目录的头节点
directory * create_directory_head()
{
	directory *p = malloc(sizeof(directory));
	if(p == NULL)
	{
		perror("allocate memory fail\n");
		return NULL;
	}
	INIT_LIST_HEAD(&p->list);
	
	return p;
}

//创建文件信息链表
bool add_file_node(file *head, char *node_info)
{
	if(head == NULL)
		return false;
	if(node_info == NULL)
		return false;
	
	file *node = malloc(sizeof(file));
	if(node == NULL)
	{
		perror("allocate memory error\n");
		return false;
	}
	INIT_LIST_HEAD(&node->list);
	strcpy(node->file_name, node_info);
	
	list_add_tail(&node->list, &head->list);
	
	return true;
}
//创建目录链表
bool add_directory_node(directory *head, char *node_info)
{
	if(head == NULL)
		return false;
	if(node_info == NULL)
		return false;
	
	directory * node = malloc(sizeof(directory));
	if(node == NULL)
	{
		perror("allocate memory error\n");
		return false;
	}
	INIT_LIST_HEAD(&node->list);
	strcpy(node->dir_name, node_info);
	
	list_add_tail(&node->list, &head->list);
	
	return true;
}
//销毁链表
bool destory_list(file *file_head, directory *directory_head)
{
	if(file_head == NULL)
		return false;
	if(directory_head == NULL)
		return false;
	
	struct list_head *pos1 = directory_head->list.next, *deld = NULL;
	//销毁文件链表
	while(pos1 != &directory_head->list)
	{
		deld = pos1;
		pos1 = pos1->next;
		list_del_init(deld);
		//free(deld);
	}
	free(directory_head);
	
	//销毁目录链表
	struct list_head *pos2 = file_head->list.next, *delf = NULL;
	while(pos2 != &file_head->list)
	{
		delf = pos2;
		pos2 = pos2->next;
		list_del_init(delf);
		//free(delf);
	}
	free(file_head);
	
	return true;
}


/*
//线程处理函数，参数为目标路径
void *copy_directory_task(void *arg)
{
	char *dest_path = (char *)arg;
	//打开目标路径，如果不存在则创建
	DIR *dirp = opendir(dest_path);
	if(dirp == NULL)
	{
		int ret = mkdir(dest_path, S_IRWXU);
		if(ret == -1)
		{
			perror("mkdir fail\n");
			return (void *)1;
		}
	}
	else
		perror("the directory %s has existed\n", dest_path);
	//复制目录,遍历目录链表
	copy_directory(directory_head, dest_path);
	return (void *)0;
}*/


//复制的任务
void *copy_task(void *arg)
{
	if(arg == NULL)
		return (void *)1;
	
	char (*copy)[300] = (char (*)[300])arg;
	//printf("src = %s\n", (char *)copy);
	//printf("des = %s\n", (char *)(copy+1));
	
	char *buf = malloc(sizeof(char) * 128);
	bzero(buf, sizeof(buf));
	
	
	//打开原来的文件
	printf("old file --> %s\n", (char *)copy);
	FILE * fp1 = fopen((char *)copy, "r+");
	if(fp1 == NULL)
	{
		perror("fopen src file fail\n");
		//while(1);
		return false;
	}
	//创建新的文件
	printf("new file --> %s\n", (char *)(copy+1));
	FILE * fp2 = fopen((char *)(copy+1), "w+");
	
	if(fp2 == NULL)
	{
		perror("fopen dest file fail\n");
		//while(1);
		return false;
	}	
	while( fread(buf, sizeof(char), sizeof(buf), fp1) )
	{
		//fscanf(fp1, "%s", buf);
		//fprintf(fp2, "%s", buf);
		fwrite(buf, sizeof(char), strlen(buf), fp2);
		fflush(fp2);
		bzero(buf, sizeof(buf));
	}
	//成功复制的文件
	file_count++;
	
	
	fclose(fp1);
	fclose(fp2);
	
	return (void *)0;
}

//复制目录
bool copy_directory(directory *head, char *dest_path)
{
	//复制目录,遍历目录链表
	if(head == NULL)
	{
		return false;
	}
	if(dest_path == NULL)
		return false;
	
	char path1[300];
	bzero(path1, sizeof(path1));

	struct list_head *pos = NULL;
	directory *dirp = NULL;
	int ret;
	char *p1;
	int  i;
	
	//char *dest_path = (char *)arg;
	
	//打开目标路径，如果不存在则创建
	DIR *dirp1 = opendir(dest_path);
	if(dirp1 == NULL)
	{
		int ret = mkdir(dest_path, MODE);
		if(ret == -1)
		{
			perror("mkdir fail\n");
			return (void *)1;
		}
	}
	else
		printf("the directory %s has existed\n", dest_path);
	
	//复制目录,遍历目录链表
	list_for_each(pos, &head->list)
	{
		dirp = list_entry(pos, directory, list);
		
		p1 = dirp->dir_name;
		for(i = 0; i<=lenght; i++)
		{
			p1++;
		}
		//printf("%s\n", p1);
		//如果是根目录
		if(flag_root == 1)
		{
			sprintf(path1, "%s/%s", dest_path, dirp->dir_name+1);
			ret = mkdir(path1, MODE);
			if(ret == -1)
			{
				perror("mkdir error");
				//提示用户
				//while(1);
				//return false;
				continue;
			}
			printf("MKDIR ---> %s\n", path1);
			
		}
		//如果是其他目录
		else
		{
			sprintf(path1, "%s/%s", dest_path, p1);
			ret = mkdir(path1, MODE);
			if(ret == -1)
			{
				perror("mkdir error");
				//提示用户
				//return false;
				//while(1);
				continue;
			}
			printf("MKDIR ---> %s\n", path1);
			//bzero(path1, sizeof(path1));
		}
		bzero(path1, sizeof(path1));
		
		//成功复制的目录数
		directory_count++;
	}
	return true;
}

//复制文件
bool copy_file(file *head, char *dest_path)
{
	if(head == NULL)
		return false;
	//复制文件,遍历文件链表
	if(dest_path == NULL)
		return false;
	
	char path[300];
	bzero(path, sizeof(path));
	
	struct list_head *pos;
	file *filep;
	char *p1 = NULL;
	int i;
	
	char arg[2][300];
	bzero(arg, sizeof(arg));
	
	int ret ;
	int reta;
	int retr;
	
	list_for_each(pos, &head->list)
	{
		filep = list_entry(pos, file, list);
		//创建新的的目录
		p1 = filep->file_name;
		for(i = 0; i<=lenght; i++)
		{
			p1++;
		}
		
		//判断是否为根目录
		if(flag_root == 1)
		{
			sprintf(path, "%s/%s", dest_path, filep->file_name+1);
		}
		else
		{
			sprintf(path, "%s/%s", dest_path, p1);
		}
		
		strcpy(arg[0], filep->file_name);
		strcpy(arg[1], path);
	
		//添加任务
		add_task(pool, copy_task, arg);
		//任务添加的频率
		usleep(50000);
		
		ret = remove_thread(pool, 0);
		//如果任务太多,则添加线程
		if(ret > MAX_WAITING_TASKS)
		{
			//添加20个线程
			reta = add_thread(pool, 20);
			if(reta != 0)
			{
				printf("\n add %d additional threads \n", reta);
			}
			else
				printf("\n add additional threads fail\n");
			
			//如果任务太多降低频率
			usleep(3000000);
		}
		//如果任务太少减少线程
		if(pool->waiting_tasks < 50 && pool->active_threads >50)
		{
			printf("task number < 50\n");
			retr = remove_thread(pool, 20);
			if(retr != -1)
			{
				printf("\nthere are %d threads\n", retr);
			}
			else
				printf("remove threads fail\n");
		}
		bzero(path, sizeof(path));
		bzero(arg ,sizeof(arg));
		
	}
}

//递归遍历源路径，提取目录位置和文件信息
bool traverse_src(char *src_path)
{
	if(src_path == NULL)
		return false;
	DIR *dirp = NULL;
	
	dirp = opendir(src_path);
	
	if(dirp == NULL)
	{
		perror("src directory not exist\n");
		
		return false;
	}
	else
		printf("OPEN  -->  %-25s\n", src_path);
	struct dirent * entry;
	
	char path_buf[300];
	bzero(path_buf, sizeof(path_buf));
	char ch, *p = NULL;
	
	char file_buf[300];
	bzero(file_buf, sizeof(file_buf));
	char ch1, *p1 = NULL;

	while(entry = readdir(dirp))
	{
		
		if(entry->d_type == DT_DIR)
		{
			if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
				continue;
			
			//继续递归调用,源路径的末尾不可以有'/'
			//if( strcasecmp(src_path, ))
			if( !strcasecmp(src_path, "/") || !strcasecmp(src_path, "./"))
				sprintf(path_buf, "%s%s", src_path, entry->d_name);
			else
				sprintf(path_buf, "%s/%s", src_path, entry->d_name);
			
			printf("DIR   -->  %-25s\n", path_buf);
			
			//统计目录数
			++real_directory_count;
			//创建目录链表
			add_directory_node(directory_head, path_buf);
			
			traverse_src(path_buf);
				
		}
		if(entry->d_type == DT_REG)
		{ 
			
			ch1 = '/';
			p1 = strrchr(src_path, ch1);
			int flag1 = strcasecmp(p1, "/");
			if(flag1 ==0)
			{
				sprintf(file_buf, "%s%s", src_path, entry->d_name); 
			}
			else 
				sprintf(file_buf, "%s/%s", src_path, entry->d_name);
			
			printf("REG   -->  %-25s\n", file_buf);
			
			//统计文件数
			
			//pthread_mutex_lock(&lock_protect);
			++real_file_count;
			//pthread_mutex_unlock(&lock_protect);
			
			//创建文件信息链表
			add_file_node(file_head, file_buf);
			
		}
		
	}
	closedir(dirp);
	
	return true;
}


//打印链表内容
bool show_directory_node(directory *head)
{
	if(head == NULL)
	{
		return false;
	}
	struct list_head *pos;
	directory *dirp;
	
	list_for_each(pos, &head->list)
	{
		dirp = list_entry(pos, directory, list);
		printf("path ==> %s\n", dirp->dir_name);
	}
	return true;
}
bool show_file_node(file *head)
{
	if(head == NULL)
	{
		return false;
	}
	struct list_head *pos;
	file *dirp;
	
	list_for_each(pos, &head->list)
	{
		dirp = list_entry(pos, file, list);
		printf("file ===> %s\n", dirp->file_name);
	}
	return true;
}


































