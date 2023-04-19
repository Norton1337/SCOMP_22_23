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

#define shm_name "/shm_ex10"

typedef struct{
	int nums[10];
	int itemsLeft;
} ex10_type;

int main(void) {
	printf("Producing");

	int data_size = sizeof(ex10_type);
    int fd = shm_open(shm_name, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(shm_name, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
            if (fd < 0) {
                perror("shm_open");
                exit(1);
            }
            if (ftruncate(fd, data_size) < 0) {
                perror("ftruncate");
                exit(1);
            }
        } else {
            perror("shm_open");
            exit(1);
        }
    }
   ex10_type *sharedMemory = (ex10_type*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
    if (sharedMemory == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
	int pipe1[2];
	int pipe2[2];
	pid_t pid = fork();
	if (pid < 0) {
        perror("fork failed");
        exit(1);
        }
        
	int writing;
	printf("Producing");

	if(pid>0){
		//producer
		close(pipe1[0]);
		close(pipe2[1]);
		writing=0;
		int fd = shm_open(shm_name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
		ftruncate (fd, data_size);
		ex10_type *sharedMemory = (ex10_type*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
		sharedMemory->itemsLeft = 30;
		
		int newNumber=0;
		while(sharedMemory->itemsLeft>0){

			while(writing==1){

				read(pipe2[0],&writing,sizeof(int));
			}
			for(int i=0;i<10;i++){
				sharedMemory->nums[i]=newNumber;
				newNumber++;
				sharedMemory->itemsLeft--;
			}
			writing=1;
			
			write(pipe1[1],&writing,sizeof(int));

		}
		close(pipe1[1]);
		close(pipe2[0]);
		
	}else{
		//consumer
		close(pipe1[1]);
		close(pipe2[0]);
		writing = 0;
		int fd = shm_open(shm_name, O_RDWR, S_IRUSR|S_IWUSR);
		ftruncate (fd, data_size);
		ex10_type *sharedMemory = (ex10_type*) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
		while(1){
			while(writing!=1){
	
				read(pipe1[0],&writing,sizeof(int));
			}
			for(int i=0;i<10;i++){
				printf("%d\n",sharedMemory->nums[i]);
			}
			
			writing = 0;
			write(pipe2[1],&writing,sizeof(int));
			printf("printed 10 nums, %d remaining\n",sharedMemory->itemsLeft);
			if(sharedMemory->itemsLeft==0)
				break;
		}
		close(pipe1[0]);
		close(pipe2[1]);
		exit(1);
	}
		int status;
		wait(&status);

	
	
	shm_unlink(shm_name);

	return 0; 
}
