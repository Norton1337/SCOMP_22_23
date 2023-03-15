#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <stddef.h>
#include <string.h>

int USR1_COUNTER = 0;
handle_SIGUSR1(){
	USR1_COUNTER++;
	printf("SIGUSR1 signal captured, USR1_COUNTER = %d\n", USR1_COUNTER);
}

int main(){
	
	sigset_t mask, pending;
	struct sigaction act;
	
	memset(&act,0,sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_handler = handle_SIGUSR1;
	act.sa_flags = 0;
	sigaction(SIGUSR1,&act,NULL);
		
	/*sigemptyset(&mask);
	sigaddset(&mask,SIGUSR1);
	sigprocmask(SIG_BLOCK, &mask,0);
	/*SIGUSR1 is now blocked */
	
	for(;;){
		
		printf("Im working!\n");
		sleep(1);
		
	}
	
	return 0;
}
