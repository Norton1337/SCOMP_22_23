/*
    Create 1 shared memory and 2 semaphores. 
    One is a writer/reader priority one is mutual exclusion.

    I have process set A accessing the shared memory, the mutual exclusion semaphore controls which process is accessing the memory. 

    When all A processes are done, the barrier gives permission to B set to execute.^

    Attention to "sleep" times.
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

#define SEM_NAME "/sem14"
#define SEM_NAME_BUFFER "/sem14buffer"
#define SEM_NAME_BARRIER "/sem14barrier"
#define SHM_NAME "/shm14"

#define A_PROCESS 3
#define B_PROCESS 2

typedef struct{
    int list[10];
    int waiting;
}integers;

int main(){
    sem_unlink(SEM_NAME);
    sem_unlink(SEM_NAME_BARRIER);

    pid_t pids_a[A_PROCESS];
    pid_t pids_b[B_PROCESS];

    int integers_size = sizeof(integers);
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
            if (fd < 0) {
                perror("shm_open");
                exit(1);
            }
            if (ftruncate(fd, integers_size) < 0) {
                perror("ftruncate");
                exit(1);
            }
        } else {
            perror("shm_open");
            exit(1);
        }
    }

    integers *integers = mmap(NULL, integers_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (integers == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    integers->waiting = 0;
    for(int i=0 ; i< 10; i++){
        integers->list[i]=0;
        

    } 
    
    
    sem_t *semaphore = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 1);
    if (semaphore == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }

    sem_t *semaphorePriority = sem_open(SEM_NAME_BARRIER, O_CREAT | O_EXCL, 0644, 1);
    if (semaphorePriority == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }

     sem_t *sembuffer = sem_open(SEM_NAME_BUFFER, O_CREAT | O_EXCL, 0644, 1);
    if (sembuffer == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }


    for(int i=0; i<A_PROCESS; i++){
        pids_a[i] = fork();
        
        if (pids_a[i] == -1) {
            perror("Error creating child process");
            exit(EXIT_FAILURE);
        }

        if(pids_a[i] == 0){
            printf("hello mate\n");
            fflush(stdout);
            while (1)
            {
                if(integers->waiting == 0){
                    sem_wait(semaphorePriority);
                }
                sem_wait(sembuffer);
                integers->waiting++;
                sem_post(sembuffer);
                
                sem_wait(semaphore);

                for(int j=0 ; j< 10; j++){
                    integers->list[j]=rand()%255;
                }
                
                sleep(1);
                printf("pid:%d, of A just finished - Waiting : %d \n", getpid(), integers->waiting);
                fflush(stdout);
                
                sem_wait(sembuffer);
                integers->waiting--;
                sem_post(sembuffer);
                
                if(integers->waiting == 0){
                    sem_post(semaphorePriority);
                }
                
                sem_post(semaphore);
                
                sleep(5);
            }
            
            
        }

    }

    for(int i=0; i<B_PROCESS; i++){
        pids_b[i] = fork();
        
        if (pids_b[i] == -1) {
            perror("Error creating child process");
            exit(EXIT_FAILURE);
        }

         if(pids_b[i] == 0){
            printf("in b mate\n");
            fflush(stdout);
            while (1)
            {
                sem_wait(semaphorePriority);
                sem_wait(semaphore);
                
                for(int j=0 ; j< 10; j++){
                    integers->list[j]=rand()%255;
                }
                printf("pid:%d, of B just finished - Waiting : %d \n", getpid(), integers->waiting);
                fflush(stdout);
                sem_post(semaphore);
            
                sem_post(semaphorePriority);
                sleep(6);
            }
            
        }
    }
    
}
