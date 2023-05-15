/*
    Controller process:
        Create shared memory and semaphores for synchronization.
        Initialize data structure in shared memory.
        Reset shared memory.

    For each Writer process:
        Connect to shared memory and semaphores.
        Wait for a semaphore to indicate that the shared memory is available.
        Increment the writer counter.
        Write a sentence in the shared memory.
        Decrement the writer counter.
        Post a semaphore to indicate that the shared memory is available.

    For each Reader process:
        Connect to shared memory and semaphores.
        Wait for writer semaphore to indicate that the shared memory is available and post right after.
        Wait for reader semaphore.
        Increment the reader counter.
        Post reader semaphore.
        Read a sentence from the shared memory.
        Wait for reader semaphore.
        Decrement the reader counter.
        Post reader semaphore.


*/

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

int main(){



    int sentence_buffer_size = sizeof(sentenceBuf);
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
            if (fd < 0) {
                perror("shm_open");
                exit(1);
            }
            if (ftruncate(fd, sentence_buffer_size) < 0) {
                perror("ftruncate");
                exit(1);
            }
        } else {
            perror("shm_open");
            exit(1);
        }
    }

    sem_unlink(SEM_NAME_WRITER);
    sem_unlink(SEM_NAME_READER);
    sem_t *semaphore_writer = sem_open(SEM_NAME_WRITER, O_CREAT | O_EXCL, 0644, 1);
    if (semaphore_writer == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }
    
    sem_t *semaphore_reader = sem_open(SEM_NAME_READER, O_CREAT | O_EXCL, 0644, 1);
    if (semaphore_reader == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(EXIT_FAILURE);
    }

    sentenceBuf *sentence = mmap(NULL, sentence_buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sentence == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    sem_wait(semaphore_writer);
    sem_wait(semaphore_reader);
    sentence->readerAmount=0;
    sentence->writerAmount=0;
    strcpy(sentence->text, "");
    sem_post(semaphore_writer);
    sem_post(semaphore_reader);


}