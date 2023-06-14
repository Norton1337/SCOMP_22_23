#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h> // For intptr_t

#define SIZE 1000
#define NUM_PRODUCTS 5
#define NUM_HYPERMARKET 3
#define NUM_THREADS 6

typedef struct {
    int id_h;
    int id_p;
    int p;
} product;

product vec[SIZE];
int vec1[SIZE], vec2[SIZE], vec3[SIZE];
pthread_t threads[NUM_THREADS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int counter = 0;
float averages[NUM_HYPERMARKET];

void* filter(void* arg) {
    intptr_t id = (intptr_t)arg;
    int counter = 0;
    for(int i = 0; i < SIZE; i++){
        if(vec[i].id_h == id){
            if(vec[i].id_p == 1){
                vec1[counter] = vec[i].p;
            } else if(vec[i].id_p == 2){
                vec2[counter] = vec[i].p;
            } else if(vec[i].id_p == 3){
                vec3[counter] = vec[i].p;
            }
            counter++;
        }
    }

    pthread_mutex_lock(&mutex);
    counter++;
    if (counter == NUM_HYPERMARKET) {
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* compute(void* arg){
    pthread_mutex_lock(&mutex);
    while(counter < NUM_HYPERMARKET){
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    intptr_t id_h = (intptr_t)arg;
    int* vec_aux;
    if(id_h == 1){
        vec_aux = vec1;
    } else if(id_h == 2){
        vec_aux = vec2;
    } else if(id_h == 3){
        vec_aux = vec3;
    }

    int sum = 0;
    for(int i = 0; i < SIZE; i++){
        sum += vec_aux[i];
    }
    averages[id_h-1] = sum/NUM_PRODUCTS;
    return NULL;
}

void present(){
    printf("here");
    float min_avg = averages[0];
    int min_id_h = 1;
    for(int i = 0; i < NUM_HYPERMARKET; i++){
        if(averages[i] < min_avg){
            min_avg = averages[i];
            min_id_h = i+1;
        }
    }
    printf("Hypermarket with the lowest average product price: %d (average price: %.2f)\n", min_id_h, min_avg);
}

int main(){
    srand(time(NULL));
    for(int i = 0; i < SIZE; i++){
        //random values for id_h between 1 and 3 because of vecs1,2,3
        vec[i].id_h = (rand() % NUM_HYPERMARKET) + 1;
        vec[i].id_p = (rand() % NUM_PRODUCTS) + 1;
        vec[i].p = (rand() % 100) + 1;
    }

    for(int i =0 ; i< NUM_HYPERMARKET; i++){
        pthread_create(&threads[i], NULL, filter, (void*)(intptr_t)(i+1));
    }

    for(int i = NUM_HYPERMARKET; i< NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, compute, (void*)(intptr_t)(i+1-NUM_HYPERMARKET));
    }

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    present();

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
