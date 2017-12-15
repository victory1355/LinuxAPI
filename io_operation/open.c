#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	int fd, re_ret, wr_ret;
	char wri_buf[] = "l am xiao mo";
	char re_buf[50];
	fd = open("a.txt",O_EXCL, 0777);
	if(fd == -1)
	{
		perror("open fail");
		return -1;
	}
	else
		printf("file descrition = %d\n", fd);


	wr_ret = write(fd, wri_buf, sizeof(wri_buf));
	printf("write %d char\n", wr_ret);

	lseek(fd,-2, SEEK_SET);
	//lseek(fd,-2, SEEK_END);
	//lseek(fd,-2, SEEK_CUR);


	re_ret = read(fd, re_buf, sizeof(re_buf));
	printf("re_buf = %d, re_buf = %s\n", re_ret, re_buf);

	close(fd);

	return 0;
}

