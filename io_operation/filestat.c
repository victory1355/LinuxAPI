#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct stat file_info;
	int sta = stat(argv[1], &file_info);
	if(sta == -1)
	{
		perror("获取文件信息失败");
		return -1;
	}
	printf("%s的大小为：%d\n", argv[1], (int)file_info.st_size);
	
	/*int flag;
	flag = file_info.st_mode & S_IFREG;
	if(flag == S_IFREG)	
	{
		printf("%s 是一个普通文件\n", argv[1]);
	}*/
	//判断文件的类型
	if(S_ISREG(file_info.st_mode))
		printf("%s 是一个普通文件\n", argv[1]);
	if(S_ISLNK(file_info.st_mode))
		printf("%s 是一个符号链接文件\n", argv[1]);
	if(S_ISDIR(file_info.st_mode))
		printf("%s 是一个目录\n", argv[1]);
	if(S_ISCHR(file_info.st_mode))
		printf("%s 是一个字符设备文件\n", argv[1]);
	if(S_ISSOCK(file_info.st_mode))
		printf("%s 是一个套接字文件\n", argv[1]);
	
	//文件的块大小
	printf("文件的块大小 = %lu, 块数 = %lu\n", file_info.st_blksize, file_info.st_blocks);
	

	
	
	
	
	
	
	
	return 0;
}
