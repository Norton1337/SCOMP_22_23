#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(void){
	int i;
	int pidvec[4];
	
	 
	for (i = 0; i < 4; i++) {
		pid_t p = fork();
		if (p == 0) { 
			printf("This is the end.\n");
			exit(i);/*sleep(): unistd.h*/
		}
		pidvec[i]=p;
	}

	for(int i = 0 ; i<4 ; i++){
		int status;
		if(pidvec[i] % 2 == 0){
			
			waitpid(pidvec[i], &status ,0);
			}
		
	}
	printf("This is the end.\n");
	return 0;
}
	

