#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>
#include <time.h>

typedef struct {
	int customer_code;
	int product_code;
	int quantity;
}saleRecord;

int main(void){

	int fd[2];

	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}

	srand(time(NULL));
	

	saleRecord array[5000];
	
	for (int i = 0; i < 5000; i++) {
		
		array[i].customer_code = 1 + (rand() % 5000);
		array[i].product_code = 1 + (rand() % 100);
		array[i].quantity = 1 + (rand() % 100);
		
	}

	
	for(int i=0;i<10;i++){
		if(fork()==0){
			// Child
			close(fd[0]);
			int amount=0;
			for(int k=500*i;k<500+i*500;k++){
				if(array[k].quantity>20){
					write(fd[1],&array[k].product_code,sizeof(int));
					amount++;
				}
				
				
				
			}
			


			close(fd[1]);

			exit(amount);
		}			
	}
	int amount=0;
	int status;
	for(int i=0;i<10;i++){
		wait(&status);
		amount+=WEXITSTATUS(status);
	
	}
	
	// Parent
	close(fd[1]);

	int products[5000];
	for(int i=0;i<amount;i++){
		read(fd[0],&products[i],sizeof(int));
		
	}
	for(int i=0;i<amount;i++){
		
		printf("product[%d]: %d\n",i,products[i]);
	}

	

	close(fd[0]);
}