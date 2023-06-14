#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h> // For intptr_t

#define SIZE 3000
#define NUM_PRODUCTS 5
#define NUM_HYPERMARKET 3
#define NUM_THREADS 6

typedef struct {
    int id_h;
    int id_p;
    int p;
} product;

product vec[SIZE];
product vec1[SIZE], vec2[SIZE], vec3[SIZE];
pthread_t threads[NUM_THREADS];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int counter1 = 0;
int counter2 = 0;
int counter3 = 0;
float averages[NUM_HYPERMARKET];
int filterFinished=0;

int globalSum = 0;
int globalLowerH=0;

void* filter(void* arg) {
    intptr_t id = (intptr_t)arg;
    for(int i = 0 + 1000*id; i < (id*1000)+1000; i++){

        if(vec[i].id_h == 1){
                pthread_mutex_lock(&mutex1);
                vec1[counter1] = vec[i];
                counter1++;
                pthread_mutex_unlock(&mutex1);
        } else if(vec[i].id_h == 2){
                pthread_mutex_lock(&mutex2);
                vec2[counter2] = vec[i];
                counter2++;
                pthread_mutex_unlock(&mutex2);
        } else if(vec[i].id_h == 3){
                pthread_mutex_lock(&mutex3);
                vec3[counter3] = vec[i];
                counter3++;
                pthread_mutex_unlock(&mutex3);
        }

    }

    pthread_mutex_lock(&mutex);
    filterFinished++;
    pthread_cond_broadcast(&cond);

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* compute(void* arg){
    
    pthread_mutex_lock(&mutex);
    while(filterFinished < NUM_HYPERMARKET){
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    intptr_t id = (intptr_t)arg;
    int product[5] = {0,0,0,0,0};
    int productCount[5] = {0,0,0,0,0};

    if(id == 1){
        for(int i = 0; i < counter1; i++){
            if(vec1[i].id_p == 1){
                product[0] += vec1[i].p;
                productCount[0]++;
            } else if(vec1[i].id_p == 2){
                product[1] += vec1[i].p;
                productCount[1]++;
            } else if(vec1[i].id_p == 3){
                product[2] += vec1[i].p;
                productCount[2]++;
            } else if(vec1[i].id_p == 4){
                product[3] += vec1[i].p;
                productCount[3]++;
            } else if(vec1[i].id_p == 5){
                product[4] += vec1[i].p;
                productCount[4]++;
            }
        }
    
    } else if(id == 2){
        for(int i = 0; i < counter2; i++){
            if(vec2[i].id_p == 1){
                product[0] += vec2[i].p;
                productCount[0]++;
            } else if(vec2[i].id_p == 2){
                product[1] += vec2[i].p;
                productCount[1]++;
            } else if(vec2[i].id_p == 3){
                product[2] += vec2[i].p;
                productCount[2]++;
            } else if(vec2[i].id_p == 4){
                product[3] += vec2[i].p;
                productCount[3]++;
            } else if(vec2[i].id_p == 5){
                product[4] += vec2[i].p;
                productCount[4]++;
            }
        }
    } else if(id == 3){
        for(int i = 0; i < counter3; i++){
            if(vec3[i].id_p == 1){
                product[0] += vec3[i].p;
                productCount[0]++;
            } else if(vec3[i].id_p == 2){
                product[1] += vec3[i].p;
                productCount[1]++;
            } else if(vec3[i].id_p == 3){
                product[2] += vec3[i].p;
                productCount[2]++;
            } else if(vec3[i].id_p == 4){
                product[3] += vec3[i].p;
                productCount[3]++;
            } else if(vec3[i].id_p == 5){
                product[4] += vec3[i].p;
                productCount[4]++;
            }
        }
    }

    for(int i = 0; i < NUM_PRODUCTS; i++){
        product[i] = product[i]/productCount[i];
    }
    
    int sum = 0;
    for(int i = 0; i < NUM_PRODUCTS; i++){
        sum += product[i];
    }

    pthread_mutex_lock(&mutex);
    if(globalSum==0){
        globalSum = sum;
        globalLowerH = id;
    } else if(sum < globalSum){
        globalSum = sum;
        globalLowerH = id;
    }
    pthread_mutex_unlock(&mutex);

    // pthread_exit(sum);
    return NULL;
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
        pthread_create(&threads[i], NULL, filter, (void*)(intptr_t)(i));
    }

    for(int i = NUM_HYPERMARKET; i< NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, compute, (void*)(intptr_t)(i+1-NUM_HYPERMARKET));
    }

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("The lowest average product price is: %d and it's from hypermarket nº%d\n", globalSum, globalLowerH);
 


    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
