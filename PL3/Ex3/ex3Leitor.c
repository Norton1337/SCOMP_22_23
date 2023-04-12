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


#define arraySize 10
#define shm_name "/shm_ex3"

int main(int argc, char *argv[]) {
    int fd = shm_open(shm_name, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(shm_name, O_RDWR, S_IRUSR|S_IWUSR);
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

    printf("Numbers read from shared memory:\n");
    for (int i = 0; i < arraySize; i++) {
        printf("pos[%d] = %d\n",i, numArray[i]);
    }

    int r = munmap(numArray, arraySize); /* disconnects */
    if (r < 0) printf("ERROR"); /* Check error */
    r = close(fd); /* closes */
    if (r < 0) printf("ERROR"); /* Check error*/
    
    if (shm_unlink(shm_name) < 0) {
        perror("shm_unlink");
        exit(1);
    }
    exit(0);
    return 0;
}
