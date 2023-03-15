#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    for(;;){
        printf("I Like Signal\n");
        sleep(1);
    }
}

/* a) On a terminal compile and run the program. On the same terminal press
CTRL-C. What happened? For the explanation bear in mind the reception
of a signal and action performed.

Ctrl+C para o programa, ele envia um sinal SIGINT para o processo, que interrompe a execução do programa.

*/