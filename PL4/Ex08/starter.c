#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <string.h>

typedef struct{
    char address[50];
    char name[30];
    int number;
    int filled;
} datarecord;

datarecord *memAddress;
sem_t *sem;
int fd, size;

datarecord* sharedMemory(int option){
    // ########################### SHARED MEMORY ################################

    size = sizeof(datarecord) * 100;
    
    fd = shm_open("/ex10", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);

    if(fd < 0) perror("Error when creating shared memory file.\n");

    if (ftruncate (fd, size) < 0) perror("Error when executing ftruncate() function.\n");
    
    memAddress = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    return memAddress;

    // ###########################################################################
}

sem_t* semaphore(){
    // ############################### Semaphores ################################

    if ((sem = sem_open("10", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open function\n");
        exit(1);
    }

    return sem;

    // ###########################################################################
}

void closeSharedComponents(){
    fd = munmap(memAddress, size);
	fd = shm_unlink("/ex10");
	close(fd);
    
    sem_close(sem);
    sem_unlink("10");
}