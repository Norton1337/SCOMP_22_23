#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


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
		close(fd[0]);

		pid_t myPid = getpid();
		printf("Parent wrote: %d\n", myPid);
		write(fd[1],&myPid,sizeof(pid_t));
		
        close(fd[1]);

	} else{
		close(fd[1]);
		
        pid_t parentPID;
		read(fd[0], &parentPID, sizeof(pid_t));
		printf("Child read: %d\n", parentPID);
		
        close(fd[0]);
		}
	
}