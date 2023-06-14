#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_EXAMS 300

typedef struct {
    int number;
    int g1Result;
    int g2Result;
    int g3Result;
    int finalGrade;
} Exam;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_t2_t3 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_t4 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_t5 = PTHREAD_COND_INITIALIZER;


Exam ArrayProva[MAX_EXAMS];
int count = 0;
int analyzed = 0;
int passedExams = 0;
int failedExams = 0;
int terminate = 0;

void *T1(void *arg) {
    int i;
    for (i = 0; i < MAX_EXAMS; i++) {
        pthread_mutex_lock(&mutex);
        ArrayProva[i].number = i;
        ArrayProva[i].g1Result = (rand() % 100) + 1;
        ArrayProva[i].g2Result = (rand() % 100) + 1;
        ArrayProva[i].g3Result = (rand() % 100) + 1;
        count++;

        pthread_cond_signal(&cond_t2_t3);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    terminate = 1;
    pthread_cond_signal(&cond_t2_t3);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *T2_T3(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        if (analyzed >= MAX_EXAMS) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        while (analyzed >= MAX_EXAMS) {
            pthread_cond_wait(&cond_t2_t3, &mutex);
        }
        
       

        ArrayProva[analyzed].finalGrade = (ArrayProva[analyzed].g1Result + ArrayProva[analyzed].g2Result + ArrayProva[analyzed].g3Result)/3;
        analyzed++;
        if (ArrayProva[analyzed].finalGrade >= 50) {
            pthread_cond_signal(&cond_t4);
            // printf("t4 analyzed exam %d\n", analyzed);
        } else {
            pthread_cond_signal(&cond_t5);
            printf("t5 analyzed exam %d\n", analyzed);
        }
        
        pthread_mutex_unlock(&mutex);
       
    }
}

void *T4(void *arg) {
    while (1) {
        printf("iujgrthe");
        pthread_mutex_lock(&mutex);
        while (analyzed <= MAX_EXAMS) {
            pthread_cond_wait(&cond_t4, &mutex);
        }
       
     
        
        
        passedExams++;
        printf("test");
        if (analyzed >= MAX_EXAMS) {
            printf("THE NOISE\n\n\n");
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
        


    }
}

void *T5(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (analyzed <= MAX_EXAMS) {
             pthread_cond_wait(&cond_t5, &mutex);
        }
       
     
        
        
        failedExams++;
        printf("test");
        if (analyzed >= MAX_EXAMS) {
            printf("THE NOISE\n\n\n");
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
        

    }
}


int main() {
    pthread_t t1, t2, t3, t4, t5;

    pthread_create(&t1, NULL, T1, NULL);
    pthread_create(&t2, NULL, T2_T3, NULL);
    pthread_create(&t3, NULL, T2_T3, NULL);
    pthread_create(&t4, NULL, T4, NULL);
    pthread_create(&t5, NULL, T5, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
    

    printf("Positive amount: %d\n",passedExams); 
    printf("Negative amount: %d\n",failedExams); 

    double posPercentage = (double) passedExams / 300 * 100;
    double negPercentage = (double) failedExams / 300 * 100;

    printf("Positive grades: %.2f%%\n", posPercentage);
    printf("Negative grades: %.2f%%\n", negPercentage);

    return 0;
}
