#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

typedef struct{
	int num1;
	int num2;
	int round;
} sharedMemorytype;

int main(int argc, char *argv[]) {
	pid_t a;
	int fd;
	int data_size = sizeof(sharedMemorytype);


	fd = shm_open("/ex5", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	
	if (fd == -1){
		perror("Error in shm_open().\n");
		return 1;
	}

	ftruncate(fd, data_size);

	

	sharedMemorytype *sharedMemory=(sharedMemorytype *)mmap(NULL,data_size,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	sharedMemory->num1=1000;
	sharedMemory->num2=500;
	sharedMemory->round=0;

	
	a = fork();
	printf("Num1: %d\n", sharedMemory->num1);
	printf("Num2: %d\n", sharedMemory->num2);
	if(a>0){
		while(sharedMemory->round<1000000){
			sharedMemory->num--;
			sharedMemory->num2++;
			sharedMemory->round++;	
		}
		
		
	 }else{
		while(sharedMemory->round<1000000){
			sharedMemory->num1++;
			sharedMemory->num2--;
			printf("%d\n",sharedMemory->num2);
			sharedMemory->round++;
		}
		exit(1);
	}
	int status;
	wait(&status);
	printf("Num1: %d\n", sharedMemory->num1);
	printf("Num2: %d\n", sharedMemory->num2);

	munmap(sharedMemory, data_size);
	shm_unlink("/ex5");
	
	return 0; 
}

/*
 * These result might change, we can't control which process is the last 
 * to alter the shared memory
 * 
 */
