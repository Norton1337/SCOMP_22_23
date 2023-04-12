#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define shm_name "/shm_ex9"
#define AMOUNT 30
#define BUFFER_SIZE 10
#define BUFFER_MASK (BUFFER_SIZE - 1)

typedef struct circular_buffer{
    int buffer[BUFFER_SIZE];
    int head;
    int tail;
    int count;
} circular_buffer;


int main() {
    int buffer_size = sizeof(circular_buffer);
    int fd = shm_open(shm_name, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(shm_name, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
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
    circular_buffer *circBuf = mmap(NULL, buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (circBuf == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }



    circBuf->head = 0;
    circBuf->tail = 0;
    circBuf->count = 0;



    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {

        for (int i = 1; i <= AMOUNT; i++) {

            circBuf->buffer[circBuf->tail] = i;
            circBuf->tail = (circBuf->tail + 1) % BUFFER_SIZE;
            circBuf->count++;

        }

        exit(0);
    }

    for (int i = 0; i < AMOUNT; i++) {

        while (circBuf->count == 0);
        int value = circBuf->buffer[circBuf->head];
        circBuf->head = (circBuf->head + 1) % BUFFER_SIZE;
        circBuf->count--;
        printf("Consumer: %d\n", value);
    }


    


    waitpid(pid,NULL,0);


    if (munmap(circBuf, buffer_size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    if (fd >= 0 && close(fd) < 0) {
        perror("close");
        exit(1);
    }


    if (shm_unlink(shm_name) < 0) {
        perror("shm_unlink");
        exit(1);
    }

    return 0;
}
