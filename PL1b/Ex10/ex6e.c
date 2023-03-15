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

void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}


int main(){
	
	sigset_t mask, pending;
	struct sigaction act;
	
	memset(&act,0,sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_handler = handle_SIGUSR1;
	act.sa_flags = 0;
	sigaction(SIGUSR1,&act,NULL);
		
	//sigemptyset(&mask);
	//sigaddset(&mask,SIGUSR1);
	//sigprocmask(SIG_BLOCK, &mask,0);
	/*SIGUSR1 is now blocked */
	
	pid_t p = fork();
	
	if(p == 0){
		int parent_pid=getppid();
		for(int i = 10; i< 22; i++){
			printf("Sou o filho\n");
			sleep(1);
			kill(parent_pid,i);
		}
	}
	
	for(;;){
		
		printf("Im working!\n");
		sleep(1);
		
	}
	
	return 0;
}
