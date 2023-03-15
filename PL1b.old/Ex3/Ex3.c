/* a) Which is most suitable to handle signals? Justify your choice.

Sigaction é melhor, oferece mais controlo sobre o que fazer quando o sinal é recebido.

*/

/* b) Document with detail all the parameters of sigaction as well as its 
possible values

 signum -> numero do sinal, pode ser um numero inteiro ou um sinal
 sigaction act -> estrutura que contem a informação sobre o sinal
    sa_handler -> apontador para uma função que é chamada quando o sinal é recebido
    sa_sigaction -> apontador para uma função que é chamada quando o sinal é recebido
    sa_mask -> conjunto de sinais que são bloqueados durante a execução da função
    sa_flags -> flags que controlam o comportamento do sinal
 sigaction oldact -> estrutura que contem a informação sobre o sinal anterior

*/

/* c) Write a program that when the SIGUSR1 signal is received the message
“SIGUSR1 has been captured and was sent by the process with PID XX”
where XX is the process PID which sent the SIGUSR1 signal.
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void handle_USR1(int signum,siginfo_t *info) {
    pid_t pid = info->si_pid;
    char text[] = "SIGUSR1 has been captured and was sent by the process with PID: ";
    char pid_str[10];
    sprintf(pid_str, "%d", pid);
    strcat(text, pid_str);
    strcat(text, "\n");
    write(STDOUT_FILENO, text, strlen(text));
    exit(0);
}


int main() {
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_sigaction = handle_USR1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    printf("PID: %d\n", getpid());
    sleep(20);
    return 0;
}


/* d) On the function code which handles the signal is it possible to use global
or static variables safely? Justify your answer.

Não, porque o comportamento é indefinido quando o sinal é recebido, podendo ser que o sinal seja recebido durante a execução da função, o que pode causar erros.

*/

/* e)  Which functions can be called in an adequate manner on a signal
handler? (Sugestion: “man signal-safety”)

Apenas funçoes assincronas seguras podem ser usadas em funções que lidam com sinais, como por exemplo:

    write
    getpid
    exit
    sigaction
*/