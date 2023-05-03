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
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
            if (fd < 0) {
                perror("shm_open");
                exit(1);
            }
            if (ftruncate(fd, string_type_size) < 0) {
                perror("ftruncate");
                exit(1);
            }
        } else {
            perror("shm_open");
            exit(1);
        }
    }


    string_type *stringType = mmap(NULL, string_type_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (stringType == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_t *semaphore = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 1);
    if (semaphore == SEM_FAILED) {
        if (errno == EEXIST) {
            semaphore = sem_open(SEM_NAME, 0);
        } else {
            perror("Error in sem_open()");
            exit(EXIT_FAILURE);
        }
    } else{
        for(int i = 0; i < STRING_AMOUNT; i++){
            strcpy(stringType->string[i], "\0");
        }
        stringType->stringAmount = 0;
    }


    while(stringType->stringAmount < STRING_AMOUNT){
        if(sem_wait(semaphore)==-1){
            perror("Error in sem_wait()\n");
            exit(EXIT_FAILURE);
        }
        if(stringType->stringAmount == STRING_AMOUNT){
            break;
        }
        sprintf(stringType->string[stringType->stringAmount], "I'm the Father - with PID %d", getpid());
        stringType->stringAmount++;
        
        time_t t;
        srand((int)time(&t) % getpid());
        
        sleep((rand() % 4)+1);

        if(sem_post(semaphore)==-1){
            perror("Error in sem_post()\n");
            exit(EXIT_FAILURE);
        }
    }
 

    if(sem_unlink(SEM_NAME)==-1){
		perror("Error in sem_unlink().\n");
	    exit(EXIT_FAILURE);
	}

    if (munmap(stringType, string_type_size) == -1) {
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

}