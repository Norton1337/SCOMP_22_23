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



#define CHILD_AMOUNT 5
#define SEM_AMOUNT 2
int main(){
    
    pid_t pids[CHILD_AMOUNT];
    sem_t *sems[SEM_AMOUNT];

    for (int i = 0; i < SEM_AMOUNT; i++) {
        char name[15];
        sprintf(name, "/sem_ex7b_%d", i+1);
        sem_unlink(name);
    }
    
    srand(getpid());
    int buy_beer_amount = (rand() % 6)+1;
    int buy_chips_amount = 6-buy_beer_amount;

    printf("buy_beer_amount: %d\n",buy_beer_amount);
    fflush(stdout);
    printf("buy_chips_amount: %d\n",buy_chips_amount);
    fflush(stdout);
    

    sems[0] = sem_open("/sem_ex7b_1", O_CREAT | O_EXCL, 0644, buy_beer_amount);
    if (sems[0] == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }
    sems[1] = sem_open("/sem_ex7b_2", O_CREAT | O_EXCL, 0644, buy_chips_amount);
    if (sems[1] == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }

    int sem_0_value;
    int sem_1_value;

   for (int i = 0; i < buy_beer_amount; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("Error creating child process");
            exit(EXIT_FAILURE);
        }
        if(pids[i] == 0){

            srand(getpid());
            int r = rand() % 5;
            sleep(r);

            printf("Process [%d] - buy_beer()\n",getpid());
            fflush(stdout);
           
            if(sem_wait(sems[0])==-1){
                perror("Error in sem_wait()\n");
                exit(EXIT_FAILURE);
            }
           
            do{
                sem_getvalue(sems[0], &sem_0_value);
                sem_getvalue(sems[1], &sem_1_value);
            }while(sem_0_value!=0 || sem_1_value!=0);

            printf("Process [%d] - eat_and_drink()\n",getpid());
            fflush(stdout);
            exit(EXIT_SUCCESS);
        }
   }  
   for (int i = buy_beer_amount; i < buy_chips_amount+buy_beer_amount-1; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("Error creating child process");
            exit(EXIT_FAILURE);
        }
        if(pids[i] == 0){

            srand(getpid());
            int r = rand() % 5;
            sleep(r);

            printf("Process [%d] - buy_chips()\n",getpid());
            fflush(stdout);
            if(sem_wait(sems[1])==-1){
                perror("Error in sem_wait()\n");
                exit(EXIT_FAILURE);
            }

            
            do{
                sem_getvalue(sems[0], &sem_0_value);
                sem_getvalue(sems[1], &sem_1_value);
            }
            while(sem_0_value!=0 || sem_1_value!=0);


            printf("Process [%d] - eat_and_drink()\n",getpid());
            fflush(stdout);
            exit(EXIT_SUCCESS);
        }   
   }

    srand(getpid());
    int r = rand() % 5;
    sleep(r);


    printf("Father - buy_chips()\n");

    if(sem_wait(sems[1])==-1){
        perror("Error in sem_wait()\n");
        exit(EXIT_FAILURE);
    }
    do{
        sem_getvalue(sems[0], &sem_0_value);
        sem_getvalue(sems[1], &sem_1_value);
    }while(sem_0_value!=0 || sem_1_value!=0);

    printf("Father - eat_and_drink()\n");


    for (int i = 0; i < CHILD_AMOUNT; i++) {
        waitpid(pids[i], NULL, 0);
    }

    for (int i = 0; i < SEM_AMOUNT; i++) {
        sem_close(sems[i]);
        char name[15];
        sprintf(name, "/sem_ex7b_%d", i+1);
        sem_unlink(name);
    }
}
