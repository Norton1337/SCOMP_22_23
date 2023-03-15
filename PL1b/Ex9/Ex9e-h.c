/* e) Change the program so that each time a SIGQUIT signal is received the “I won’t let the process
end by pressing CTRL-\!” message is displayed.
Recompile and run the program and test by pressing several times CTRL-\.
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
/* f) Open another terminal and send a SIGINT signal to the target process using the kill command.
What happened?

Escrevem-se as mensagens "I won't let the process end with CTRL-C!". Tal como pressionar CTRL-C. 

*/

/* e) This time, send a SIGQUIT signal to the target process, instead. What happened?
Aparece a mensagem: "I won't let the process end by pressing CTRL-\\!". Tal como pressionar CTRL-\.
*/


/* h) Start the program again and perform as follows.
    i. Press CTRL-Z to suspend the process.
    ii. Access the process table with command ps to check the state of the process as well as its PID.
    iii. Execute the jobs command and check the job number (between square brackets) of the process.
    iv. Type the kill %1 command to terminate the suspended process.

What is the meaning of %1 in this last command?

kill %1 envia um sinal SIGTERM para o processo com o "job id" 1, o comando jobs mostra os processos a serem executados no terminal.
O primeiro processo da lista jobs é terminado com "kill %1"


*/
