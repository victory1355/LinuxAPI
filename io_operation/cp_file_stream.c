#include <stdio.h>


int main(int argc, char *argv[])
{
	FILE *f1, *f2;
	char buf[1024];

	f1 = fopen(argv[1], "r");	
	if(f1 == NULL)
	{
		perror("open file1 fail");
		return -1;
	}
	f2 = fopen(argv[2], "a+");
	if(f2 == NULL)
	{
		perror("open file1 fail");
		return -1;
	}
	int  read_len;
	while(read_len = fread(buf, sizeof(char), 128, f1))
	 fwrite(buf, sizeof(char), read_len, f2);
	//printf("read %d char\n", read_len);

//	printf("write %d char\n", write_len);
	//while(len = fscanf(f1, "%s", buf))
	//	fprintf(f2, "%s", buf);
	
	fclose(f1);
	fclose(f2);

	return 0;
}
