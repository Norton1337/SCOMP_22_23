/* b) Change the program so that each time a SIGINT signal is received the “I
won’t let the process end with CTRL-C!” message is displayed. Recompile
and run the program and test by pressing several times CTRL-C.

*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void handle_INT(int signum) {
    char text[] = "I won't let the process end with CTRL-C!\n";
    write(STDOUT_FILENO, text, strlen(text));

}


int main() {
    struct sigaction act;
    sigset_t mask;

    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_handler = handle_INT;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);

    for(;;){
        printf("I Like Signal\n");
        sleep(1);
    }
}


/* c) On the terminal where the process was launched in b. press the CTRL-\
keys. What happened? For the explanation bear in mind the reception of
a signal and action performed

Ctrl + \ faz com que o processo seja terminado, o sinal SIGQUIT é enviado para o processo, que termina a execução do programa.

*/