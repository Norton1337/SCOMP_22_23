#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>
#include <time.h>

#define NUM_PIPES 6

typedef struct {
	int id;
	int readerID;
}request;

typedef struct {
	char name[256];
	double price;
}product;



int main(void){
	int fd_vec[NUM_PIPES][2];
	
	for(int i = 0; i<NUM_PIPES; i++){
		if(pipe(fd_vec[i]) == -1){
			perror("pipe failed");
			return 1;
		}
	}
	
	for(int i = 0; i<NUM_PIPES-1; i++){
		
		if(fork() == 0){
			//Child process
			
			close(fd_vec[0][0]);
			
			for(int j = 1; j<NUM_PIPES; j++){
				close(fd_vec[j][1]);
				if (i != j){
					close(fd_vec[j][0]);
				}
			}
			
			
		
			while(1){
				srand((int)time(NULL) % getpid()); 
				sleep(1 + (rand() % 7));
				int productID = 1 + (rand() % 4);
				
				request newProduct;
				newProduct.id=productID;
				newProduct.readerID=i;
				
				write(fd_vec[0][1],&newProduct,sizeof(request));
				product products;
				
				switch(i){
					case 0:
						read(fd_vec[i][0],&products,sizeof(product));
						break;
					case 1:
						read(fd_vec[i][0],&products,sizeof(product));
						break;
					case 2:
						read(fd_vec[i][0],&products,sizeof(product));
						break;
					case 3:
						read(fd_vec[i][0],&products,sizeof(product));
						break;
					case 4:
						read(fd_vec[i][0],&products,sizeof(product));
						break;
				}
				printf("product name: %s, price: %.2f EUR\n",products.name,products.price);
			}		
		}
	}
	//Parent process
	close(fd_vec[0][1]);
	for(int i = 1; i<NUM_PIPES ; i++){
		close(fd_vec[i][0]);
	}
	product products[5] = {
			{"product1", 10},
			{"product2", 20},
			{"product3", 30},
			{"product4", 40},
			{"product5", 50}
		};
	
	request childRequest;
	while(1){
		read(fd_vec[0][0],&childRequest,sizeof(request));
		printf("id: %d\n",childRequest.id);
		
		switch(childRequest.readerID){
					
			case 0:
				write(fd_vec[childRequest.readerID][1],&products[childRequest.id],sizeof(product));
				break;
			case 1:
				write(fd_vec[childRequest.readerID][1],&products[childRequest.id],sizeof(product));
				break;
			case 2:
				write(fd_vec[childRequest.readerID][1],&products[childRequest.id],sizeof(product));
				break;
			case 3:
				write(fd_vec[childRequest.readerID][1],&products[childRequest.id],sizeof(product));
				break;
			case 4:
				write(fd_vec[childRequest.readerID][1],&products[childRequest.id],sizeof(product));
				break;
		}
	}
	for(int k = 1 ; k<5; k++){
		close(fd_vec[k][0]);
	}

	
	
}
