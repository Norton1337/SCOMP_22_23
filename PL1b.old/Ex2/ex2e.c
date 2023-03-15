#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>

int main() {
	pid_t target_pid;
    int signal_number;
    
    printf("Pid do processo:");
    scanf("%d\n", &target_pid);
    
    printf("Qual o número do sinal?");
    scanf("%d\n", &signal_number);
    
    //Kill(2) tem target pid e signal number
    if(kill(target_pid,signal_number)==0){
		printf("Sinal enviado.");
	}else{
		printf("Sinal NÃO enviado, aprende a programar");
	}
	
	return 0;
    
}
