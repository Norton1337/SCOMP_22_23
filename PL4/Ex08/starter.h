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

datarecord* sharedMemory();

sem_t* semaphore();

void closeSharedComponents();