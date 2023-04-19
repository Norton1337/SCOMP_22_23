#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define ARRAY_SIZE 10
#define SHM_NAME "/shm_ex3"

int main(int argc, char *argv[]) {
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR|S_IWUSR);
            if (fd < 0) {
                perror("shm_open");
                exit(1);
            }
            if (ftruncate(fd, ARRAY_SIZE * sizeof(int)) < 0) {
                perror("ftruncate");
                exit(1);
            }
        } else {
            perror("shm_open");
            exit(1);
        }
    }

    int *numArray = mmap(NULL, ARRAY_SIZE * sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (numArray == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    int numSequences = 5;
    int numReceivedSequences = 0;

    while (numReceivedSequences < numSequences) {
        bool receivedSequence = false;

        while (!receivedSequence) {
            int numRead = 0;
            printf("Waiting for sequence %d...\n", numReceivedSequences + 1);
            for (int i = 0; i < ARRAY_SIZE; i++) {
                if (numArray[i] != 0) {
                    numRead++;
                }
            }
            if (numRead == ARRAY_SIZE) {
                receivedSequence = true;
                numReceivedSequences++;

                printf("Numbers read from shared memory (sequence %d):\n", numReceivedSequences);
                for (int i = 0; i < ARRAY_SIZE; i++) {
                    printf("pos[%d] = %d\n", i, numArray[i]);
                }

                for (int i = 0; i < ARRAY_SIZE; i++) {
                    numArray[i] = 0;
                }
            }
        }
    }

    if (munmap(numArray, ARRAY_SIZE * sizeof(int)) < 0) {
        perror("munmap");
        exit(1);
    }
    if (close(fd) < 0) {
        perror("close");
        exit(1);
    }
    if (shm_unlink(SHM_NAME) < 0) {
        perror("shm_unlink");
        exit(1);
    }
    exit(0);
}
