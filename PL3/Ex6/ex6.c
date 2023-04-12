#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#define ARRAY_SIZE 1000
#define LOCAL_MAX_SIZE 10
#define ARRAY_CHUNK_SIZE 100

typedef struct {
    int local_max[LOCAL_MAX_SIZE];
} shared_memory_t;

void find_local_max(int arr[], int start, int end, int local_max_arr[], int process_id) {
    int local_max = arr[start];
    for (int i = start; i <= end; i++) {
        if (arr[i] > local_max) {
            local_max = arr[i];
        }
    }
    local_max_arr[process_id] = local_max;
}

int main() {
    pid_t pid, wpid;
    int status;
    int array[ARRAY_SIZE];
    shared_memory_t *shared_memory;
    int shm_fd;

    // initialize array with random values
    srand(time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 1000;
    }

    // create shared memory area
    shm_fd = shm_open("/shared_memory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(shm_fd, sizeof(shared_memory_t)) == -1) {
        perror("ftruncate");
        exit(1);
    }
    shared_memory = (shared_memory_t *) mmap(NULL, sizeof(shared_memory_t),
                                             PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // create 10 child processes
    for (int i = 0; i < LOCAL_MAX_SIZE; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            find_local_max(array, i * ARRAY_CHUNK_SIZE, (i + 1) * ARRAY_CHUNK_SIZE - 1,
                            shared_memory->local_max, i);
            exit(0);
        }
    }

    // wait for child processes to finish
    while ((wpid == wait(&status)) > 0);

    // determine global maximum
    int global_max = shared_memory->local_max[0];
    int global_max_pos = -1;
    for (int i = 1; i < LOCAL_MAX_SIZE; i++) {
        if (shared_memory->local_max[i] > global_max) {
            global_max = shared_memory->local_max[i];
            global_max_pos = i;
        }
    }
    printf("Global maximum: %d\n", global_max);
    printf("Position of global maximum : %d\n", global_max_pos);

    // clean up shared memory area
    if (munmap(shared_memory, sizeof(shared_memory_t)) == -1) {
        perror("munmap");
        exit(1);
    }
    if (shm_unlink("/shared_memory") == -1) {
        perror("shm_unlink");
        exit(1);
    }

    return 0;
}
