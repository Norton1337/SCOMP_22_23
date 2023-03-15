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

/* a) he usual terminal line discipline assigns control characters to the SIGINT SIGKILL and SIGTSTP
signals:
• CTRL-C sends SIGINT;
• CTRL-\ sends SIGQUIT;
• CTRL-Z sends SIGTSTP.
Confirm these associations typing the command stty -a.


Resultado Consola:
intr = ^C; quit = ^\; erase = ^?; kill = ^U; eof = ^D; eol = <undef>;
eol2 = <undef>; swtch = <undef>; start = ^Q; stop = ^S; susp = ^Z; rprnt = ^R;
werase = ^W; lnext = ^V; discard = ^O;
*/


/* b) Run the program. Once the program is executing, press CTRL-C.
Explain what happened, considering the reception of the signal and the action performed.

Ctrl+C para o programa, ele envia um sinal SIGINT para o processo, que interrompe a execução do programa.

*/