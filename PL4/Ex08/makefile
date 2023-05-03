starter: starter.c
	gcc -g -c -Wall starter.c -o starter.o -lrt -lpthread

consult: starter consult.c
	gcc -g -c -Wall consult.c -o consultSolo.o -lrt -lpthread
	gcc -g -Wall consultSolo.o starter.o -o consult.o -lrt -lpthread

insert: insert.c starter
	gcc -g -c -Wall insert.c -o insertSolo.o -lrt -lpthread
	gcc -g -Wall insertSolo.o starter.o -o insert.o -lrt -lpthread

consultAll: consultAll.c starter
	gcc -g -c -Wall consultAll.c -o consultAllSolo.o -lrt -lpthread
	gcc -g -Wall consultAllSolo.o starter.o -o consultAll.o -lrt -lpthread

main: main.c starter
	gcc -g -c -Wall main.c -o mainSolo.o -lrt -lpthread
	gcc -g -Wall mainSolo.o starter.o -o main.o -lrt -lpthread

run: consult insert consultAll main
	./main.o

clean:
	rm *.o