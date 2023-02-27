#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 1000
int main (int argc, char *argv[]){
    float result[ARRAY_SIZE]; 
    int numbers[ARRAY_SIZE];
    int n; 
    time_t t; 
    int i;

    srand ((unsigned) time (&t));

    for (i = 0; i < ARRAY_SIZE; i++)
        numbers[i] = rand () % 255;
   
    n = rand () % 1000;
    for(i = 0; i<5; i++){
        pid_t pid = fork();
        if(pid == -1){
            perror("Failed Fork");
            exit(1);
        }
        if(pid == 0){
            float maxNum=0;
            for(int j = i*200; j<(i+1)*200; j++){
                if(numbers[j] > maxNum)
                    maxNum = (float)numbers[j];
            }
            exit(maxNum);
        }
    }

    int status;
    float maxNum=0;
    for(i = 0; i<5; i++){
        waitpid(-1, &status, 0);
        if(WEXITSTATUS(status) > maxNum)
            maxNum = (float)WEXITSTATUS(status);
    }


    pid_t pid1 = fork();
    if(pid1 == -1){
            perror("Failed Fork");
            exit(1);
        }
    if(pid1 == 0){
        for(i = 0; i<ARRAY_SIZE/2; i++){
            result[i] = ((int)numbers[i]/maxNum)*100;
        }
        for(i = 0; i<ARRAY_SIZE/2; i++){
            printf("result[%d] = %f\n", i, result[i]);
        }
        exit(0);
    }
    for(i = ARRAY_SIZE/2; i<ARRAY_SIZE; i++){
        result[i] = ((int) numbers[i]/maxNum)*100;
    }
    waitpid(pid1, &status, 0);
    for(i = ARRAY_SIZE/2; i<ARRAY_SIZE; i++){
        printf("result[%d] = %f\n", i, result[i]);
    }
    return 0;
}