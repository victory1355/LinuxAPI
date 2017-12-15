#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
	DIR *dirp = NULL;
	char type_buf[512];
	memset(type_buf, 0 , sizeof(type_buf));
	struct dirent *entry;
	//打开目录
	dirp = opendir(argv[1]);
	if(dirp == NULL)
	{
		printf("open dir fail");
		return -1;
	}
	char delete_buf[200];
	memset(delete_buf, 0 ,sizeof(delete_buf));
	//删除目录
	int ret_re = rmdir(argv[1]);
	if(ret_re == -1)
	{
		printf("rmdir fail\n");
		return -1;
	}
	int i;
	for()
	
	
	
	
	
	
	
	
	
	//读取目录信息
	while(entry = readdir(dirp))
	{
		//printf("目录名：%-20s   文件类型：%hhu\n", entry->d_name, entry->d_type);
		if(entry->d_type & DT_DIR)
		{
			
			sprintf(type_buf, "%s%s", "dir->", entry->d_name);
			printf("%s\n", type_buf);
		}
		//else if(entry->d_type & DT_REG)
		//{
		//	sprintf(type_buf, "%s%s", "nor->", entry->d_name);
		//	printf("%s\n", type_buf);
		//}
	}
	
	
	int ret_c = closedir(dirp);
	if(ret == -1)
	{
		printf("close dir fail");
		return -1;
	}
	
	return 0;
}