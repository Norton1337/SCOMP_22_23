#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    pid_t pid;
    int signal;
    printf("What is the PID of the process: ");
    scanf("%d", &pid);
    printf("What is the ID of the Signal: ");
    scanf("%d", &signal);
    kill(pid, signal);
    return 0;
}