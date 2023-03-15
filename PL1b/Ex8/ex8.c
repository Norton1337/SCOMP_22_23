#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <stddef.h>
#include <string.h>


void signalHandler(int signalNum, siginfo_t *siginfo, void *context){
		printf("Caught SIGUSR1 signal sent by the process with PID %d\n", siginfo->si_pid);
	
}

int main(){
	
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signalHandler;
	sigemptyset(&sa.sa_mask);
	
	if(sigaction(SIGUSR1, &sa, NULL)== -1){
		perror("sigaction");
		return 1;
	}
	
	printf("PID = %d\n", getpid());
	
	while(1){
		sleep(1);
	}
	
	return 0;
}
