#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>

int main(void){

    int fd1[2];  
	int fd2[2];
	int fd3[2];
	int fd4[2];
	int fd5[2];

    if(pipe(fd1) == -1){
		perror("Pipe failed");
		return 1;
	}
	if(pipe(fd2) == -1){
		perror("Pipe failed");
		return 1;
	}
    if(pipe(fd3) == -1){
		perror("Pipe failed");
		return 1;
    }
    if(pipe(fd4) == -1){
		perror("Pipe failed");
		return 1;
	}
	if(pipe(fd5) == -1){
		perror("Pipe failed");
		return 1;
	}

    srand(time(NULL));

	int vec1[1000];
	int vec2[1000];
	for (int i = 0; i < 1000; i++)
        vec1[i] = 1 + (rand() % 100);
    for (int i = 0; i < 1000; i++)
        vec2[i] = 1 + (rand() % 100);

    int testValue=653;
	printf("vec1[%d] = %d, vec2[%d] = %d,  %d\n",testValue,vec1[testValue],testValue,vec2[testValue],vec1[testValue]+vec2[testValue]);

    for(int i=0;i<5;i++){

        if(fork()==0){
            close(fd1[0]);// Close Read
            close(fd2[0]);// Close Read
            close(fd3[0]);// Close Read
            close(fd4[0]);// Close Read
            close(fd5[0]);// Close Read

            int result[200];
            for(int j=0; j<200;j++){
                result[j]=vec1[j+i*200]+vec2[j+i*200];
            }

            switch(i){
				case 0:
					write(fd1[1],result,sizeof(int[200]));
					break;
				case 1:
					write(fd2[1],result,sizeof(int[200]));
					break;
				case 2:
					write(fd3[1],result,sizeof(int[200]));
					break;
				case 3:
					write(fd4[1],result,sizeof(int[200]));
					break;
				case 4:
					write(fd5[1],result,sizeof(int[200]));
					break;	
			}


			close(fd1[1]); //Close Write
			close(fd2[1]); //Close Write
			close(fd3[1]); //Close Write
			close(fd4[1]); //Close Write
			close(fd5[1]); //Close Write

			exit(1);
		}	


    }

    close(fd1[1]); //Close Write
    close(fd2[1]); //Close Write
    close(fd3[1]); //Close Write
    close(fd4[1]); //Close Write
    close(fd5[1]); //Close Write

    int result[200];
	int total[1000];
	for(int i=0;i<5;i++){
		
		switch(i){
			case 0:
				read(fd1[0],result,sizeof(int[200]));
				break;
			case 1:
				read(fd2[0],result,sizeof(int[200]));
				break;
			case 2:
				read(fd3[0],result,sizeof(int[200]));
				break;
			case 3:
				read(fd4[0],result,sizeof(int[200]));
				break;
			case 4:
				read(fd5[0],result,sizeof(int[200]));
				break;
				
		}

		for(int j=0;j<200;j++)
			total[j+i*200]=result[j];
			
	}
    printf("Total[%d] = %d\n",testValue,total[testValue]);

    close(fd1[0]);// Close Read
    close(fd2[0]);// Close Read
    close(fd3[0]);// Close Read
    close(fd4[0]);// Close Read
    close(fd5[0]);// Close Read

}