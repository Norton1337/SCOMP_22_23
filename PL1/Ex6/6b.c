#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void main(){
	int i;
	int status;
	
	 
	for (i = 0; i < 4; i++) {
		if (fork() == 0) { 
			break;/*sleep(): unistd.h*/
		}
	}

	printf("This is the end.\n");
}
