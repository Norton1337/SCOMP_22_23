#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
 pid_t p;

 if (fork() == 0) {
 printf("PID = %d\n", getpid()); sleep(10);exit(0);
 }

 if ((p=fork()) == 0) {
 printf("PID = %d\n", getpid());sleep(10); exit(0);
 }

 printf("Parent PID = %d\n", getpid());

 printf("Waiting... (for PID=%d)\n",p);
 waitpid(p, NULL, 0); 
 

 printf("Enter Loop...\n");
 while (1); /* Infinite loop */
} 


// O processo da linha 11 é um zombie chamado defunct 
