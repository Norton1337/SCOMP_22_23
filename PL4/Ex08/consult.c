#include "starter.h"

int main(){

    datarecord *memAddress = sharedMemory();
    sem_t *sem = semaphore();

    sem_wait(sem);

    printf("Por favor insira o número desejado: ") ;
    int numDesejado;
    scanf("%d", &numDesejado);

    datarecord record = *memAddress;
    int i = 0;
    while(record.filled != 0){
        i++;
        if(record.number == numDesejado){
            printf("Name: %s\nAddress: %s\nNumber: %d", record.name, record.address, record.number);
            break;
        }

        record = *(memAddress + i);
    }

    if(i == 0) printf("Não há nenhuma estrutura disponível.\n");
    sem_post(sem);

    return 0;
}