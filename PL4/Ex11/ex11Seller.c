/*

Create shared memory and semaphores.
Initialize data structure in shared memory.

    While LOOP:
        Wait for semaphore2. (Client process will post it when it's ready to receive a client)
        Increment the ticket number.
        Post semaphore1. (Client process will wait for it to indicate that the ticket has been obtained)




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

    sem_t *semaphore1 = sem_open(SEM_NAME_1, O_CREAT | O_EXCL, 0644, 0);
    if (semaphore1 == SEM_FAILED) {
        if (errno == EEXIST) {
            semaphore1 = sem_open(SEM_NAME_1, 0);
        } else {
            perror("Error in sem_open()");
            exit(EXIT_FAILURE);
        }
    }
    sem_t *semaphore2 = sem_open(SEM_NAME_2, O_CREAT | O_EXCL, 0644, 0);
    if (semaphore2 == SEM_FAILED) {
        if (errno == EEXIST) {
            semaphore2 = sem_open(SEM_NAME_2, 0);
        } else {
            perror("Error in sem_open()");
            exit(EXIT_FAILURE);
        }
    }

    tickets *ticketsBuf = mmap(NULL, buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ticketsBuf == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    ticketsBuf->current = 0;

    sem_post(semaphore1);

    while(1){
        sem_wait(semaphore2);

        ticketsBuf->current++;
        printf("Seller sold a ticket! Current value: %d\n", ticketsBuf->current);
        fflush(stdout);

        sem_post(semaphore1);

    }


    if (munmap(ticketsBuf, buffer_size) == -1) {
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


    sem_close(semaphore1);
    sem_close(semaphore2);
    sem_unlink(SEM_NAME_1);
    sem_unlink(SEM_NAME_2);





    return 0;
}

