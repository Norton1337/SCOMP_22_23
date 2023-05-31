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
#include <pthread.h>

#define NUMBER_ELEMENTS 5

typedef struct {
    int number;
    char name[10];
    int grade;
} vec_element;

 void print_vector_element(vec_element* element){
    printf("Number: %d\n", element->number);
    printf("Name: %s\n", element->name);
    printf("Grade: %d\n", element->grade);
    printf("\n");
 }

int main() {
    vec_element element1 = {1,"one",1};
    vec_element element2 = {2,"two",2};
    vec_element element3 = {3,"three",3};
    vec_element element4 = {4,"four",4};
    vec_element element5 = {5,"five",5};
    vec_element vec[5] = {element1, element2,element3,element4,element5};


    print_vector_element(vec);

    pthread_t threads[NUMBER_ELEMENTS];
    
    for(int i = 0; i < NUMBER_ELEMENTS; i++){
        pthread_create(&threads[i], NULL, print_vector_element, &vec[i]);
    }

    for(int i = 0; i < NUMBER_ELEMENTS; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;


}