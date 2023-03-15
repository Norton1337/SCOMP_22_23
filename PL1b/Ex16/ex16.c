#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_WORKERS 50
#define SUCCESS_PROB 0.01 // Probability of success for simulate1()

int num_finished = 0;
int num_success = 0;

void handle_signal(int sig) {
    if (sig == SIGUSR1) {
        num_success++;
    }
    num_finished++;
}

int simulate1() {
    srand(time(NULL));
    int result = rand() % 100; // Generate a random number between 0 and 99
    if (result < SUCCESS_PROB * 100) {
        return 1; // Success
    } else {
        return 0; // Failure
    }
}

void simulate2() {
    srand(time(NULL));
    int result = rand() % 100; // Generate a random number between 0 and 99
    printf("simulate2() returned %d\n", result);
}

int main() {
    pid_t workers[NUM_WORKERS];

    // Create workers
    for (int i = 0; i < NUM_WORKERS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Worker process
            int success = simulate1();
            if (success) {
                kill(getppid(), SIGUSR1); // Notify parent process of success
            } else {
                kill(getppid(), SIGUSR2); // Notify parent process of failure
            }
            pause(); // Wait for signal from parent before starting simulate2()
            simulate2();
            exit(0);
        } else {
            // Parent process
            workers[i] = pid;
        }
    }

    // Set signal handlers
    signal(SIGUSR1, handle_signal);
    signal(SIGUSR2, handle_signal);

    // Wait for workers to finish
    while (num_finished < NUM_WORKERS) {
        pause();
        if (num_finished == 25) {
            if (num_success == 0) {
                printf("Inefficient algorithm!\n");
                for (int i = 0; i < NUM_WORKERS; i++) {
                    kill(workers[i], SIGTERM); // Terminate all children
                }
                exit(0);
            } else {
                for (int i = 0; i < NUM_WORKERS; i++) {
                    kill(workers[i], SIGUSR1); // Signal all workers to start simulate2()
                }
            }
        }
    }

    // Wait for workers to finish simulate2()
    for (int i = 0; i < NUM_WORKERS; i++) {
        wait(NULL);
    }

    return 0;
}
