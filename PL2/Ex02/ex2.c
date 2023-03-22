#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct {
		int newInt;
		char newString[256];
}newStruct; 

int main() {
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    }

    if (pid > 0) {
		//Parent process
		close(fd[0]);
		newStruct ns;
		
		printf("Write an Integer:");
		scanf("%d", &ns.newInt);
		while(getchar()!='\n');
		printf("write a string:");
		fgets(ns.newString,256,stdin);
		
		write(fd[1], &ns, sizeof(newStruct));
		
		close(fd[1]);
		
	}else{
		//child process
		newStruct parentStruct;
		read(fd[0], &parentStruct,sizeof(newStruct));
		
		printf("Parent int = %d\n", parentStruct.newInt);
		printf("Parent string = %s\n", parentStruct.newString);
		close(fd[0]);
		
	}
	
	return 0;
	
}
