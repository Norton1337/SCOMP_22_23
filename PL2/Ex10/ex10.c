#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>
#include <time.h>


int main(void){

	int fd[2];
	int fd2[2];

	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}

	if(pipe(fd2) == -1){
		perror("Pipe failed");
		return 1;
	}

	
		

	if(fork()==0){
		// Child
		close(fd[1]);
		close(fd2[0]);

		int status=1;
		int myCredit;
		while(status==1){
			status=0;
			read(fd[0],&status,sizeof(int));
			if(status==1){
				srand((int)time(NULL) % getpid()); 
				int randomNumChild = 1 + (rand() % 5);
				
				write(fd2[1],&randomNumChild,sizeof(int));
				
				read(fd[0],&myCredit,sizeof(int));
				printf("My Credit is: %d\n",myCredit);
			}
		}
		close(fd[0]);
		close(fd2[1]);

		exit(1);
	}			
	int childCredit = 20;
	int n1;
	int childBet;
	// Parent
	close(fd[0]);
	close(fd2[1]);
	int status;
	int onGoing=1;
	while(onGoing){
	
		
		if(childCredit!=0){
			n1=1;
			write(fd[1],&n1,sizeof(int));
			if(waitpid(-1, &status, WNOHANG)>0){
				onGoing=0;
				printf("Child stopped working\n");
				break;
			}
			
			sleep(1);
			int randomNumParent = 1 + (rand() % 5);
			read(fd2[0],&childBet,sizeof(int));
			printf("correctNum = %d, childNum = %d\n",randomNumParent,childBet);
			if(childBet==randomNumParent)
				childCredit+=10;
			else
				childCredit-=5;
			
			write(fd[1],&childCredit,sizeof(int));	
			
		}else{
			n1=0;
			write(fd[1],&n1,sizeof(int));
			break;
		}
		sleep(0);
		
	}
	close(fd[1]);
	close(fd2[0]);
}
