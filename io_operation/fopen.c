#include <stdio.h>
#include <errno.h>



int main(int argc, char *argv[])
{	
	int ret, wr_cnt ,rd_cnt;
	char wr_buf[] = "hello,kitty!";
	char rd_buf[128] = {0};


	FILE *fp;
	fp = fopen(argv[1], "a");
	if(fp == NULL)
	{
		printf("fopen fail.\n");
		return -1;
	}

	printf("open file %s success.\n", argv[1]);



	wr_cnt = fwrite(wr_buf, sizeof(char), sizeof(wr_buf), fp);
	if(wr_cnt < sizeof(wr_buf))
	{
		printf("write file fail. \n");
		return -1;
	}
	printf("cnt_write: %d, write: %s \n", wr_cnt, wr_buf);
	
	//rewind(fp);
	long ft = ftell(fp);
	printf("ft = %ld\n", ft);
	
	//fseek(fp, 0L, SEEK_SET);
	rewind(fp);
	fflush(fp);
	
	//ft = ftell(fp);
	//printf("ft = %ld\n", ft);
	
	rd_cnt = fread(rd_buf, sizeof(char),sizeof(rd_buf), fp);
	if(rd_cnt == 0)
	{
		ferror("fread fail");
		printf("errno == %d\n", errno);
		return -1;
	}
	printf("cnt_read: %d, read: %s \n", rd_cnt, rd_buf);
	
	
	


	ret = fclose(fp);
	if(ret == 0)
	{
		printf("close success. \n");
	}
	else 
		printf("close fail. \n");

	return 0;
}


