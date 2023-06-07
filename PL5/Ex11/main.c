#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define EXAME_AMOUNT 300
#define THREAD_COUNT 5

pthread_cond_t cond_var;
pthread_cond_t cond_t4;
pthread_cond_t cond_t5;

pthread_mutex_t mutex;
pthread_mutex_t mutex_t4;
pthread_mutex_t mutex_t5;

typedef struct{
    int number;
    int g1Result;
    int g2Result;
    int g3Result;
    int finalGrade;
} Exam;

Exam exams[EXAME_AMOUNT];
int examCounter = 0;
int passedExams = 0;
int failedExams = 0;

void *t1(void *arg){
    srand(time(NULL));
    for(int i = 0; i < EXAME_AMOUNT; i++){
        exams[i].number = i;
        exams[i].g1Result = (rand() % 100) + 1;
        exams[i].g2Result = (rand() % 100) + 1;
        exams[i].g3Result = (rand() % 100) + 1;
        exams[i].finalGrade = (exams[i].g1Result + exams[i].g2Result + exams[i].g3Result)/3;
        pthread_mutex_lock(&mutex);
        examCounter++;
        
        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(1);   
}

void *t2T3(void *arg){
    while(examCounter < 300){
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond_var, &mutex);
        
        // printf("Exam %d: %d\n", exams[i].number, exams[i].finalGrade);
        if(exams[examCounter].finalGrade >= 50){
            pthread_cond_signal(&cond_t4);    
        }else{
            pthread_cond_signal(&cond_t5);
        }
        pthread_mutex_unlock(&mutex);


    }


    pthread_exit(23);

}

void *t4(void *arg){
    while(passedExams + failedExams < 300){
        pthread_mutex_lock(&mutex_t4);
        pthread_cond_wait(&cond_t4, &mutex_t4);
        passedExams++;
        pthread_mutex_unlock(&mutex_t4);
        
    }

    pthread_exit(4);

}

void *t5(void *arg){
    while(passedExams + failedExams < 300){
        pthread_mutex_lock(&mutex_t5);
        pthread_cond_wait(&cond_t5, &mutex_t5);
        failedExams++;
        pthread_mutex_unlock(&mutex_t5);
        
    }

    pthread_exit(5);
}


int main(){
    
    pthread_t threads[THREAD_COUNT];
    pthread_cond_init(&cond_var, NULL);
    pthread_cond_init(&cond_t4, NULL);
    pthread_cond_init(&cond_t5, NULL);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_t4, NULL);
    pthread_mutex_init(&mutex_t5, NULL);

    pthread_create(&threads[0], NULL, (void *) t1, NULL);
    pthread_create(&threads[1], NULL, (void *) t2T3, NULL);
    pthread_create(&threads[2], NULL, (void *) t2T3, NULL);
    pthread_create(&threads[3], NULL, (void *) t4, NULL);
    pthread_create(&threads[4], NULL, (void *) t5, NULL);

    void *thread_result;

    for(int i = 0; i < THREAD_COUNT; i++){
        pthread_join(threads[i], &thread_result);
        printf("Thread %d returned %d\n", i, (int) thread_result);
        if(i=2){
            printf("Passed exams: %d\n", passedExams);
            printf("Failed exams: %d\n", failedExams);
        }
    }


    pthread_cond_destroy(&cond_var);
    pthread_cond_destroy(&cond_t4);
    pthread_cond_destroy(&cond_t5);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_t4);
    pthread_mutex_destroy(&mutex_t5);

    return 0;

}