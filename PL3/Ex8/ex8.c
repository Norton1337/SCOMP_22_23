#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define CHILD_AMOUNT 10


typedef struct{
	char path[CHILD_AMOUNT][100];
	char word[CHILD_AMOUNT][100];
	int occurrences[CHILD_AMOUNT];
} ex8_type;

int main(void) {
	int data_size = sizeof(ex8_type);


	int fd = shm_open("/ex8", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);


	ftruncate (fd, data_size);
	

	ex8_type *sharedMemory = (ex8_type*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	
	for(int i = 0; i < CHILD_AMOUNT; i++){
		printf("\nChild nº %d\n",i);
		printf("	File Path: ");
		scanf("%s", sharedMemory->path[i]);
		
		printf("	Word to search: ");
		scanf("%s", sharedMemory->word[i]);
		
	
	}

	pid_t pid;
	
	for(int i = 0; i < CHILD_AMOUNT; i++){
		pid = fork();

		if (pid == 0){

			FILE *file = fopen(sharedMemory->path[i], "r");
			
			if(file != NULL){

				char line[100];

				char *pos;
				int index;
				sharedMemory->occurrences[i] = 0;

				while(fgets(line, 100, file) != NULL){
					index = 0;

					while((pos = strstr(line + index, sharedMemory->word[i])) != NULL){

						index = (pos - line) + 1;
						sharedMemory->occurrences[i]++;
					}	
				}
			} else {
				printf("File not found.\n");
				exit(0);
			}
			
			exit(0);
		}
	}
	
	int status;
	for(int i = 0; i < CHILD_AMOUNT; i++){
		wait(&status);
	}

	for(int i = 0; i < CHILD_AMOUNT; i++){
		printf("Child [%d] - The word %s had %d occurences.\n",i,sharedMemory->word[i],sharedMemory->occurrences[i]);
	} 

	shm_unlink("/ex8");

	return 0; 
}
