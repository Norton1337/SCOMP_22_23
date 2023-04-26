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


#define STRING_AMOUNT 50
#define STRING_SIZE 80
#define SHM_NAME "/shm_ex3"
#define SEM_NAME "/sem_ex3"

typedef struct {
    int stringAmount;
    char string[STRING_AMOUNT][STRING_SIZE];
} string_type;

int main(){

    int string_type_size = sizeof(string_type);
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        perror("Shared memory object does not exist\n");
        exit(EXIT_FAILURE);
    }


    string_type *stringType = mmap(NULL, string_type_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (stringType == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_t *semaphore = sem_open(SEM_NAME, 0);
    if (semaphore == SEM_FAILED) {
        perror("Semaphore does not exist");
        exit(EXIT_FAILURE);
    }


    if(sem_wait(semaphore)==-1){
        perror("Error in sem_wait()\n");
        exit(EXIT_FAILURE);
    }
    int total=0;
    int amount=0;
    for(int i=0; i<STRING_AMOUNT; i++){
        if(stringType->string[i][0] != '\0'){
            printf("%s\n", stringType->string[i]);
            amount++;
        }
        total++;
            
    }
    
    printf("%d / %d String are filled\n", amount, total);

    if(sem_post(semaphore)==-1){
        perror("Error in sem_post()\n");
        exit(EXIT_FAILURE);
    }

    if (munmap(stringType, string_type_size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

}