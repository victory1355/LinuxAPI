#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{

	execl("/mnt/hgfs/workspace/system_interface/", "./hello", NULL);	


	return 0;
}
