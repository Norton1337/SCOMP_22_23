/* d) Change the program so that each time a SIGQUIT signal is received the “I
won’t let the process end by pressing CTRL-\!” message is displayed.
Recompile and run the program and test by pressing several times
CTRL-\.
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void handle_INTQUIT(int signum) {
    char text[200];
    if(signum == SIGINT)
        strcat(text, "I won't let the process end with CTRL-C!\n");
    else if(signum == SIGQUIT)
        strcat(text, "I won't let the process end by pressing CTRL-\\!\n");

    
    write(STDOUT_FILENO, text, strlen(text));

}


int main() {
    struct sigaction act;
    sigset_t mask;

    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_handler = handle_INTQUIT;
    act.sa_flags = 0;
    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGINT, &act, NULL);

    for(;;){
        printf("I Like Signal\n");
        sleep(1);
    }
}
/* e) On another terminal send, by using the kill command a SIGINT and a
SIGQUIT signal to the process created in d. What happened?

Escrevem-se as mensagens "I won't let the process end with CTRL-C!" e "I won't let the process end by pressing CTRL-\\!".
Tal como pressionar CTRL-C e CTRL-\ 

*/


/* f) Press CTRL-Z to suspend the process. Access the process table to check
the state of the process as well as its PID. Use the kill command to kill the
suspended process.
Sugestion: the “kill %1” can be used. Execute the “jobs” command. What
is the meaning of “kill %1”?

kill %1 envia um sinal SIGTERM para o processo com o "job id" 1, o comando jobs mostra os processos a serem executados no terminal.
O primeiro processo da lista jobs é terminado com "kill %1"


*/

/* g) On the signal handlers instead of printf the write function should be
used. Why is that?

write é mais eficiente que printf, pois não é necessário fazer o buffering do output.
write é assincrono, enquanto que printf é sincrono, e no handler só é recomendado usar funções assincronas.

 
*/