#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define arraySize 10
#define shm_name "/shm_ex3"

int main(int argc, char *argv[]) {
    int fd = shm_open(shm_name, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(shm_name, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
            if (fd < 0) {
                perror("shm_open");
                exit(1);
            }
            if (ftruncate(fd, arraySize) < 0) {
                perror("ftruncate");
                exit(1);
            }
        } else {
            perror("shm_open");
            exit(1);
        }
    }



    int *numArray = mmap(NULL, arraySize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (numArray == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }


    for (int i = 0; i < arraySize; i++) {
        numArray[i] = rand() % 20 + 1;
    }

    printf("Numbers written to shared memory:\n");
    for (int i = 0; i < arraySize; i++) {
        printf("pos[%d] = %d\n",i, numArray[i]);
    }
   
   
    if (munmap(numArray, arraySize) < 0) {
        perror("munmap");
        exit(1);
    }
    if (fd >= 0 && close(fd) < 0) {
        perror("close");
        exit(1);
    }

    return 0;
}
