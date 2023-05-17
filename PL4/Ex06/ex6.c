/*
    Here we need 2 semaphores, one is mutual exclusion and the other is to synchronize processes with events. 
    This is to control the execution beetween the writing of the "S" first than the "C"
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

int main() {
    sem_t *sem_s, *sem_c;
    pid_t pid;
    int status;

    // Create the semaphores with initial values
    sem_s = sem_open("sem_s", O_CREAT, 0644, 1);
    sem_c = sem_open("sem_c", O_CREAT, 0644, 0);

    if (sem_s == SEM_FAILED || sem_c == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process writes 'C' indefinitely
        while (1) {
            sem_wait(sem_c);
            putchar('C');
            fflush(stdout);
            sem_post(sem_s);
        }
    } else {
        // Parent process writes 'S' indefinitely
        while (1) {
            sem_wait(sem_s);
            putchar('S');
            fflush(stdout);
            sem_post(sem_c);
        }
        waitpid(pid, &status, 0);
    }

    // Close and unlink the semaphores
    sem_close(sem_s);
    sem_close(sem_c);
    sem_unlink("sem_s");
    sem_unlink("sem_c");

    return 0;
}
