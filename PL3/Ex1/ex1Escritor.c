#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

typedef struct {
    int number;
    char name[100];
} student_type;

int main(int argc, char *argv[]) {
    int data_size = sizeof(student_type);
    int fd = shm_open("/ex1", O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open("/ex1", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
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

    student_type *student = mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (student == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    printf("Enter student number: ");
    int number;
    if (scanf("%d", &number) != 1) {
        printf("Invalid input\n");
        exit(1);
    }
    getchar(); // consume newline character
    student->number = number;

    printf("Enter student name: ");
    fgets(student->name, sizeof(student->name), stdin);
    student->name[strcspn(student->name, "\n")] = 0; // remove trailing newline character
    printf("Student number: %d\n", student->number);
    printf("Student name: %s\n", student->name);

    if (munmap(student, data_size) < 0) {
        perror("munmap");
        exit(1);
    }
    if (fd >= 0 && close(fd) < 0) {
        perror("close");
        exit(1);
    }

    return 0;
}
