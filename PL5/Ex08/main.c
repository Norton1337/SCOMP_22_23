#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1000
#define THREAD_COUNT 5
#define VALUES_PER_THREAD (ARRAY_SIZE / THREAD_COUNT)

int data[ARRAY_SIZE];
int result[ARRAY_SIZE];
pthread_mutex_t lock;

void *calculate_partial(void *arg) {
    int thread_num = *(int *)arg;
    int start_index = thread_num * VALUES_PER_THREAD;
    int end_index = start_index + VALUES_PER_THREAD;

    for (int i = start_index; i < end_index; i++) {
        result[i] = data[i] * 10 + 2;
    }

    //controll access to the shared resource
    pthread_mutex_lock(&lock);
    printf("Thread %d calculated values %d to %d\n", thread_num, start_index, end_index - 1);
    for (int i = start_index; i < end_index; i++) {
        printf("result[%d] = %d\n", i, result[i]);
    }
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int thread_nums[THREAD_COUNT];

    for (int i = 0; i < ARRAY_SIZE; i++) {
        data[i] = rand() % 100;
    }

    pthread_mutex_init(&lock, NULL);
    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_nums[i] = i;
        pthread_create(&threads[i], NULL, calculate_partial, &thread_nums[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // mutual exclusion
    pthread_mutex_destroy(&lock);

    return 0;
}
