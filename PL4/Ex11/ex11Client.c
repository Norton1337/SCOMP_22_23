/*

Parse command-line argument to determine the number of clients.
Connect to shared memory and semaphores.
Initialize data structure in shared memory.

    For each child process:
        Generate a random delay.
        Wait for semaphore1. (Seller process will post it when it's ready to receive a client)
        Sleep for the generated delay to simulate the client's arrival.
        Print obtained ticket number.
        Post semaphore2. (Seller process will wait for it to indicate that the client has been served)

Wait for all child processes to finish.
Cleanup shared memory and semaphores.


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

#define SHM_NAME "/shm_ex11"
#define SEM_NAME_1 "/sem_ex11_1"
#define SEM_NAME_2 "/sem_ex11_2"
typedef struct {
    int current;
} tickets;

int main(int argc, char *argv[]){

    if (argc != 2) {
        printf("Invalid number of arguments!\n");
        return 1;
    }

    int num_clients = atoi(argv[1]);
    pid_t pids[num_clients];
    int buffer_size = sizeof(tickets);
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
            if (fd < 0) {
                perror("shm_open");
                exit(1);
            }
            if (ftruncate(fd, buffer_size) < 0) {
                perror("ftruncate");
                exit(1);
            }
        } else {
            perror("shm_open");
            exit(1);
        }
    }

    sem_t *semaphore1 = sem_open(SEM_NAME_1, 0644, 0);
    if (semaphore1 == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }
    sem_t *semaphore2 = sem_open(SEM_NAME_2, 0644, 0);
    if (semaphore2 == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }

    tickets *ticketsBuf = mmap(NULL, buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ticketsBuf == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }




    for(int i=0;i<num_clients;i++){
        pids[i] = fork();
        if(pids[i]<0){
            perror("fork");
            exit(1);
        }
        if(pids[i]==0){
            srand(getpid()*time(NULL));
            
            
            sem_wait(semaphore1);
            sleep((rand()%10)+1);
            int myTicket = ticketsBuf->current;
            printf("Client %d got ticket %d\n", getpid(), myTicket);
            fflush(stdout);
            sem_post(semaphore2);


            exit(0);
        }
    }

    for (int i = 0; i < num_clients; i++) {
        waitpid(pids[i], NULL, 0);
    }


    if (munmap(ticketsBuf, buffer_size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }


    return 0;
}

