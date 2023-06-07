#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 8
#define NUM_THREADS 10

int A[SIZE][SIZE];
int B[SIZE][SIZE];
int RESULT[SIZE][SIZE];

// Mutex for result matrix
pthread_mutex_t mutexes[SIZE][SIZE];

// Threads for generating matrices
pthread_t gen_threads[2];

// Threads for calculating
pthread_t calc_threads[SIZE];

void *fill_matrix(void *param) {
    int (*matrix)[SIZE] = param;

    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            matrix[i][j] = rand() % 10;
        }
    }
}

void *multiply_matrices(void *row) {
    int my_row = (int)row;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            pthread_mutex_lock(&mutexes[my_row][i]);
            RESULT[my_row][i] += A[my_row][j] * B[j][i];
            pthread_mutex_unlock(&mutexes[my_row][i]);
        }
    }
}

int main() {
    srand(time(NULL));
    
    // Initialize mutexes
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            pthread_mutex_init(&mutexes[i][j], NULL);
        }
    }

    // Create threads to generate matrices
    pthread_create(&gen_threads[0], NULL, fill_matrix, (void *)A);
    pthread_create(&gen_threads[1], NULL, fill_matrix, (void *)B);

    // Wait for matrix generation to complete
    pthread_join(gen_threads[0], NULL);
    pthread_join(gen_threads[1], NULL);

    // Create threads to calculate multiplication
    for(int i = 0; i < SIZE; i++) {
        pthread_create(&calc_threads[i], NULL, multiply_matrices, (void *)i);
    }

    // Wait for calculation to complete
    for(int i = 0; i < SIZE; i++) {
        pthread_join(calc_threads[i], NULL);
    }

    // Print resulting matrix
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            printf("%d ", RESULT[i][j]);
        }
        printf("\n");
    }

    // Destroy mutexes
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            pthread_mutex_destroy(&mutexes[i][j]);
        }
    }

    return 0;
}
