/*
Write a program that when the SIGUSR1 signal is received the message
“SIGUSR1 has been captured and was sent by the process with PID XX”
where XX is the process PID which sent the SIGUSR1 signal.
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void handle_USR1(int signum) {
    write(STDOUT_FILENO, "SIGUSR1 has been captured and was sent by the process with PID\n", 60);
    exit(0);
}


int main() {
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_sigaction = handle_USR1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    sleep(20);
    return 0;
}