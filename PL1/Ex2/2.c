#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main(void){
	int status;
	//1st child
	printf("I'm...\n");
	pid_t p = fork();
	
	if(p == -1){
		perror("Fork Falhou"); exit(1);
	}
	
	if(p ==0){
		printf("I'll never join you!\n");
		exit(1);
	}
			
	waitpid(p, &status,0);
	
	//2nd
	printf("the...\n");
	pid_t p2 = fork();
	
	if(p2 == -1){
		perror("Fork Falhou"); exit(1);
	}
	
	if(p2 ==0){
		printf("I'll never join you!\n");
		exit(1);
	}
	
	waitpid(p2, &status,0);
	
	//3rd
	printf("father!\n");
	pid_t p3 = fork();
	
	if(p3 == -1){
		perror("Fork Falhou"); exit(1);
	}
	
	if(p3 ==0){
		printf("I'll never join you!\n");
		exit(1);
	}
	
	waitpid(p3, &status,0);
	
	
	return 0;
		
	
	
	
}
