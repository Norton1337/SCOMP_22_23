#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define NUM_SEQ 5
#define arraySize 10
#define shm_name "/shm_ex3"
#define flag_name "/flag_ex3"

int main(int argc, char *argv[]) {
    int fd = shm_open(shm_name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(fd, NUM_SEQ*arraySize*sizeof(int)) < 0) {
        perror("ftruncate");
        exit(1);
    }

    int flag_fd = shm_open(flag_name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if (flag_fd < 0) {
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(flag_fd, sizeof(int)) < 0) {
        perror("ftruncate");
        exit(1);
    }

    int *numArray = mmap(NULL, NUM_SEQ*arraySize*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (numArray == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    int *flag = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, flag_fd, 0);
    if (flag == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    for (int seq = 0; seq < NUM_SEQ; seq++) {
        printf("Writing sequence %d to shared memory...\n", seq);

        for (int i = 0; i < arraySize; i++) {
            numArray[seq*arraySize + i] = rand() % 20 + 1;
        }

        *flag = 1;  // signal that a sequence has been written

        while (*flag != 0) {
            // active waiting for the reader to read the sequence
        }
    }

    if (munmap(numArray, NUM_SEQ*arraySize*sizeof(int)) < 0) {
        perror("munmap");
        exit(1);
    }
    if (munmap(flag, sizeof(int)) < 0) {
        perror("munmap");
        exit(1);
    }
    if (fd >= 0 && close(fd) < 0) {
        perror("close");
        exit(1);
    }
    if (flag_fd >= 0 && close(flag_fd) < 0) {
        perror("close");
        exit(1);
    }

    return 0;
}
