#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>
#include <time.h>

#define NUM_PIPES 6

int main(){

    int fdArray[NUM_PIPES][2];

    for(int i = 0; i< NUM_PIPES;i++){
        if(pipe(fdArray[i]) == -1){
            perror("Pipe failed");
            return 1;
        }
    }

    for(int i=0;i<5;i++){
        if(fork() == 0){
            srand((int)time(NULL) % getpid());
            int random = (rand() % 500) +1 ;
            printf("child[%d] random: %d\n",getpid(),random);
            int previousRandom;

           

            switch (i){
                case 0:
                    for(int k=0;k<NUM_PIPES;k++){
                        if(k==i)
                            close(fdArray[k][1]);
                        else if(k==i+1)
                            close(fdArray[k+1][0]);
                        else{
                            close(fdArray[k][0]);
                            close(fdArray[k][1]);
                        }
                    }
                    read(fdArray[i][0],&previousRandom,sizeof(previousRandom));
                    break;
                case 1:
                    for(int k=0;k<NUM_PIPES;k++){
                        if(k==i)
                            close(fdArray[k][1]);
                        else if(k==i+1)
                            close(fdArray[k+1][0]);
                        else{
                            close(fdArray[k][0]);
                            close(fdArray[k][1]);
                        }
                    }
                    read(fdArray[i][0],&previousRandom,sizeof(previousRandom));
                    break;
                case 2:
                    for(int k=0;k<NUM_PIPES;k++){
                        if(k==i)
                            close(fdArray[k][1]);
                        else if(k==i+1)
                            close(fdArray[k+1][0]);
                        else{
                            close(fdArray[k][0]);
                            close(fdArray[k][1]);
                        }
                    }
                    read(fdArray[i][0],&previousRandom,sizeof(previousRandom));
                    break;
                case 3:
                    for(int k=0;k<NUM_PIPES;k++){
                        if(k==i)
                            close(fdArray[k][1]);
                        else if(k==i+1)
                            close(fdArray[k+1][0]);
                        else{
                            close(fdArray[k][0]);
                            close(fdArray[k][1]);
                        }
                    }
                    read(fdArray[i][0],&previousRandom,sizeof(previousRandom));
                    break;
                case 4:
                    for(int k=0;k<NUM_PIPES;k++){
                        if(k==i)
                            close(fdArray[k][1]);
                        else if(k==i+1)
                            close(fdArray[k+1][0]);
                        else{
                            close(fdArray[k][0]);
                            close(fdArray[k][1]);
                        }
                    }
                    read(fdArray[i][0],&previousRandom,sizeof(previousRandom));
                    break;

            }

            if(previousRandom>random)
                random = previousRandom;
            
            switch (i){
                case 0:
                    write(fdArray[i+1][1],&random,sizeof(random));
                    for(int k=0;k<NUM_PIPES;k++){
                        if(k==i)
                            close(fdArray[k][0]);
                        else if(k==i+1)
                            close(fdArray[k+1][1]);
                    }
                    break;
                case 1:
                    write(fdArray[i+1][1],&random,sizeof(random));
                    for(int k=0;k<NUM_PIPES;k++){
                        if(k==i)
                            close(fdArray[k][0]);
                        else if(k==i+1)
                            close(fdArray[k+1][1]);
                    }
                    break;
                case 2:
                    write(fdArray[i+1][1],&random,sizeof(random));
                    for(int k=0;k<NUM_PIPES;k++){
                        if(k==i)
                            close(fdArray[k][0]);
                        else if(k==i+1)
                            close(fdArray[k+1][1]);
                    }
                    break;
                case 3:
                    write(fdArray[i+1][1],&random,sizeof(random));
                    for(int k=0;k<NUM_PIPES;k++){
                        if(k==i)
                            close(fdArray[k][0]);
                        else if(k==i+1)
                            close(fdArray[k+1][1]);
                    }
                    break;
                case 4:
                    write(fdArray[i+1][1],&random,sizeof(random));
                    for(int k=0;k<NUM_PIPES;k++){
                        if(k==i)
                            close(fdArray[k][0]);
                        else if(k==i+1)
                            close(fdArray[k+1][1]);
                    }
                    break;
                    
            }
            exit(1);

        }
    }

    srand((int)time(NULL));
    int random = (rand() % 500) +1 ;
    printf("parent[%d] random: %d\n",getpid(),random);
    for(int k=0;k<NUM_PIPES;k++){
        if(k==0)
            close(fdArray[k][0]);
        else if(k==5)
            close(fdArray[k][1]);
        else{
            close(fdArray[k][0]);
            close(fdArray[k][1]);
        }
    }
    write(fdArray[0][1],&random,sizeof(random));
    read(fdArray[5][0],&random,sizeof(random));
    printf("Largest Random number is: %d\n",random);

    for(int k=0;k<NUM_PIPES;k++){
        if(k==0)
            close(fdArray[k][1]);
        else if(k==5)
            close(fdArray[k][0]);
    }


}

