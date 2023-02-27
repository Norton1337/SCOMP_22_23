#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 1000
int main (int argc, char *argv[]){
    int numbers[ARRAY_SIZE]; /* array to lookup */
    int n; /* the number to find */
    time_t t; /* needed to init. the random number generator (RNG)
    */
    int i;
    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand ((unsigned) time (&t));
    /* initialize array with random numbers (rand(): stdlib.h) */
    for (i = 0; i < ARRAY_SIZE; i++)
        numbers[i] = rand () % 1000;
    /* initialize n */
    n = rand () % 1000;
    int childNAmount=0;
    int parenteNAmount=0;
    pid_t pid1 = fork();
    if(pid1 == -1){
        perror("Failed Fork");
        exit(1);
    }
    if(pid1 == 0){
        for(i = 0; i<ARRAY_SIZE/2; i++){
            if(numbers[i] == n)
                childNAmount++;
        }
        exit(childNAmount);
    }
    for(i = ARRAY_SIZE/2; i<ARRAY_SIZE; i++){
        if(numbers[i] == n)
            parenteNAmount++;
    }
    int status;
    waitpid(pid1, &status, 0);
    int total = parenteNAmount + WEXITSTATUS(status);
    printf("N was found %d times, %d times by child, %d times by parent\n", total, WEXITSTATUS(status), parenteNAmount);
    return 0;
}