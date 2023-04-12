#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>


#define shm_name "/shm_ex1"


typedef struct {
    int number;
    char name[100];
} student_type;

int main(int argc, char *argv[]) {
    int data_size = sizeof(student_type);
    int fd = shm_open(shm_name, O_RDWR,S_IRUSR|S_IWUSR);
    if (fd < 0) {
        perror("shm_open");
        exit(1);
    }
    ftruncate(fd, data_size);
    student_type *student = (student_type*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    bool data_received = false;
    while (!data_received) {
        if (student->number != 0) {
            printf("Number: %d\n", student->number);
            printf("Name: %s\n", student->name);
            data_received = true;
        }
        else
            sleep(1); // wait for 1 second before checking again
    }
    int r = munmap(student, data_size); /* disconnects */
    if (r < 0) printf("ERROR"); /* Check error */
    r = close(fd); /* closes */
    if (r < 0) printf("ERROR"); /* Check error*/
    
    if (shm_unlink(shm_name) < 0) {
        perror("shm_unlink");
        exit(1);
    }
    exit(0);
    return 0;
}
