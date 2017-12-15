#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#define MAX_PATH 512 

void print_file_info(char *pathname)  
{  
	struct stat filestat;  
	if (stat(pathname, &filestat) == -1)  
	{  
		printf("cannot access the file %s", pathname);  
		return;  
	} 
 
	printf("%s %8ld\n", pathname, filestat.st_size);  
	
	if ((filestat.st_mode & S_IFMT) == S_IFDIR)  
	{  
		dir_order(pathname);  
	}  
}  

void dir_order(char *pathname)  
{  
	DIR *dfd;  
	char name[MAX_PATH];  
	struct dirent *dp;  
	if ((dfd = opendir(pathname)) == NULL)  
	{  
		printf("dir_order: can't open %s\n %s", pathname,strerror(errno));  
		return;  
	}  
	while ((dp = readdir(dfd)) != NULL)  
	{  
		//过滤当前目录和上一级的目录信息
		if (strncmp(dp->d_name, ".", 1) == 0)  
			continue;  
		if (strlen(pathname) + strlen(dp->d_name) + 2 > sizeof(name))  
		{  
			printf("dir_order: name %s %s too long\n", pathname, dp->d_name);  
		} else  
		{  
			memset(name, 0, sizeof(name));  
			sprintf(name, "%s/%s", pathname, dp->d_name); 
			print_file_info(name);  
		}  
	}  
	closedir(dfd);  

}  

int main(int argc, char *argv[])  
{  
	if (argc == 1)  
	{  
		dir_order(".");  
	} else  
	{  
		dir_order(argv[1]);  
	}  
	return 0;  
}  







