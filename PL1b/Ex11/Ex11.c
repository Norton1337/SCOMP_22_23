/* Using sigprocmask(), sigismember() and sigfillset() functions implement a function that lists all
signals blocked when a process receives the SIGUSR1 signal. Are all signals blocked? Justify your answer
by mentioning those which are not eventually blocked.
Suggestion: use NULL as the value for the second parameter of the sigprocmask function.


*/
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <stddef.h>
#include <string.h>

void handle_SIGUSR1(){
	sigset_t mask;
	sigprocmask(0, NULL, &mask);
	
	for (int sig = 1; sig < NSIG; sig++) {
		if (sigismember(&mask, sig)) {
			printf("Signal %d (%s) is blocked.\n", sig, strsignal(sig));
		}
	}
}

int main(){
	
	sigset_t mask, pending;
	struct sigaction act;
	
	memset(&act,0,sizeof(struct sigaction));
	sigfillset(&act.sa_mask);
	act.sa_handler = handle_SIGUSR1;
	act.sa_flags = 0;
	sigaction(SIGUSR1,&act,NULL);
		

	
	for(;;){
		
		printf("Im working!\n");
		sleep(1);
		
	}
	
	return 0;
}
