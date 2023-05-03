#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <time.h>



#define SEM_AMOUNT 2
#define CHILD_AMOUNT 1

int main(){
    
    pid_t pids[CHILD_AMOUNT];
    sem_t *sems[SEM_AMOUNT];
    
    for(int i = 0; i < SEM_AMOUNT; i++){
        char name[15];
        sprintf(name, "/sem_ex7a_%d", i+1);
        sems[i] = sem_open(name, O_CREAT | O_EXCL, 0644, 0);
        if (sems[i] == SEM_FAILED) {
            perror("Error creating semaphore");
            exit(EXIT_FAILURE);
        }
    }
   
   for (int i = 0; i < CHILD_AMOUNT; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("Error creating child process");
            exit(EXIT_FAILURE);
        }
        if(pids[i] == 0){
            if(i==0){

                srand(time(NULL));
                int r = rand() % 5;
                sleep(r);

                
                printf("Process [%d] - buy_chips()\n",i);

                if(sem_post(sems[0])==-1){
                    perror("Error in sem_post()\n");
                    exit(EXIT_FAILURE);
                }

                if(sem_wait(sems[1])==-1){
                    perror("Error in sem_wait()\n");
                    exit(EXIT_FAILURE);
                }

                printf("Process [%d] - eat_and_drink()\n",i);
                

                
                exit(EXIT_SUCCESS);
            }
        }
   }

    srand(time(NULL));
    int r = rand() % 5;
    sleep(r);


    printf("Father - buy_beer()\n");

    if(sem_post(sems[1])==-1){
        perror("Error in sem_post()\n");
        exit(EXIT_FAILURE);
    }

    if(sem_wait(sems[0])==-1){
        perror("Error in sem_wait()\n");
        exit(EXIT_FAILURE);
    }

    printf("Father - eat_and_drink()\n");


    for (int i = 0; i < CHILD_AMOUNT; i++) {
        waitpid(pids[i], NULL, 0);
    }

    for (int i = 0; i < SEM_AMOUNT; i++) {
        sem_close(sems[i]);
        char name[15];
        sprintf(name, "/sem_ex7a_%d", i+1);
        sem_unlink(name);
    }
}
