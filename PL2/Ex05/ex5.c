#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>

int main(void){

	int fd[2];
	int fd2[2];
	pid_t pid;
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	if(pipe(fd2) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	
	pid = fork();

    if(pid==-1){
        perror("Failed Fork");
        exit(1);
    }


	if(pid > 0){
		// Parent | Server
		close(fd[0]); //Close Read
		close(fd2[1]); //Close Write

		char newString[256];
		read(fd2[0],newString,sizeof(newString));
		for(int i=0;newString[i]!='\0';i++){
			newString[i] = toupper(newString[i]);
			
		}
		write(fd[1], newString,sizeof(newString));

		close(fd[1]); //Close Write
		close(fd2[0]); //Close Read

	} else{
		// Child | Client
		close(fd[1]); //Close Write
		close(fd2[0]); //Close Read
		
		char newString[256];
		printf("Insert string: ");
		fgets(newString,256,stdin);
		write(fd2[1],newString,sizeof(newString));
		read(fd[0],newString,sizeof(newString));
		printf("%s\n",newString);
		
		close(fd[0]); //Close Read
		close(fd2[1]); //Close Write
		exit(1);
		}
	
}