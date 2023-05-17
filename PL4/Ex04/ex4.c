/*
    2 semaphores , because we only have 2 crucial points where the control of execution is needed, in the first to father and father to second child.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

#define NumSem 2
#define NumChildren 2

int main() {
    
    sem_t *sems[NumSem];
    pid_t pids[NumChildren];
    int i;
    for (int i = 0; i < NumSem; i++) {
        char name[10];
        sprintf(name, "/sem%d", i+1);
        sems[i] = sem_open(name, O_CREAT | O_EXCL, 0644, 0);
        if (sems[i] == SEM_FAILED) {
            perror("Error creating semaphore");
            exit(EXIT_FAILURE);
        }
    }
    
    for (int i = 0; i < NumChildren; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("Error creating child process");
            exit(EXIT_FAILURE);
        }
        if(pids[i]==0 && i == 0){
            printf("1st child!\n");
            fflush(stdout);
            sem_post(sems[0]);
            exit(EXIT_SUCCESS);
        }

        if(pids[i]==0 && i == 1){
            sem_wait(sems[1]);
            printf("2nd child!\n");
            fflush(stdout);
            exit(EXIT_SUCCESS);
        }

    }
    sem_wait(sems[0]);
    printf("Im father\n");
    fflush(stdout);
    sem_post(sems[1]);
    
    for (int i = 0; i < NumChildren; i++) {
        waitpid(pids[i], NULL, 0);
    }

    for (int i = 0; i < NumSem; i++) {
        sem_close(sems[i]);
        char name[10];
        sprintf(name, "/sem%d", i+1);
        sem_unlink(name);
    }

   
    return 0;
}
