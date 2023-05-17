/*
    We need 1 semaphore to control the acess to resources >= 0, and several others (the number of doors) that act like mutual exclusion to simulate the behaviour of the doors. 

*/


#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>

#define CAPACIDADE 20
#define QUANT_PORTAS 3
#define PASSAGEIROS 55

int main(){

    sem_t *semPortas[QUANT_PORTAS], *semCap;

    if ((semCap = sem_open("/s112", O_CREAT | O_EXCL, 0644, CAPACIDADE)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }

    for(int i = 0; i < QUANT_PORTAS; i++){
        char name[10];
        sprintf(name, "/s%d", i);
        if ((semPortas[i] = sem_open(name, O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED) {
            perror("Error in sem_open()");
            exit(1);
        }
    }

    for(int i = 0; i < PASSAGEIROS; i++){

        pid_t pid = fork();

        if(pid == -1){
            perror("Fork Falhou");
            exit(2);
        }

        if(pid == 0){
            srand(time(NULL) + getpid()); 

            printf("O passageiro %d chegou à estação A.\n", i);

            int entered = 0;
            while(!entered){
                for(int j = 0; j < QUANT_PORTAS; j++){
                    if(sem_trywait(semPortas[j]) == 0){
                        sem_wait(semCap);
                        printf("O passageiro %d entrou no comboio pela porta %d!\n", i, j);
                        entered = 1;
                        break;
                    }
                }
            }

            //ride the train
            if(i >= 15){
                sleep(5);
                printf("O passageiro %d saiu do comboio na estação B !\n", i);
                sem_post(semCap);
            }

            sem_post(semPortas[i % QUANT_PORTAS]);

            exit(0);
        }
    }

    for(int i = 0; i < PASSAGEIROS; i++){
        wait(NULL);
    }

    for(int i = 0; i < QUANT_PORTAS; i++){
        char name[10];
        sprintf(name, "/s%d", i);
        sem_unlink(name);
    }

    if (sem_unlink("/s112") == -1 ){
        perror("Error in sem_unlink()");
        exit(5);
    }

    return 0;
}
