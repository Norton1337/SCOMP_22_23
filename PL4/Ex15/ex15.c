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
#define SEM_NAME "/sem_ex17"

#define MAX_CAPACITY 10
#define VIP 0
#define SPECIAL 1
#define NORMAL 2
#define WAIT_TIME 10000

typedef struct {
    int vipWaiting;
    int specialWaiting;
    int normalWaiting;
	int clients;
} club;

void vipClient(sem_t *semClub,club *sharedMemory){
    pid_t vip = fork();
    if(vip == -1){
        perror("Error in fork()");
        exit(1);
    }
	if (vip == 0){
		time_t t;
		srand((int)time(&t) % getpid());
		while(1){

			if(sharedMemory->clients<MAX_CAPACITY && sharedMemory->vipWaiting>0){
				sem_wait(semClub);
                sharedMemory->vipWaiting--;
				sharedMemory->clients++;
				sem_post(semClub);
			}else
			if(rand() % 100==0){
				if(sharedMemory->clients==MAX_CAPACITY){
                    sem_wait(semClub);
					sharedMemory->vipWaiting++;
                    sem_post(semClub);
				}else{
					sem_wait(semClub);
					sharedMemory->clients++;
					printf("new vip, now %d clients\n",sharedMemory->clients);
					sem_post(semClub);
				}
			}
			usleep(WAIT_TIME);
			
		}
		exit(0);
	}
}

void specialClient(sem_t *semClub,club *sharedMemory){
    pid_t special = fork();
    if (special == -1){
        perror("Error in fork()");
        exit(1);
    }
	if (special == 0){
		time_t t;
		srand((int)time(&t) % getpid());
		while(1){

			if(sharedMemory->clients<MAX_CAPACITY && sharedMemory->vipWaiting==0 && sharedMemory->specialWaiting>0){
				sem_wait(semClub);
                sharedMemory->specialWaiting--;
				sharedMemory->clients++;
				sem_post(semClub);
			}else
			if(rand() % 50==1){
				if(sharedMemory->clients==MAX_CAPACITY){
                    sem_wait(semClub);
					sharedMemory->specialWaiting++;
                    sem_post(semClub);
				}else{
					sem_wait(semClub);
					sharedMemory->clients++;
					printf("new special, now %d clients\n",sharedMemory->clients);
					sem_post(semClub);
				}
			}
			usleep(WAIT_TIME);
			
		}
		exit(0);
	}
}

void normalClient(sem_t *semClub,club *sharedMemory){
    pid_t normal = fork();
    if (normal == -1){
        perror("Error in fork()");
        exit(1);
    }
	if (normal == 0){
		time_t t;
		srand((int)time(&t) % getpid());
		while(1){

			if(sharedMemory->clients<MAX_CAPACITY && sharedMemory->vipWaiting==0 && sharedMemory->specialWaiting==0 && sharedMemory->normalWaiting>0){
				sem_wait(semClub);
                sharedMemory->normalWaiting--;
				sharedMemory->clients++;
				sem_post(semClub);
			}else
			if(rand() % 25==2){
				if(sharedMemory->clients==MAX_CAPACITY){
                    sem_wait(semClub);
					sharedMemory->normalWaiting++;
                    sem_post(semClub);
				}else{
					sem_wait(semClub);
					sharedMemory->clients++;
					printf("new normal, now %d clients\n",sharedMemory->clients);
					sem_post(semClub);
				}
			}
			usleep(WAIT_TIME);
	
			
		}
		exit(0);
	}
}

int main(int argc, char *argv[]) {
    shm_unlink(SHM_NAME);
	sem_unlink(SEM_NAME);
	
	int data_size = sizeof(club);
	int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
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
	club *sharedMemory = (club*)mmap(NULL, data_size, PROT_READ | PROT_WRITE,MAP_SHARED, fd, 10);
	if (sharedMemory == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	sharedMemory->clients=MAX_CAPACITY;

	sem_t *semClub;

	
	if ((semClub = sem_open(SEM_NAME, O_CREAT, 0644,1)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
			
	vipClient(semClub,sharedMemory);
    specialClient(semClub,sharedMemory);
    normalClient(semClub,sharedMemory);

	
	
	
	
	
	time_t t;
	srand((int)time(&t));
	while(1){
		
		if(rand() %20 ==0 && sharedMemory->clients>0){
			sem_wait(semClub);
			sharedMemory->clients--;
			printf("a client just left, %d clients still inside\n",sharedMemory->clients);
			sem_post(semClub);
		}
		usleep(WAIT_TIME);
				
        printf("\nvipWaiting: %d\n",sharedMemory->vipWaiting);
        printf("specialWaiting: %d\n",sharedMemory->specialWaiting);
        printf("normalWaiting: %d\n",sharedMemory->normalWaiting);
			
		
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

    
	if(sem_unlink(SEM_NAME)==-1){
		perror("Error in sem_unlink().\n");
		exit(1);
	}
    return 0;
}