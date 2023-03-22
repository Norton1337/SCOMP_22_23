#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>
#include <time.h>
int main(void){

	int fd[2];

	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	srand(time(NULL));
	

	int vec1[1000];
	int vec2[1000];
	for (int i = 0; i < 2000; i++) {
		if(i<1000)
			vec1[i] = 1 + (rand() % 100);
		else
			vec2[i-1000] = 1 + (rand() % 100);
	}

	
	for(int i=0;i<5;i++){
		if(fork()==0){
			// Child
			close(fd[0]);
			int tmp=0;
			for(int k=i*200;k<i*200+200;k++)
				tmp +=vec1[k]+vec2[k];
			
			printf("tmp: %d\n",tmp);
			write(fd[1],&tmp,sizeof(tmp));

			close(fd[1]);

			exit(1);
		}			
	}
	// Parent
	close(fd[1]);
	
	int total=0;
	for(int i=0;i<5;i++){
		int tmp;
		read(fd[0],&tmp,sizeof(total));
		total+=tmp;
	}
	
	
	printf("total : %d\n",total);
	
	close(fd[0]);
}



/* there is no need for synchronization because the parent will read
 * 5 values and their order doesn't matter.
 * 
 */
