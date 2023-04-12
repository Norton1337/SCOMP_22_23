#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main() {
    // Create shared memory area to store integer
    int *number = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (number == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Initialize shared integer to 100
    *number = 100;

    // Create child process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        for (int i = 0; i < 1000000; i++) {
            (*number)++;
            (*number)--;
        }
        exit(EXIT_SUCCESS);
    } else {
        for (int i = 0; i < 1000000; i++) {
            (*number)--;
            (*number)++;
        }

    }

    waitpid(pid,NULL,0);
    printf("Final value: %d\n", *number);


    if (munmap(number, sizeof(int)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    return 0;
}
