
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

int main(void){

	int fd[2];
	pid_t pid;
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	pid = fork();
    if(pid==-1){
        perror("Failed Fork");
        exit(1);
    }

	if(pid > 0){
		// Parent
		close(fd[0]);

		write(fd[1],"Hello World",sizeof(char[20]));
		write(fd[1],"Goodbye!",sizeof(char[20]));
		close(fd[1]);
		int status=0;
		wait(&status);
		printf("PID Value: %d\n",getpid());
	} else{
		// Child
		close(fd[1]);
		char string[20];
		read(fd[0],string,sizeof(char[20]));
		printf("Received string: %s\n",string);
		read(fd[0],string,sizeof(char[20]));
		printf("Received string: %s\n",string);
		close(fd[0]);
		exit(1);
		}
	
}