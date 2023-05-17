/*
    We need 1 shared memory and 3 semaphores. 
    1 is mutual exclusion, 1 is for sychronization of processes with events, and 1 for controlling access to resources.
    
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>
#include <semaphore.h>

#define BUF_SIZE 10
#define PRODUTORES 2
#define MAX 10

typedef struct {
	int buffer[BUF_SIZE];
    int head;
    int tail;
} circular_buffer;

typedef struct {
	circular_buffer circularBuffer;
} info;


void printBuffer(circular_buffer* buffer) {
    printf("- - - - - - - - - - - - - - - - - - - -\n");
    for (int i = 0; i < BUF_SIZE; i++) {
        printf("%d  ", buffer->buffer[i]);
        fflush(stdout);
    }
    printf("\n");
    for (int i = 0; i < BUF_SIZE; i++) {
        if(i == buffer->head && i == buffer->tail) {
            printf("HT ");
            fflush(stdout);
        } else if(i == buffer->head) {
            printf("H  ");
            fflush(stdout);
        } else if(i == buffer->tail) {
            printf("T  ");
            fflush(stdout);
        } else {
            printf("   ");
            fflush(stdout);
        }
    }
    printf("\n");
}


int main(){

    // Criação dos semáforos

    sem_t *canWrite, *canRead, *index_access;

    if ((canWrite = sem_open("/cwrite", O_CREAT | O_EXCL, 0644, BUF_SIZE)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}
	
	if ((canRead = sem_open("/cread", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	}

    if ((index_access = sem_open("/saccess", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Error in sem_open()");
        exit(1);
	} 
    
    // Criação da memória partilhada

    int size = sizeof(info); 
	info *infoPartilhada;

    int fd = shm_open("/exercicio12",O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR); 

    if(fd == -1){
	    perror("Creating or opening shared memory failure");
		shm_unlink("/exercicio12");
		return 1;
	}

    if(ftruncate(fd,size) == -1) {
		perror("Truncating shared memory failure");
		return 2;
	}

    infoPartilhada = (info*) mmap(NULL, size,PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

    // Inicialização

    for (int i = 0; i < BUF_SIZE; i++)
	{
		infoPartilhada->circularBuffer.buffer[i] = 0;
	}
	infoPartilhada->circularBuffer.head = 0;
	infoPartilhada->circularBuffer.tail = 0;
    
    sleep(1);
    printBuffer(&infoPartilhada->circularBuffer);

    for(int i = 0; i < PRODUTORES; i++){
		
		pid_t pid = fork();

        if(pid == -1){
			perror("Fork Falhou");
            exit(2);
		}

        if( pid == 0){
            // Produtores

            fflush(stdout);
		    for (int i = 0; i < 30; i++) {
			    if (sem_wait(canWrite) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }
                if (sem_wait(index_access) == -1) {
                    perror("Error at sem_wait().");
                    exit(3);
                }

			    int head = infoPartilhada->circularBuffer.head;
			    infoPartilhada->circularBuffer.buffer[head] = rand() % MAX;
			    
			
			    if(head == BUF_SIZE - 1) {
					infoPartilhada->circularBuffer.head = 0;
			    } else {
					infoPartilhada->circularBuffer.head = head + 1;
			    }
			    if (sem_post(index_access) == -1) {
                    perror("Error at sem_post().");
                    exit(3);
                
                }
                if (sem_post(canRead) == -1) {
                    perror("Error at sem_post().");
                    exit(3);
                }   
		    }

		    exit(0);
        }
    }
    

    // Consumidor
    for(int i = 0; i < 60; i++) {
        fflush(stdout);
		
		if (sem_wait(canRead) == -1) {
            perror("Error at sem_wait().");
            exit(3);
        }
            
        if (sem_wait(index_access) == -1) {
            perror("Error at sem_wait().");
            exit(3);
        }
		int tail =infoPartilhada->circularBuffer.tail;

        if(tail == BUF_SIZE - 1) {
            infoPartilhada->circularBuffer.head = 0;
        } else {
            infoPartilhada->circularBuffer.head = tail + 1;
        }

        sleep(2);
        printBuffer(&infoPartilhada->circularBuffer);
            
        if (sem_post(index_access) == -1) {
            perror("Error at sem_post().");
            exit(3);
        }
           
        if (sem_post(canWrite) == -1) {
            perror("Error at sem_post().");
            exit(3);
        }

    }

    for (int i = 0; i <PRODUTORES; i++) {
		wait(NULL);
	}

    if (munmap((void *)infoPartilhada, size) < 0) {
        printf("Error at munmap()!\n");
        return 3;
    } 

    if (close(fd) < 0) {
        printf("Error at close()!\n");
        return 4;
    }


    if (shm_unlink("/exercicio12") < 0) {
        printf("Error at shm_unlink()!\n");
        exit(1);
    }

    if (sem_unlink("/cwrite") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

    if (sem_unlink("/cread") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

    if (sem_unlink("/saccess") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

    return 0;
}
