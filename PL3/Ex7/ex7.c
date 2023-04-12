#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define shm_name "/shm_ex5"

#define STR_SIZE 50
#define NR_DISC 10
typedef struct aluno{
    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
    int in_creation;
} aluno_type;


int main() {
    int fd = shm_open(shm_name, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0) {
        if (errno == ENOENT) {
            printf("Shared memory object does not exist, creating it...\n");
            fd = shm_open(shm_name, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
            if (fd < 0) {
                perror("shm_open");
                exit(1);
            }
            if (ftruncate(fd, sizeof(aluno_type)) < 0) {
                perror("ftruncate");
                exit(1);
            }
        } else {
            perror("shm_open");
            exit(1);
        }
    }
    aluno_type *aluno = mmap(NULL, sizeof(aluno_type), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (aluno == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    printf("Enter student number: ");
    int number;
    if (scanf("%d", &number) != 1) {
        printf("Invalid input\n");
        exit(1);
    }
    getchar();
    aluno->numero = number;
    printf("Enter student name: ");
    fgets(aluno->nome, sizeof(aluno->nome), stdin);

    srand(time(NULL));

    for(int i=0; i<NR_DISC;i++){
        aluno->disciplinas[i] = rand() % 20 + 1;
    }

    aluno->in_creation=0;

    printf("Student number: %d\n",aluno->numero);
    printf("Student Name: %s\n",aluno->nome);
    printf("Student grades: \n");
    for(int i=0;i<NR_DISC;i++){
        printf("    Disc[%d]: %d\n",i,aluno->disciplinas[i]);
    }

   
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        //Filho 1

        while(aluno->in_creation!=0);
        int lowest=aluno->disciplinas[0];
        int highest=aluno->disciplinas[0];
        for(int i=0;i<NR_DISC;i++){
            if(aluno->disciplinas[i]>highest)
                highest=aluno->disciplinas[i];
            if(aluno->disciplinas[i]<lowest)
                lowest=aluno->disciplinas[i];
        }
        printf("Highest grade: %d\n",highest);
        printf("Lowest grade: %d\n",lowest);
        exit(EXIT_SUCCESS);
    } 
   
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        //Filho 2

        while(aluno->in_creation!=0);
        int media=0;
        for(int i=0;i<NR_DISC;i++){
            media+=aluno->disciplinas[i];
        }
        media = media/NR_DISC;
        printf("Average grade: %d\n",media);
        exit(EXIT_SUCCESS);
    } 

    waitpid(pid1,NULL,0);
    waitpid(pid2,NULL,0);


    if (munmap(aluno, sizeof(int)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    if (fd >= 0 && close(fd) < 0) {
        perror("close");
        exit(1);
    }


    if (shm_unlink(shm_name) < 0) {
        perror("shm_unlink");
        exit(1);
    }

    return 0;
}
