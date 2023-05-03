#include "starter.h"

int main(){

    datarecord *memAddress = sharedMemory();
    sem_t *sem = semaphore();
    
    sem_wait(sem);
    
    printf("Número: ") ;
    int num;
    scanf("%d", &num);

    printf("Nome: ");
    char nome[30];
    scanf("%s", nome);

    printf("Morada: ");
    char address[50];
    scanf("%s", address);

    datarecord record; 
    record.number = num;
    strcpy(record.name, nome);
    strcpy(record.address, address);
    record.filled = 1;

    int i = 0;
    datarecord auxRecord = *(memAddress + i);
    while(auxRecord.filled != 0){
        i++;
        auxRecord = *(memAddress + i);
    } 
    *(memAddress + i) = record;
        
    sem_post(sem);

    return 0;
}