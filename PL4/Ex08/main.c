#include "starter.h"

void runFile(char *fileName){
    pid_t p = fork();
    if (p == 0) {
        execlp(fileName, fileName, (char *)NULL);
        exit(0);
    }
    wait(NULL);
}

int mainMenu(){
    int opcaoSelecionada = -1;
    while(opcaoSelecionada != 0){
        printf("Menu de Opcões:\n\n1.Consult\n2.Insert\n3.Consult All\n0.Sair\n");
        printf("\nPlease Select an Option: ");
        scanf("%d", &opcaoSelecionada);

        if(opcaoSelecionada >= 0 && opcaoSelecionada < 4)
            return opcaoSelecionada;
        else
            printf("\n\nOpção inválida.\n\n\n");
    }
    return -1;
}

int main(){
    
    sharedMemory();
    semaphore();
    
    int option = -1;
    while(1 && option != 0){
        option = mainMenu();
        
        printf("\n\n\n");
        
        switch(option){
            case 1:
                runFile("./consult.o");
                break;

            case 2:
                runFile("./insert.o");
                break;

            case 3:
                runFile("./consultAll.o");
                break;
            
            default:
                printf("\n\nAplicação Encerrada\n\n");
                break;
        }
        printf("\n\n\n");
    }

    closeSharedComponents();
    
    return 0;
}