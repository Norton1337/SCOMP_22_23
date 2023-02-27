#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int i;
    for(i = 0; i<10; i++){
        pid_t pid = fork();
        if(pid == -1){
            perror("Failed Fork");
            exit(1);
        }
        if(pid == 0){
            int j;
            for(j = 0; j<100; j++){
                printf("%d\n", i*100+j);
            }
            exit(0);
        }
    }
    int status;
    for(i = 0; i<10; i++){
        waitpid(-1, &status, 0);
    }
    return 0;
}

/*
a) O output está organizado, mas não se pode garantir que este será sempre o caso. Isto porque o sistema operativo é que define
 que processo é executado em cada momento. Devido a isto, o output pode variar de uma execução para outra.
*/