#include <stdio.h>
#include <unistd.h>

int main( int argc, char *argv[]){
	
	char *who;
	int i;
	
	if( fork() ){
		who = "Ratchaneekorn Chuadee";
	}
	else{
		who = "Child";
	}
	
	for ( i = 0 ; i < 6 ; i++ ){
		printf("*fork1: %s\n",who);
	}
	
	exit(0);
}
