#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	
	int fd_src, fd_dest;
	char buf[1024];

	fd_src = open(argv[1], 	O_EXCL| O_RDWR);
	if(fd_src == -1)
	{
		perror("open file failed");
		return -1;
	}

	fd_dest = open(argv[2], O_CREAT|O_RDWR, 0777);
	if(fd_src == -1)
	{
		perror("open file failed");
		return -1;
	}
	int len;

	while(len = read(fd_src ,buf, 1024 ))
	{
		write(fd_dest, buf, len);	
	}


	close(fd_src);
	close(fd_dest);

	return 0;
}
