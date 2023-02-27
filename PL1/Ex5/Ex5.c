#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int status;
    pid_t pid1, pid2;
    pid1 = fork();
    if(pid1 == -1){
        perror("Failed Fork");
        exit(1);
    }
    if (pid1 == 0) {
        sleep(1);
        exit(1);
    }
    pid2 = fork();
    if(pid2 == -1){
        perror("Failed Fork");
        exit(1);
    }
    if (pid2 == 0) {
        sleep(2);
        exit(2);
    }
    waitpid(pid1, &status, 0);
    printf("Child 1[%d] exited with status %d\n",pid1, WEXITSTATUS(status));
    waitpid(pid2, &status, 0);
    printf("Child 2[%d] exited with status %d\n",pid2, WEXITSTATUS(status));
    return 0;
}