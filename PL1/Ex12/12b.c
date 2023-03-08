#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>


int spawn_childs(int n){
	
	for(int i=0; i<n; i++){
		if(fork()==0){
			exit((i+1)*2);
		}
	}
	return 0;
}


int main(){
	int status;
	int n = 6;
	int p = spawn_childs(n);
	
	printf("%d\n", p); //confirmado o p a 0
	
	for (int i = 0; i < n; i++) {
		int status;
		waitpid(-1, &status, 0);
		printf("%d\n", WEXITSTATUS(status));
	}
	
}
