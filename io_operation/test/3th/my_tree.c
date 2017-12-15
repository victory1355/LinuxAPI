#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#define MAX_PATH 512 



void dir_order(char *pathname)  
{  
	DIR *dfd;  
	char name[MAX_PATH]; 
	struct stat filestat;  	
	struct dirent *dp;  
	
	if ((dfd = opendir(pathname)) == NULL)  
	{  
		printf("dir_order: can't open %s\n %s", pathname,strerror(errno));  
		return;  
	} 

	
	while ((dp = readdir(dfd)) != NULL)  
	{  
			memset(name, 0, sizeof(name));  
			sprintf(name, "%s/%s", pathname, dp->d_name);
			
			printf("文件：%s\n", name);
			
			stat(name, &filestat);
			//if(S_ISDIR(filestat.st_mode))
			if ((filestat.st_mode & S_IFMT) == S_IFDIR) 
			{
					dir_order(name);
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







