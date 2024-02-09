#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main(int agrc, char *agrv[]){
	char *who;
	int status;
	
	if( fork() ){
		who = "Ratchaneekorn Chuadee";
		printf("%p = %f\n", 4*atan(1));
		wait(&status);
		exit(0);
	}else{
		who = "child";
		execlp ("/bin/my-script", "my-script", "a", "b", (char *)0);
	}
}

