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
#define SEM_NAME_BUFFER "/sem_ex13_buffer"

typedef struct {
    char text[100];
    int readerAmount;
    int writerAmount;
} sentenceBuf;

int main(int argc, char *argv[]){
    int numWriters = 1;
    if(argc > 1){
        numWriters = atoi(argv[1]);
    }

    int sentence_buffer_size = sizeof(sentenceBuf);
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        perror("shm_open");
        exit(1);
    }

    sem_t *semaphore_writer = sem_open(SEM_NAME_WRITER, 0);
    if (semaphore_writer == SEM_FAILED) {
        perror("Error in sem_open() writer");
        exit(EXIT_FAILURE);
    }
    sem_t *semaphore_buffer = sem_open(SEM_NAME_BUFFER, 0);
    if (semaphore_buffer == SEM_FAILED) {
        perror("Error in sem_open() buffer");
        exit(EXIT_FAILURE);
    }

    sentenceBuf *sentence = mmap(NULL, sentence_buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sentence == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    for(int i=0;i<numWriters;i++){
        pid_t pid = fork();
        if(pid == -1){
            perror("Error in fork()");
            exit(EXIT_FAILURE);
        }
        if(pid == 0){
            if(sentence->writerAmount == 0){
                sem_wait(semaphore_writer);
            }
            sem_wait(semaphore_buffer);
            sentence->writerAmount++;
            sem_post(semaphore_buffer);
            fflush(stdout);
            sleep(i);
            sem_wait(semaphore_buffer);
            
            
            
            
            time_t t = time(NULL);
            time(&t);
            sprintf(sentence->text, "Writer [%d]: The current time is %s", getpid(), ctime(&t));
            printf("Writer [%d]: There are %d readers and %d writers\n", getpid(), sentence->readerAmount, sentence->writerAmount);
            fflush(stdout);
            
            sentence->writerAmount--;

            if(sentence->writerAmount == 0){
               sem_post(semaphore_writer);
            }
            sem_post(semaphore_buffer);
            exit(EXIT_SUCCESS);
        }
    }
    for(int i=0;i<numWriters;i++){
        wait(NULL);
    }
    

    return 0;
}