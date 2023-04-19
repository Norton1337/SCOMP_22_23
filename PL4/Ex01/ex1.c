#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <semaphore.h>
#include <string.h>
#include <time.h>

#define CHILDREN 8
#define INTEGERS 200


int main(int argc, char *argv[]) {

    sem_t *semOutput;
    sem_t *semNumbers;
    pid_t child;
    semOutput = sem_open("/semOutput", O_CREAT|O_EXCL, 0644, 1);
    if(semOutput == SEM_FAILED){
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }
    semNumbers = sem_open("/semNumbers", O_CREAT|O_EXCL, 0644, 1);
    if(semOutput == SEM_FAILED){
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

    //reset output.txt file
    FILE *outputFile = fopen("Output.txt","w");
    fclose(outputFile);

    for(int i = 0;i<CHILDREN;i++){
        child=fork();
        if (child < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (child==0){
            if(sem_wait(semNumbers)==-1){
                perror("Error in sem_wait()\n");
                exit(EXIT_FAILURE);
            }
            int storedNumbers[INTEGERS];
            FILE *numbersFile = fopen("Numbers.txt", "r");
			for(int k=0;k<INTEGERS;k++){
                fscanf(numbersFile, "%d", &storedNumbers[k]);
            }
            fclose(numbersFile);
            if(sem_post(semNumbers)==-1){
                perror("Error in sem_post()\n");
                exit(EXIT_FAILURE);
            }
            if(sem_wait(semOutput)==-1){
                perror("Error in sem_wait()\n");
                exit(EXIT_FAILURE);
            }
            FILE *outputFile = fopen("Output.txt", "a");
            //write to file
            for(int k = 0;k<INTEGERS;k++){
                    fprintf(outputFile,"[%d] %d\n", getpid(),storedNumbers[k]);
            }
            fclose(outputFile);
            if(sem_post(semOutput)==-1){
                perror("Error in sem_post()\n");
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
        }
    }

    for(int i=0;i<CHILDREN;i++){
        wait(NULL);
    }


    FILE *outputFileRead = fopen("Output.txt","r");


	pid_t pid;
	int number;
    while(!feof(outputFileRead)){
        fscanf(outputFileRead, "[%d] %d\n",&pid, &number);
        printf("[%d] %d\n",pid,number);
    }

    fclose(outputFileRead);
    

    if(sem_unlink("/semOutput")==-1){
		perror("Error in sem_unlink().\n");
	    exit(EXIT_FAILURE);
	}
    if(sem_unlink("/semNumbers")==-1){
		perror("Error in sem_unlink().\n");
        exit(EXIT_FAILURE);
	}

    return 0;
}