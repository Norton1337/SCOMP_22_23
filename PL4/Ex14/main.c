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
#include <errno.h>


#define SHM_NAME "/shm_ex14"
#define SEM_NAME "/sem_ex14"
#define SET_A 3
#define SET_B 2
#define SHARED_MEM_SIZE 10




void producer(const char *sem_name, const char *shm_name, int id) {
    sem_t *semaphore = sem_open(sem_name, 0);
    if (semaphore == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

    int fd = shm_open(shm_name, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        perror("shm_open");
        exit(1);
    }

    int *shm_ptr = mmap(NULL, SHARED_MEM_SIZE*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    int data[SHARED_MEM_SIZE];
    for (int i=0; i< SHARED_MEM_SIZE; i++){
        data[i] = rand() % 100;
    }

    sem_wait(semaphore);

    for (int i = 0; i < SHARED_MEM_SIZE; i++) {
        shm_ptr[i] = data[i];
    }

    printf("Producer %d wrote data into shm\n", id);

    munmap(shm_ptr, SHARED_MEM_SIZE*sizeof(int));
    sem_post(semaphore);
    sem_close(semaphore);
    exit(0);
}

void consumer(const char* sem_name, const char* shm_name, int process_id) {
    int* shared_mem_ptr = NULL;
    sem_t* semaphore = sem_open(sem_name, 0);
    if (semaphore == SEM_FAILED) {
        perror("Error in sem_open() for consumer");
        exit(EXIT_FAILURE);
    }

    int shm_fd = shm_open(shm_name, O_RDWR, S_IRUSR|S_IWUSR);
    if (shm_fd == -1) {
        perror("Failed to open shared memory in consumer");
        exit(EXIT_FAILURE);
    }

    shared_mem_ptr = mmap(NULL, SHARED_MEM_SIZE * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_mem_ptr == MAP_FAILED) {
        perror("Error mapping shared memory in consumer");
        exit(EXIT_FAILURE);
    }

    while (1) {
        sem_wait(semaphore); // wait for permission to access shared memory
        int producer_id = shared_mem_ptr[SHARED_MEM_SIZE - 1];
        if (producer_id == process_id) { // only read data produced by other producers
            sem_post(semaphore); // release shared memory for other processes
            continue;
        }
        printf("Consumer %d reading data from shared memory:\n", process_id);
        for (int i = 0; i < SHARED_MEM_SIZE - 1; i++) {
            printf("%d ", shared_mem_ptr[i]);
        }
        printf("\n");

        shared_mem_ptr[SHARED_MEM_SIZE - 1] = process_id; // update the shared memory to indicate the last producer who wrote data
        sem_post(semaphore); // release shared memory for other processes
        sleep(6); // sleep for 6 seconds before reading data again
    }
}

int main() {
    srand(time(NULL)); // initialize random seed

    // Create shared memory
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(fd, SHARED_MEM_SIZE * sizeof(int)) < 0) {
        perror("ftruncate");
        exit(1);
    }
    int *shared_mem = mmap(NULL, SHARED_MEM_SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Initialize shared memory to zeros
    memset(shared_mem, 0, SHARED_MEM_SIZE * sizeof(int));

    // Create semaphore
    sem_t *semaphore = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 1);
    if (semaphore == SEM_FAILED) {
        if (errno == EEXIST) {
            semaphore = sem_open(SEM_NAME, 0);
        } else {
            perror("Error in sem_open()");
            exit(EXIT_FAILURE);
        }
    }

    // Create producer processes
    pid_t pid;
    for (int i = 0; i < SET_A; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Failed to create producer process");
            exit(1);
        } else if (pid == 0) {
            // Child process
            producer(SEM_NAME, SHM_NAME, i);
            exit(0);
        }
    }

    // Create consumer processes
    for (int i = 0; i < SET_B; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Failed to create consumer process");
            exit(1);
        } else if (pid == 0) {
            // Child process
            consumer(SEM_NAME, SHM_NAME, SET_A + i);
            exit(0);
        }
    }

    // Wait for child processes to terminate
    while (wait(NULL) > 0);

    // Clean up resources
    if (munmap(shared_mem, SHARED_MEM_SIZE * sizeof(int)) < 0) {
        perror("munmap");
        exit(1);
    }
    if (close(fd) < 0) {
        perror("close");
        exit(1);
    }
    if (sem_close(semaphore) < 0) {
        perror("sem_close");
        exit(1);
    }
    if (sem_unlink(SEM_NAME) < 0) {
        perror("sem_unlink");
        exit(1);
    }

    return 0;
}
