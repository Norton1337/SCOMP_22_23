/*

Parse command-line arguments for the number of children and the child execution time.
Get the start time using gettimeofday().

    For each child process:
        sleep x microseconds.
        exit(0).

Wait for all child to finish.
Get the end time using gettimeofday().
Calculate the elapsed time by subtracting start_time from end_time.
Print the elapsed time in microseconds if it is less than 0.1 seconds, otherwise print it in seconds.


Run testConfiguration script to test your program.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    int num_children, child_time;
    struct timeval start_time, end_time;
    
    

    if (argc != 3) {
        printf("Invalid number of arguments!\n");
        return 1;
    }

    num_children = atoi(argv[1]);
    child_time = atoi(argv[2]);

    gettimeofday(&start_time, NULL);

    for (int i = 0; i < num_children; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            printf("Fork error!\n");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            usleep(child_time);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < num_children; i++) {
        wait(NULL);
    }

    gettimeofday(&end_time, NULL);

    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1e6;
    if (elapsed_time < 0.1) {
        printf("Elapsed time: %.2f us\n", elapsed_time * 1e6);
    } else {
        printf("Elapsed time: %.2f s\n", elapsed_time);
    }

    return 0;
}
