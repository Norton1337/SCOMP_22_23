/*
    1 semaphore mutual exclusion
    1 semaphore control of resources
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

#define SEM_NAME_WRITER "/sem16_writer"
#define SEM_NAME_READER "/sem16_reader"
#define SHM_NAME "/shm16"
#define NUMBER_PROCESSES 5

typedef struct {
    int vector[10000];
    int final[1000];
    int counter;
    int largestAvg;
} vec;

int main() {
    sem_unlink(SEM_NAME_WRITER);
    sem_unlink(SEM_NAME_READER);
    int vec_size = sizeof(vec);
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
            if (fd < 0) {
                perror("shm_open");
                exit(1);
            }
            if (ftruncate(fd, vec_size) < 0) {
                perror("ftruncate");
                exit(1);
            }
        } else {
            perror("shm_open");
            exit(1);
        }
    }

    vec *shared_vec = mmap(NULL, vec_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_vec == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    shared_vec->counter = 0;
    shared_vec->largestAvg = 0;

    sem_t *sem_control = sem_open(SEM_NAME_READER, O_CREAT | O_EXCL, 0644, 0);
    if (sem_control == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }

    sem_t *sem_exclusion = sem_open(SEM_NAME_WRITER, O_CREAT | O_EXCL, 0644, 1);
    if (sem_exclusion == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }

    pid_t children[NUMBER_PROCESSES];

    for (int i = 0; i < NUMBER_PROCESSES; i++) {
        children[i] = fork();
        if (children[i] == -1) {
            perror("Fork failed");
            exit(2);
        }

        if (children[i] == 0) {
            time_t t;
            srand((int)time(&t));
           for (int j = i * 2000; j < (i + 1) * 2000; j++) {
                shared_vec->vector[j] = (rand() % 10000) + 1;
            }

            for (int j = i * 2000; j < (i + 1) * 2000; j += 10) {
                int sum = 0;
                int med = 0;
                for (int k = j; k < j + 10; k++) {
                    sum += shared_vec->vector[k];
                }
                sem_wait(sem_exclusion);
                med = sum / 10;
                shared_vec->final[shared_vec->counter] = med;
                shared_vec->counter++;
                sem_post(sem_control);
                sem_post(sem_exclusion);
            }
            exit(1);
        }
    }

    pid_t pid;
    pid = fork();
    if(pid == -1) {
        perror("Fork failed");
        exit(2);
    }
    if(pid == 0) {
        for (int i = 0; i < 1000; i++) {
            sem_wait(sem_control);
            sem_wait(sem_exclusion);
            printf("new value: %d\n", shared_vec->final[i]);
            if (shared_vec->final[i] > shared_vec->largestAvg) {
                shared_vec->largestAvg = shared_vec->final[i];
                printf("new largest avg: %d\n", shared_vec->largestAvg);
            }
            sem_post(sem_exclusion);
        }
        exit(1);
    }

    for (int i = 0; i < NUMBER_PROCESSES; i++) {
        wait(NULL);
    }

    for (int i = 0; i < 1000; i++) {
        printf("value: %d\n", shared_vec->final[i]);
    }
    printf("largest avg: %d\n", shared_vec->largestAvg);

    return 0;
}