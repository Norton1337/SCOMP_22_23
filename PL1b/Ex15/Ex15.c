/* Consider an array of commands in which each position has the command name and the time limit (in
seconds) to complete. This is expressed in the defined type command ̇t:
1 typedef struct –
2 char cmd [32]; // Command name .
3 int time ̇cap ; // Time limit to complete ( in seconds ) .
4  ̋ command ̇t ; 
A command is terminated if it exceeds its time limit; additionally, the message “The command XX
did not complete in its allowed time!” (where XX is the command name) is displayed.
Implement a program that sequentially executes all the commands in the array, enforcing the time limits.
Suggestion: In order to test the program, you can develop another program that (1) display its name
on the screen, (2) sleeps for 10 seconds and finally (3) present the message “Execution ended!”.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

typedef struct {
    char cmd[32];
    int time_cap;
} command_t;

command_t *current_cmd;

void handle_timeout(int sig) {
    printf("The command %s did not complete in its allowed time!\n", current_cmd->cmd);
    exit(1);
}



int main() {
    command_t commands[] = {{"Ex15 Helper1", 5}, {"Ex15 Helper2", 8}, {"Ex15 Helper3", 3}};
    int num_commands = sizeof(commands) / sizeof(command_t);

    for (int i = 0; i < num_commands; i++) {
        current_cmd = &commands[i];
        printf("Executing command %s...\n", current_cmd->cmd);

        pid_t pid = fork();
        if (pid == 0) { // child process
            signal(SIGALRM, handle_timeout);
            alarm(current_cmd->time_cap);
            execl(current_cmd->cmd, current_cmd->cmd, (char*) NULL);
            exit(0);
        } else { // parent process
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                int exit_status = WEXITSTATUS(status);
                if (exit_status != 0) {
                    printf("The Command '%s' failed with exit status %d\n", current_cmd->cmd, exit_status);
                } else {
                    printf("The Command '%s' completed successfully\n", current_cmd->cmd);
                }
            } else {
                printf("The command '%s' did not complete in its allowed time!\n", current_cmd->cmd);
            }
        }
    }

    printf("All commands completed\n");
    return 0;
}
