#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[])
{
	int ret;
	int i;
	char argu[100] = {0};
	char *p = argv[1];
    for(i=0; i<strlen(argv[1]); i++)
	{
		argu[i] = p[i];
		if(p[i] == '/' || i== strlen(argv[1])-1)
		{	
			mkdir(argu, S_IRWXU|S_IRWXG| S_IRWXO);
			printf("create:%s\n", argu);
		}
			
	}
	
	return 0;
}

















