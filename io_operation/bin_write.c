#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *p;
	float read_buf[128],buf[5] = {10.2,15.6,25.6,157.3,200.1};
	p = fopen(argv[1], "rb+");
	int i;
	if(p == NULL)
	{
		perror("open fail");
		return -1;
	}
	
	int w_cnt = fwrite(buf, sizeof (float), sizeof(buf), p);
	if(w_cnt < 0)
	{
		perror("fwrite fail");
		return -1;
	}
	fflush(p);
	
	rewind(p);
	fread(read_buf, sizeof(float), 5, p);
	for(i=0; i<5; i++)
		printf("read :%.2f  ", read_buf[i]);
	printf("\n");
	
	
	int fc = fclose(p);
	if(fc<0)
	{
		perror("fclose fail");
		return -1;
	}
	
	return 0;
}