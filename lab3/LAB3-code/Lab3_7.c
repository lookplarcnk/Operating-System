#include <stdio.h> 
#include <stdlib.h>

char *name = "Ratchaneekorn Chuadee";
main()
{
	int pid;
	pid = fork();
	if (pid != 0) 
	{
		while (1); sleep(100);
	} else {
		while (1) {
			printf("%s\n", name); sleep(10+ (rand() % 5));
		}
	}
}
