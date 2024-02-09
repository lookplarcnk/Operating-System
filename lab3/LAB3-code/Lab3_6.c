#include<stdio.h>
main() {
	int pid;
	printf("I'm the original process with PID %d and PPID %d.\n",getpid(),getppid());
	pid = fork();
	if(pid != 0) {
		printf("I'm the parent with PID %d and PPID %d\n",getpid(),getppid());
		printf("My child's PID is %d\n",pid);
	} else {
		sleep(4);
		printf("I'm the child with PID %d and PPID %d.\n",getpid(),getppid());
	}
	printf("PID %d terminates.\n",getpid());
}
