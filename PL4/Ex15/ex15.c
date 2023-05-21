/*

Create shared memory and semaphores for synchronization.
Initialize data structure in shared memory.

	For each Client Type:
		If client of this type is waiting, and there is space in the club:
			Let him in (increment the number of clients in the club).
			Decrement the number of clients of this type waiting.
		Else Chance to Generate a new client of this type
			If there is space in the club, let him in (increment the number of clients in the club).
			Else increment the number of clients of this type waiting.
		Sleep for some time to simplify the simulation.
	
	Parent:
		Randomly remove clients from the club.

    
Clean up shared memory and semaphores.

*/


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
#include <errno.h>
#include <time.h>

#define SHM_NAME "/shm_ex17"
#define SEM_NAME_BUFFER "/sem_ex17_buffer"
#define SEM_NAME_VIP "/sem_ex17_vip"
#define SEM_NAME_SPECIAL "/sem_ex17_special"
#define SEM_NAME_NORMAL "/sem_ex17_normal"

#define MAX_CAPACITY 10
#define VIP 0
#define SPECIAL 1
#define NORMAL 2
#define WAIT_TIME 100000

typedef struct {
    int vipWaiting;
    int specialWaiting;
    int normalWaiting;
	int clients;
} club_Buffer;

void vipProcess(sem_t *semBuffer,sem_t *semVip,club_Buffer *sharedMemory){
	pid_t pid = fork();
	if(pid==-1){
		perror("Error in fork()");
		exit(1);
	}
	if(pid==0){
		while(1){
			
			sem_wait(semBuffer);
			if(sharedMemory->clients<MAX_CAPACITY && sharedMemory->vipWaiting>0){
				if(sem_trywait(semVip)){

	
					
					sharedMemory->clients++;
					sharedMemory->vipWaiting--;
					if(sharedMemory->vipWaiting>0){
						sem_post(semVip);
					}
					printf("VIP Client entered the club\n");
					
					fflush(stdout);
					
				}

			}
			sem_post(semBuffer);
			
			
			
		}
		
	}
}

void specialProcess(sem_t *semBuffer,sem_t *semVip, sem_t *semSpecial,club_Buffer *sharedMemory){
	pid_t pid = fork();
	if(pid==-1){
		perror("Error in fork()");
		exit(1);
	}
	if(pid==0){
		while(1){
			
			
			sem_wait(semBuffer);
			if(sharedMemory->clients<MAX_CAPACITY && sharedMemory->specialWaiting>0){
				if(sem_trywait(semVip)==-1){
					if(sem_trywait(semSpecial)){
						
						sharedMemory->clients++;
						sharedMemory->specialWaiting--;
						if(sharedMemory->specialWaiting>0){
							sem_post(semSpecial);
						}
						printf("Special Client entered the club\n");
						fflush(stdout);
			
					}
				}
				else{
					sem_post(semVip);
				}
				
			}
			sem_post(semBuffer);
			
			
			
		}
		
	}
}

void normalProcess(sem_t *semBuffer,sem_t *semVip,sem_t *semSpecial,club_Buffer *sharedMemory){
	pid_t pid = fork();
	if(pid==-1){
		perror("Error in fork()");
		exit(1);
	}
	if(pid==0){
		while(1){
			
			
			sem_wait(semBuffer);
			if(sharedMemory->clients<MAX_CAPACITY && sharedMemory->normalWaiting>0){
				if(sem_trywait(semVip)==-1){
					if(sem_trywait(semSpecial)==-1){

							sharedMemory->clients++;
							sharedMemory->normalWaiting--;
							printf("Normal Client entered the club\n");

							fflush(stdout);
							
						
					}
					else{
						sem_post(semSpecial);
					}
				}
				else{
					sem_post(semVip);
				}
			}
			sem_post(semBuffer);
			

			
			
		}
		
	}
}


