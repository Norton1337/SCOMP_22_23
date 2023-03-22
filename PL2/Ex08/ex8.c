#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>
#include <time.h>

struct winPipe {
	char message[5];
	int roundNumber;
	
};

int main(void){

	int fd[2];

	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}


	
	for(int i=0;i<10;i++){
		if(fork()==0){
			// Child
			close(fd[1]);

			
			struct winPipe pipe;
			read(fd[0],&pipe,sizeof(pipe));
			printf("%s round %d\n",pipe.message,pipe.roundNumber);

			close(fd[0]);

			exit(pipe.roundNumber);
		}			
	}
	// Parent
	close(fd[0]);
	struct winPipe pipe;
	strcpy (pipe.message, "Win");
	for(int i=1;i<11;i++){
		pipe.roundNumber=i;
		write(fd[1],&pipe,sizeof(pipe));
		sleep(2);
		
		
	}
	//sleep(1);
	int status;
	for(int i=0;i<10;i++){
		pid_t childID = wait(&status);
		printf("Child: %d won round %d\n",childID,WEXITSTATUS(status));
		
	}
	close(fd[1]);
}
