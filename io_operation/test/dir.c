#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
	DIR *dirp;
	char type_buf[128];
	dirp = opendir(argv[1]);
	if(dirp == NULL)
	{
		perror("opendir fail");
		return -1;
	}
	
	printf("DIR size = %d\n", sizeof(dirp));
	struct dirent *entry = readdir(dirp);
	if(entry == NULL)
	{
		perror("readdir fail\n");
		
		return -1;
	}
	//printf("d_name= %s\n", entry->d_name);
	sprintf(type_buf, "%s%s", "directory->", entry->d_name);
	printf("%s\n", type_buf);
	
	int ret = closedir(dirp);
	if(ret == -1)
	{
		perror("closedir fail");
		return -1;
	}
	
	return 0;
}