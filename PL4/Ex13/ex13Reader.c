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

#define SHM_NAME "/shm_ex13"
#define SEM_NAME_WRITER "/sem_ex13_writer"
#define SEM_NAME_READER "/sem_ex13_reader"

typedef struct {
    char text[100];
    int readerAmount;
    int writerAmount;
} sentenceBuf;

int main(int argc, char *argv[]){
    int numReaders = 1;
    if(argc > 1){
        numReaders = atoi(argv[1]);
    }

    int sentence_buffer_size = sizeof(sentenceBuf);
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        perror("shm_open");
        exit(1);
    }

    sem_t *semaphore_writer = sem_open(SEM_NAME_WRITER, 0);
    if (semaphore_writer == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }
    sem_t *semaphore_reader = sem_open(SEM_NAME_READER, 0);
    if (semaphore_reader == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

    sentenceBuf *sentence = mmap(NULL, sentence_buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sentence == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    for(int i=0;i<numReaders;i++){
        pid_t pid = fork();
        if(pid == -1){
            perror("Error in fork()");
            exit(EXIT_FAILURE);
        }
        else if(pid == 0){
            sem_wait(semaphore_writer);
            sem_post(semaphore_writer);

            sem_wait(semaphore_reader);
            sentence->readerAmount++;
            sem_post(semaphore_reader);

            sleep(1);
            printf("Reader [%d]: %s\t There are %d readers\n", getpid(), sentence->text, sentence->readerAmount);

            
            sem_wait(semaphore_reader);
            sentence->readerAmount--;
            sem_post(semaphore_reader);

            exit(EXIT_SUCCESS);
        }
        
        
    }

    for(int i=0;i<numReaders;i++){
        wait(NULL);
    }
 
    
    
    if (munmap(sentence,sentence_buffer_size ) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    return 0;
}