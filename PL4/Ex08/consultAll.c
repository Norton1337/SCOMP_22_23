#include "starter.h"

int main(){

    datarecord *memAddress = sharedMemory();
    sem_t *sem = semaphore();
    
    sem_wait(sem);

    int i = 0;
    datarecord record = *memAddress;
    while(record.filled != 0){
        i++;
        printf("Name: %s\nAddress: %s\nNumber: %d", record.name, record.address, record.number);
        printf("\n\n");

        record = *(memAddress + i);
    }

    if(i == 0) printf("Não há nenhuma estrutura disponível.\n");

    sem_post(sem);

    return 0;
}