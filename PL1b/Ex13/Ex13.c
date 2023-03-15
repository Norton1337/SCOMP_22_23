/* Write a program that creates a child process.
• Workflow of the parent process.
    The parent executes task A for 3 seconds.
    Once task A finishes, the parent displays on the screen the message “Task A: done!” and notifies
    the child sending it a SIGUSR1 signal.
    Finally, the parent waits for the child to complete, to display the message “Job is complete!”.

• Workflow of the child process.
    The child executes task B for a random amount of time, between 1 to 5 seconds.
    Once task B completes, the child displays on the screen the message “Task B: done!”.
    Once tasks A and B are complete, the child will execute task C for 1 second.
    When task C is complete, the child displays the message “Task C: done!” and exits.
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>

int main(){

    pid_t pid;


    pid = fork();

    if(pid==0){ //child

        //task B
        time_t t;
	
        srand((unsigned) time (&t));
        
        int num = (rand() % 5) + 1;
        printf("Waiting for %d seconds\n", num);
        sleep(num);
        printf("Task B: done!\n");

        //task C
        sleep(1);
        printf("Task C: done!\n");
        exit(0);
    } else { //parent
            
            //task A
            sleep(3);
            printf("Task A: done!\n");
            
            printf("Notifying child\n");
            kill(pid, 10);


    
            
            //wait for child
            int status;
            waitpid(pid,&status,0);
            printf("Job is complete!\n");

    }

}