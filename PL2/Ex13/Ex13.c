#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>
#include <time.h>

#define NUM_PIPES 4

int main(){

    int fdArray[NUM_PIPES][2];

    for(int i = 0; i< NUM_PIPES;i++){
        if(pipe(fdArray[i]) == -1){
            perror("Pipe failed");
            return 1;
        }
    }

    int sleepTime=0;

    /* Machine M1
        Machine M1 cuts 5 pieces, 5 pieces at a time, transfers the pieces to M2 and notifies M2;
    */
    if(fork()==0){
        for(int i = 0;i <NUM_PIPES;i++){
            if(i==0)
                close(fdArray[i][0]);
            else{
                close(fdArray[i][0]);
                close(fdArray[i][1]);
            }
        }

        int piecePile = 1000;
		while(piecePile!=0){
			
			printf("Cutting 5 pieces!\n");
			piecePile-=5;
			int newPieces = 5;
			write(fdArray[0][1],&newPieces,sizeof(int));
			sleep(sleepTime);
		}
		piecePile=-1;
		write(fdArray[0][1],&piecePile,sizeof(int));



        close(fdArray[0][1]);
        exit(1);
    }

    /* Machine M2
        Machine M2, folds the pieces, 5 at a time, transfers the pieces to M3 and notifies M3;
    */
    if(fork()==0){
        for(int i = 0;i <NUM_PIPES;i++){
            if(i==0)
                close(fdArray[i][1]);
            else if(i==1)
                close(fdArray[i][0]);
            else{
                close(fdArray[i][0]);
                close(fdArray[i][1]);
            }
        }


        int pieces;

		while(pieces!=-1){
			
			read(fdArray[0][0],&pieces,sizeof(int));
			if(pieces==-1){
				write(fdArray[1][1],&pieces,sizeof(int));
				break;
			}
			printf("Folding 5 pieces!\n");
			write(fdArray[1][1],&pieces,sizeof(int));
			sleep(sleepTime);
		}




        for(int i = 0;i <NUM_PIPES;i++){
            if(i==0)
                close(fdArray[i][0]);
            else if(i==1)
                close(fdArray[i][1]);
        }

        exit(1);
    }
    
     /* Machine M3
        Machine M3 welds the pieces, 10 pieces at a time, transfers the pieces to machine M4 and notifies M4;
    */
    if(fork()==0){
        for(int i = 0;i <NUM_PIPES;i++){
            if(i==1)
                close(fdArray[i][1]);
            else if(i==2)
                close(fdArray[i][0]);
            else{
                close(fdArray[i][0]);
                close(fdArray[i][1]);
            }
        }


        int pieces;
		int piecesHere = 0;
		while(pieces!=-1){
			
			read(fdArray[1][0],&pieces,sizeof(int));
			if(pieces==-1){
				write(fdArray[2][1],&pieces,sizeof(int));
				break;
			}
			piecesHere+=pieces;
			if(piecesHere==10){
				printf("Welding 10 pieces!\n");
				
				write(fdArray[2][1],&piecesHere,sizeof(int));
				piecesHere-=10;
				sleep(sleepTime);
			}
			
		}
		





        for(int i = 0;i <NUM_PIPES;i++){
            if(i==1)
                close(fdArray[i][0]);
            else if(i==2)
                close(fdArray[i][1]);
        }

        exit(1);
    }

     /* Machine M4
        Machine M4 packs 100 pieces at a time, transfer them to storage A1 and adds the produced parts to the inventory
    */
    if(fork()==0){
        for(int i = 0;i <NUM_PIPES;i++){
            if(i==2)
                close(fdArray[i][1]);
            else if(i==3)
                close(fdArray[i][0]);
            else{
                close(fdArray[i][0]);
                close(fdArray[i][1]);
            }
        }



		int pieces;
		int piecesHere = 0;
		while(pieces!=-1){
			
			read(fdArray[2][0],&pieces,sizeof(int));
			
			if(pieces==-1){
				write(fdArray[3][1],&pieces,sizeof(int));
				break;
			}
			
			piecesHere+=pieces;
			if(piecesHere==100){
				printf("Packing 100 pieces!\n");
				
				write(fdArray[3][1],&piecesHere,sizeof(int));
				piecesHere-=100;
				sleep(sleepTime);
			}
			
		}




        for(int i = 0;i <NUM_PIPES;i++){
            if(i==2)
                close(fdArray[i][0]);
            else if(i==3)
                close(fdArray[i][1]);
        }

        exit(1);
    }

    for(int i = 0;i <NUM_PIPES;i++){
        if(i==3)
            close(fdArray[i][1]);
        else{
            close(fdArray[i][0]);
            close(fdArray[i][1]);
        }
            
    }


    int pieces;

	int producedParts=0;
	
	while(pieces!=-1){
		read(fdArray[3][0],&pieces,sizeof(int));
		
		if(pieces!=-1){
			producedParts+=pieces;	
		}
		
	}

	printf("\nProduced %d parts!!\n",producedParts);





    close(fdArray[3][0]);
}