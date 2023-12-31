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


#define shm_name "/shm_ex5"

int main() {
    int int_size = sizeof(int);
    int fd = shm_open(shm_name, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(shm_name, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
            if (fd < 0) {
                perror("shm_open");
                exit(1);
            }
            if (ftruncate(fd, int_size) < 0) {
                perror("ftruncate");
                exit(1);
            }
        } else {
            perror("shm_open");
            exit(1);
        }
    }
    int *number = mmap(NULL, int_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (number == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    *number = 100;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        for (int i = 0; i < 1000000; i++) {
            (*number)++;
            (*number)--;
        }
        exit(0);
    } else {
        for (int i = 0; i < 1000000; i++) {
            (*number)--;
            (*number)++;
        }

    }

    waitpid(pid,NULL,0);
    printf("Final value: %d\n", *number);


    if (munmap(number, int_size) == -1) {
        perror("munmap");
        exit(1);
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
