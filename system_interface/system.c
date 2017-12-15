#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	printf("l am the process named system\n");
	printf("pid = %d, ppid = %d\n", getpid(), getppid());
	//char *argv[] = {"./hello", NULL};
	system("./hello");
	execl("/mnt/hgfs/workspace/system_interface/hello", "./hello", NULL);
	while(1);
	printf("hello\n");
	//execv("/mnt/hgfs/workspace/system_interface/hello", argv);

	//execlp(", "ls", "-l", NULL);
	//char *envp[] = {"/mnt/hgfs/workspace/system_interface/", NULL};
	//execle("/bin/ls", "ls", "-l", NULL, envp);
	//char *argv[] = {"ls ", "-l", NULL};
	//execvp("ls", argv);
	//execve("/mnt/hgfs/workspace/system_interface/hello", argv, envp);
	while(1);
	return 0;
}
