#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

main()
{	
	int fd;
	fd  = open("bb.txt", O_RDWR | O_CREAT | O_TRUNC |O_APPEND );
	if( fd == -1)
	{
		perror("open fail");
		return -1;
	}
	printf("%d\n", fd);

	close(fd);


	return 0;
}













