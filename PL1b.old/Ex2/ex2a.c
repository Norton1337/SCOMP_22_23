#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>

int main() {
	for(;;){
		printf("I Love SCOMP!\n");
		sleep(1);
	}
}

//kill -19 pid do processo
//kill -18 pid do processo
