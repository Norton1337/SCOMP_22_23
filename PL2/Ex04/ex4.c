#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

int main(void){
	int fd[2];
	pid_t pid;
	
	if(pipe(fd) ==-1){
		perror("Pipe failed");
		return 1;		
	}
	
	pid = fork();
	if(pid>0){
		//Parent
		close(fd[0]);
		
		char line[256];
		FILE *fp = fopen("textoEx4.txt", "r");
		int linesAmount = 0;
		while (fgets(line,sizeof(line),fp)){
			linesAmount++;
		}
		fclose(fp);
		
		write(fd[1],&linesAmount,sizeof(int));
		fp=fopen("textoEx4.txt","r");
		while (fgets(line,sizeof(line),fp)){
			write(fd[1],line,sizeof(line));
		}
		fclose(fp);
		
		close(fd[1]);
		int status= 0;
		wait(&status);
		printf("\n\n Child has finished\n");
		
	}else{
		//child process
		close(fd[1]);
		int amountLines;
		char string[256];
		read(fd[0],&amountLines,sizeof(int));
		printf("\n");
		
		for(int i = 0; i<amountLines; i++){
			read(fd[0],string,sizeof(string));
			printf("%s",string);
		}
		close(fd[0]);
		exit(1);
	}
	return 0;
}
