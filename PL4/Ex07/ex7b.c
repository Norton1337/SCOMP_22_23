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

#define SHM_NAME "/shm_ex7b"

typedef struct {
    int total;
} buys;

int main(){
    
    pid_t pids[CHILD_AMOUNT];
    sem_t *sems[SEM_AMOUNT];

    int buys_size = sizeof(buys);
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
            if (fd < 0) {
                perror("shm_open");
                exit(1);
            }
            if (ftruncate(fd, buys_size) < 0) {
                perror("ftruncate");
                exit(1);
            }
        } else {
            perror("shm_open");
            exit(1);
        }
    }

    buys *buys = mmap(NULL, buys_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    buys->total = 0;
    if (buys == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    


    for (int i = 0; i < SEM_AMOUNT; i++) {
        char name[15];
        sprintf(name, "/sem_ex7b_%d", i+1);
        sem_unlink(name);
    }
    
    srand(getpid());
    int buy_beer_amount = (rand() % 5)+1;
    int buy_chips_amount = 6-buy_beer_amount;

    printf("buy_beer_amount: %d\n",buy_beer_amount);
    fflush(stdout);
    printf("buy_chips_amount: %d\n",buy_chips_amount);
    fflush(stdout);
    

 

    sems[0] = sem_open("/sem_ex7b_1", O_CREAT | O_EXCL, 0644, 1);
    if (sems[0] == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }

    sems[1] = sem_open("/sem_ex7b_2", O_CREAT | O_EXCL, 0644, 0);
    if (sems[1] == SEM_FAILED) {
        perror("Error creating semaphore");
        exit(EXIT_FAILURE);
    }
    




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
           
            sem_wait(sems[0]);
            buys->total++;
            sem_post(sems[0]);

            if(buys->total==6){
                sem_post(sems[1]);
            }
            else{
                sem_wait(sems[1]);
                sem_post(sems[1]);
            }
           
           

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
            sem_wait(sems[0]);
            buys->total++;
            sem_post(sems[0]);


            if(buys->total==6){
                sem_post(sems[1]);
            }
            else{
                sem_wait(sems[1]);
                sem_post(sems[1]);
            }
           


            printf("Process [%d] - eat_and_drink()\n",getpid());
            fflush(stdout);
            exit(EXIT_SUCCESS);
        }   
   }

    srand(getpid());
    int r = rand() % 5;
    sleep(r);


    printf("Father - buy_chips()\n");
    sem_wait(sems[0]);
    buys->total++;
    sem_post(sems[0]);

    if(buys->total==6){
        sem_post(sems[1]);
    }
    else{
        sem_wait(sems[1]);
        sem_post(sems[1]);
    }


    printf("Father - eat_and_drink()\n");



    for (int i = 0; i < CHILD_AMOUNT; i++) {
        waitpid(pids[i], NULL, 0);
    }
    
    if (munmap(buys, buys_size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    if (fd >= 0 && close(fd) < 0) {
        perror("close");
        exit(1);
    }


    if (shm_unlink(SHM_NAME) < 0) {
        perror("shm_unlink");
        exit(1);
    }

    for (int i = 0; i < SEM_AMOUNT; i++) {
        sem_close(sems[i]);
        char name[15];
        sprintf(name, "/sem_ex7b_%d", i+1);
        sem_unlink(name);
    }
}
