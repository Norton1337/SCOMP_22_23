// Implement a program that generates statistics of all bank customers balances. All balance values are
// stored in an array of 1000 values. The program should:
// • Create 3 threads;
// • The first thread searches for the lowest balance and stores it in a global variable;
// • The second thread searches for the highest balance and stores it in a global variable;
// • The third thread computes the average balance and stores it in a global variable;
// • The main thread, using the global variables, prints out all the results stored by each threads

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BALANCE_SIZE 1000
#define THREAD_COUNT 3

int balance_values[BALANCE_SIZE];




void *searchLowest(void *arg){


    int lowest = balance_values[0];
    for(int i = 0; i < BALANCE_SIZE; i++){
        if(balance_values[i] < lowest){
            lowest = balance_values[i];
        }
    }
    
    pthread_exit(lowest);

}

void *searchHighest(void *arg){

    int highest = balance_values[0];
    for(int i = 0; i < BALANCE_SIZE; i++){
        if(balance_values[i] > highest){
            highest = balance_values[i];
        }
    }
    pthread_exit(highest);

}

void *searchAverage(void *arg){

    int sum = 0;
    for(int i = 0; i < BALANCE_SIZE; i++){
        sum += balance_values[i];
    }
    pthread_exit((sum / BALANCE_SIZE));

}

int main(){
    srand(time(NULL));
    pthread_t threads[THREAD_COUNT];
    for(int i = 0; i < BALANCE_SIZE; i++){
        balance_values[i] = (rand() % 9000) + 1000;
    }

    //proving lowest value
    int lowest = balance_values[0];
    for(int i = 0; i < BALANCE_SIZE; i++){
        if(balance_values[i] < lowest){
            lowest = balance_values[i];
        }
    }
    printf("Lowest value: %d\n", lowest);

    //proving highest value
    int highest = balance_values[0];
    for(int i = 0; i < BALANCE_SIZE; i++){
        if(balance_values[i] > highest){
            highest = balance_values[i];
        }
    }

    printf("Highest value: %d\n", highest);

    //proving average value
    int sum= 0;
    for(int i = 0; i < BALANCE_SIZE; i++){
        sum += balance_values[i];
    }
    printf("Average value: %d\n", (sum / BALANCE_SIZE));

    printf("Threads:\n");


    void *thread_result;

    pthread_create(&threads[0], NULL, searchLowest, NULL);
    pthread_create(&threads[1], NULL, searchHighest, NULL);
    pthread_create(&threads[2], NULL, searchAverage, NULL);


    for(int i = 0; i < THREAD_COUNT; i++){
        pthread_join(threads[i], &thread_result);
        if(thread_result != NULL){
            printf("Thread %d returned %ld\n", i, (long)thread_result);
        }
    }


    return 0;
}