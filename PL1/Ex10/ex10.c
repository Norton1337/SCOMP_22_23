#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>

int main(){
	int vec[2000];
	time_t t;
	
	srand((unsigned) time (&t));
	
	int n = rand() % 10;

	
	for(int i=0 ; i< 2000; i++)
		vec[i] = rand() % 10;
		
	for (int i = 0; i < 10; i++) {
		if (fork() == 0) { 
			for(int j=i*200;j<(i+1)*200; j++){
				if(vec[j] == n){
					//printf("%d\n",j-(i*200));
					exit(j-(i*200));
					} 
			}
			exit(255);/*sleep(): unistd.h*/
		}
	}
	
	for (int i = 0; i < 10; i++) {
		int status;
		waitpid(-1, &status, 0);
		printf("%d\n", WEXITSTATUS(status));
	}
}
