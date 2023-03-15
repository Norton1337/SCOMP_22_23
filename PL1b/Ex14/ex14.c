#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <stddef.h>
#include <string.h>

void handle_ALARM(int signo){
	char text[]= "Too slow dumbo\n";
	write(STDOUT_FILENO, text,strlen(text));
}
int main(){

	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask); /* No signals blocked */
	act.sa_handler = handle_ALARM;
	sigaction(SIGALRM, &act, NULL);

	char str[100];
	kill(SIGALRM,handle_ALARM);
	printf("write a string within 10 seconds.");
	alarm(10);
	fgets(str,100,stdin);
	alarm(0);
	printf("The size of the string is: %ld\n", strlen(str)-1);
	

	return 0;
}