int main(int argc, char *argv[]) {
    shm_unlink(SHM_NAME);
	sem_unlink(SEM_NAME_BUFFER);
	sem_unlink(SEM_NAME_VIP);
	sem_unlink(SEM_NAME_SPECIAL);
	
	int data_size = sizeof(club_Buffer);
	int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
			fflush(stdout);
            fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
            if (fd < 0) {
                perror("shm_open");
                exit(1);
            }
            if (ftruncate(fd, data_size) < 0) {
                perror("ftruncate");
                exit(1);
            }
        } else {
            perror("shm_open");
            exit(1);
        }
    }
	club_Buffer *sharedMemory = mmap(NULL, data_size, PROT_READ | PROT_WRITE,MAP_SHARED, fd, 0);
	if (sharedMemory == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	sharedMemory->vipWaiting=0;
	sharedMemory->specialWaiting=0;
	sharedMemory->normalWaiting=0;
	sharedMemory->clients=MAX_CAPACITY;

	sem_t *semBuffer;
	sem_t *semVip;
	sem_t *semSpecial;
	sem_t *semNormal;

	if ((semBuffer = sem_open(SEM_NAME_BUFFER, O_CREAT, 0644,1)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	if ((semVip = sem_open(SEM_NAME_VIP, O_CREAT, 0644,0)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	if ((semSpecial = sem_open(SEM_NAME_SPECIAL, O_CREAT, 0644,0)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	if ((semNormal = sem_open(SEM_NAME_NORMAL, O_CREAT, 0644,0)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}

	vipProcess(semBuffer,semVip,sharedMemory);
	specialProcess(semBuffer,semVip,semSpecial,sharedMemory);
	normalProcess(semBuffer,semVip,semSpecial,sharedMemory);


	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(1);
	}
	if(pid == 0){
		time_t t;
		srand((int)time(&t));
		while(1){
			
			if(rand() %20 ==0 && sharedMemory->clients>0){
				sem_wait(semBuffer);
				sharedMemory->clients--;
				printf("a client just left, %d clients still inside\n",sharedMemory->clients);
				fflush(stdout);
				sem_post(semBuffer);
			}

			if(rand() %100 == 0){
				sem_wait(semBuffer);
				sharedMemory->vipWaiting++;
				if(sharedMemory->vipWaiting==1){
					if(sem_trywait(semVip)==-1){
						sem_post(semVip);
					}
				}


				sem_post(semBuffer);
			}

			if(rand() %50 == 0){
				sem_wait(semBuffer);
				sharedMemory->specialWaiting++;
				if(sharedMemory->specialWaiting==1){
					if(sem_trywait(semSpecial)==-1){
						sem_post(semSpecial);
					}
				}
				
				sem_post(semBuffer);
			}

			if(rand() %20 == 0){
				sem_wait(semBuffer);
				sharedMemory->normalWaiting++;
				sem_post(semBuffer);
			}



			usleep(WAIT_TIME);
					
			printf("\nvipWaiting: %d\n",sharedMemory->vipWaiting);
			fflush(stdout);
			printf("specialWaiting: %d\n",sharedMemory->specialWaiting);
			fflush(stdout);
			printf("normalWaiting: %d\n",sharedMemory->normalWaiting);
			fflush(stdout);
			printf("clients inside: %d\n",sharedMemory->clients);
			fflush(stdout);

			
		}
	}

	
	
	
	
	
	
	

	for (int i=0;i<3;i++){
        wait(NULL);            
    }
    
    if (munmap(sharedMemory, data_size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    if (fd >= 0 && close(fd) < 0) {
        perror("close");
        exit(1);
    }


    if (shm_unlink(SHM_NAME) < 0) {
        perror("shm_unlink");
        exit(1);
    }

    
	if(sem_unlink(SEM_NAME_BUFFER)==-1){
		perror("Error in sem_unlink().\n");
		exit(1);
	}
	if(sem_unlink(SEM_NAME_SPECIAL)==-1){
		perror("Error in sem_unlink().\n");
		exit(1);
	}
	if(sem_unlink(SEM_NAME_VIP)==-1){
		perror("Error in sem_unlink().\n");
		exit(1);
	}
    return 0;
}