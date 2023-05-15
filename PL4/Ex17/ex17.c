/*

Create shared memory and semaphores for synchronization.
Initialize data structure in shared memory.
Create sensor processes.

    Sensors:
        Generate random measurements and write in the shared memory.
        If value is greater than 50, increment alarm counter.
        If sensor is alarmed, and the last 2 measurements are less than 50, decrement alarm counter.
        Repeat 6 times.

    Controller:
        Wait for a semaphore to indicate that a measurement occured
        Print the measurement and increment the counter.
        Check for any changes in the alarm status and print if it did change.
        Repeat until all measurements have been processed.

Wait for sensor processes to finish.
Clean up shared memory and semaphores.

*/


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define SENSOR_AMOUNT 5
#define TOTAL_REPETITIONS 6

#define SHM_NAME "/shm_ex19"
#define SEM_NAME "/sem_ex19"

typedef struct {
	int measurement[SENSOR_AMOUNT][TOTAL_REPETITIONS];
	int sensorsAlarm;
} ex19_type;

int main(int argc, char *argv[]) {
	
    int data_size = sizeof(ex19_type);
    int fd = shm_open(SHM_NAME, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
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

    sem_t *semaphores[SENSOR_AMOUNT];
    sem_t *semaphores_v2[SENSOR_AMOUNT];
    char sem_name[20];
    for(int i=0; i<SENSOR_AMOUNT;i++){
        sprintf(sem_name, "%s_%d", SEM_NAME, i);
        if ((semaphores[i] = sem_open(sem_name, O_CREAT, 0644,0)) == SEM_FAILED) {
            perror("Error in sem_open()");
            exit(1);
        }
        sprintf(sem_name, "%s_%d_v2", SEM_NAME, i);
        if ((semaphores_v2[i] = sem_open(sem_name, O_CREAT, 0644,0)) == SEM_FAILED) {
            perror("Error in sem_open()");
            exit(1);
        }
    }

    sem_t *can_write;
    
    if ((can_write = sem_open(SEM_NAME, O_CREAT, 0644,1)) == SEM_FAILED) {
            perror("Error in sem_open()");
            exit(1);
        } 
    
    ex19_type *data = (ex19_type *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == NULL) {
        perror("Error mapping the object");
        exit(EXIT_FAILURE);
    }
    sem_wait(can_write);
    data->sensorsAlarm=0;
    sem_post(can_write);

    for(int i=0;i<SENSOR_AMOUNT;i++){
        if(sem_post(semaphores[i])==-1){
            perror("sem_post");
            exit(1);
        }
    }

    pid_t pid;
    for(int i = 0; i < SENSOR_AMOUNT; i++) {
        pid = fork();
        if(pid == -1) {
            perror("fork");
            exit(1);
        }
        if(pid==0){
            //Sensors

            srand(time(NULL) ^ (getpid()<<16));
            int isAlarmed=0;
            for(int j=0;j<TOTAL_REPETITIONS;j++){
                if(sem_wait(semaphores[i])==-1){
                    perror("sem_wait");
                    exit(1);
                }
                int result = rand()%101;
                if(sem_wait(can_write)==-1){
                    perror("sem_wait");
                    exit(1);
                }
                if(result>50 && !isAlarmed){
                    isAlarmed=1;
                    data->sensorsAlarm++;
                } else if(j>0 && data->measurement[i][j-1]>50 && isAlarmed){
                    isAlarmed=0;
                    data->sensorsAlarm--;
                }
                
                data->measurement[i][j]=result;

                if(sem_post(can_write)==-1){
                    perror("sem_post");
                    exit(1);
                }
                if(sem_post(semaphores[i])==-1){
                    perror("sem_post");
                    exit(1);
                }
                if(sem_post(semaphores_v2[i])==-1){
                    perror("sem_post");
                    exit(1);
                }
                sleep(1);
            }

            exit(EXIT_SUCCESS);
        }
    }


    //Controller 
    int counter[TOTAL_REPETITIONS] = {0,0,0,0,0,0};
    int count=0;
    int i = 0;
    int lastAlarm=0;
    while(count<SENSOR_AMOUNT*TOTAL_REPETITIONS){
        
        if(counter[i]<6 && sem_trywait(semaphores_v2[i])==0){
            
            
            printf("Sensor [%d] - measurment nº %d -> %d\n",i,counter[i], data->measurement[i][counter[i]]);
            fflush(stdout);
            count++;
            counter[i]++;


        }
        i++;
        if(i==SENSOR_AMOUNT)
            i=0;
        if(data->sensorsAlarm!=lastAlarm){
            printf("Alarm: %d\n",data->sensorsAlarm);
            fflush(stdout);
            lastAlarm=data->sensorsAlarm;
        }
        
    }

    for(int i=0;i<SENSOR_AMOUNT;i++){
        wait(NULL);
    }

    printf("All sensors finished\n");

    for(int i=0;i<SENSOR_AMOUNT;i++){
        if(sem_close(semaphores[i])==-1){
            perror("sem_close");
            exit(1);
        }
        if(sem_close(semaphores_v2[i])==-1){
            perror("sem_close");
            exit(1);
        }
    }

    if(sem_close(can_write)==-1){
        perror("sem_close");
        exit(1);
    }

    if (munmap(data, data_size) < 0) {
        perror("Error un-mmapping the object");
        exit(EXIT_FAILURE);
    }

    if (close(fd) < 0) {
        perror("Error closing the object");
        exit(EXIT_FAILURE);
    }

    if (shm_unlink(SHM_NAME) < 0) {
        perror("Error unlinking the object");
        exit(EXIT_FAILURE);
    }


	
    return 0;
}