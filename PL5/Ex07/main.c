// ”Totoloto” is a lottery game which encompasses two separate lotteries:
// • First is a pick-5 from 49 numbers (for our exercise only this part will be relevant);
// • Second is a pick-1 from 13 numbers.
// To play the game, the gambler must choose 5 numbers from 1 to 49 and 1 number from 1 to 13 by
// marking the numbered squares on a bet slip. Then the bet slip is registered by a lottery retailer (or
// agent) that enters the selection in the on-line terminal, to produce a game ticket.
// A draw will occur in which a ”key” is drawn. This ”key” is composed by 5 numbers from 1 to 49 as
// well as 1 number from 1 to 13.
// Implement a program that outputs statistical information concerning the first lottery (pick-5 from 49
// numbers). The expected result is the amount of times each one of the 49 number were drawn.
// Consider a database composed by 8000 ”keys”, being each ”key” a set of 5 numbers concerning the
// first lottery.
// The program should:
// • Create 16 threads;
// • For each thread perform a partial account of 500 “keys” (8000/16);
// • In the main thread print out the statistical values.
// Suggestion: Use an array of mutexes to ensure the data coherence


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LOTTERY_SIZE 8000
#define THREAD_COUNT 16

pthread_mutex_t mutex;

int values_count[49];

typedef struct {
    int values[5];
} LotteryValues;

LotteryValues lottery_values[LOTTERY_SIZE];

void *countValues(void *arg){
    pthread_mutex_lock(&mutex);
    int thread_id = (int) arg;

    int start = thread_id * (LOTTERY_SIZE / THREAD_COUNT);
    int end = start + (LOTTERY_SIZE / THREAD_COUNT);

    for(int i = start; i < end; i++){
        for(int j = 0; j < 5; j++){
            values_count[lottery_values[i].values[j] - 1]++;
        }
    }

    pthread_mutex_unlock(&mutex);
}

int main(){
    srand(time(NULL));
    pthread_t threads[THREAD_COUNT];
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < 49; i++){
        values_count[i] = 0;
    }

    for(int i = 0; i < LOTTERY_SIZE; i++){
        for(int j = 0; j < 5; j++){
            int value = rand() % 49 + 1;
            int isDifferent = 1;
            for(int k = 0; k < j; k++){
                if(value == lottery_values[i].values[k]){
                    isDifferent = 0;
                    break;
                }
            }
            if(isDifferent){
                lottery_values[i].values[j] = value;
            }else{
                j--;
            }
        }
    }

    for(int i = 0; i < THREAD_COUNT; i++){
        pthread_create(&threads[i], NULL, (void *) countValues, (void *) i);
    }

    for(int i = 0; i < THREAD_COUNT; i++){
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    for(int i = 0; i < 49; i++){
        printf("Number %d was drawn %d times\n", i + 1, values_count[i]);
    }


}