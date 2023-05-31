#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1000
#define THREAD_COUNT 10
#define SEARCH_SIZE (ARRAY_SIZE / THREAD_COUNT)

int array[ARRAY_SIZE];

typedef struct {
    int thread_id;
    int search_number;
} ThreadInfo;


void *search(void *arg){
    ThreadInfo *thread_info = (ThreadInfo *) arg;
    int start = thread_info->thread_id * SEARCH_SIZE;
    int end = start + SEARCH_SIZE;
    int search_number = thread_info->search_number;

    for(int i = start; i < end; i++){
        if(array[i] == search_number){
            printf("Found number %d at position %d\n", search_number, i);
            pthread_exit((thread_info->thread_id)+1);
        }
    }

    pthread_exit(NULL);
}


int main(){
    pthread_t threads[THREAD_COUNT];
    ThreadInfo thread_info[THREAD_COUNT];
    int search_number;
    void *thread_result;

    for(int i = 0; i < ARRAY_SIZE; i++){
        array[i] = i;
    }

    printf("Number to search: ");
    scanf("%d", &search_number);

    for(int i = 0; i < THREAD_COUNT; i++){
        thread_info[i].thread_id = i;
        thread_info[i].search_number = search_number;
        pthread_create(&threads[i], NULL, search, (void *) &thread_info[i]);
    }

    for(int i = 0; i < THREAD_COUNT; i++){
        pthread_join(threads[i], &thread_result);
        if(thread_result != NULL){
            printf("Thread %d returned %ld\n", i, (long)thread_result);
            break;
        }
    }

    return 0;
}