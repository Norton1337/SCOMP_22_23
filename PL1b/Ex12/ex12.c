#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <stddef.h>
#include <string.h>


volatile sig_atomic_t num_children = 5;

void child_handler(int signum){
	char text[]= "here \n";
	if(signum == SIGUSR1){
		
		write(STDOUT_FILENO,text,strlen(text));
		num_children--;
	}
}

void print_sequence(int start, int end) {
    for (int i = start; i < end; i++) {
        printf("%d\n", i);
    }
}

int main(){
	pid_t pid[5];
	
	struct sigaction act;
	act.sa_handler = child_handler; //sig_ign para ignorar sigchild prevenir criação zombies
	act.sa_flags = SA_NOCLDWAIT | SA_NOCLDSTOP; //ignorar sianis dos filhos quando pararem e n transformar em zombies
	sigaction(SIGUSR1, &act, NULL);
	
	
	
	for(int i = 0; i< 5; i++){
		pid[i] = fork();
		
		if(pid[i] == 0){
			sleep(i+1);
			int start = i*200;
			int end = (i+5)*200;
			print_sequence(start,end);
			kill(getppid(),SIGUSR1);
			exit(EXIT_SUCCESS);
		}
	}
	

	 while (num_children > 0) {
        pause();
    }
    
    //espera todos filhos acabem
    for(int i = 0; i < 5; i++){
		waitpid(pid[i], NULL,0);
    }
    
    return 0;    
	
}
