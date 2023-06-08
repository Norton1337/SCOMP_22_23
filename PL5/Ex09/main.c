/*

Implement a railway simulator. The railway network’s infrastructure is composed by four city train
stations (”City A”, ”City B”, ”City C” and ”City D”) City A connects to City B, City B connects to City C and D.
The program should:
• Use threads to simulate the trains;
• Number each train;
• Simulate each trip using sleep function (pthread sleep);
• Guarantee each connection is a single-track. So, only one train can use it at a given time instant;
• Print the train number, origin an destination when a track is being used;
• Print the trip’s duration when the train finishes it .

*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_TRAINS 5
#define NUM_TRACKS 3
//A<->B, B<->C, B<->D
pthread_mutex_t trackMutexes[NUM_TRACKS];

typedef struct {
    int train_number;
    char *origin;
    char *destination;
    pthread_mutex_t *trackMutex;
} Train;

void *simulateTrain(void *arg) {
    Train *train = (Train *)arg;
    int trip_duration = rand() % 10 + 1;

    // Lock the track
    pthread_mutex_lock(train->trackMutex);

    printf("Train %d: Departing from %s to %s\n", train->train_number, train->origin, train->destination);

    sleep(trip_duration);
 
    printf("Train %d: Arrived at %s. Trip duration: %d seconds\n", train->train_number, train->destination, trip_duration);
    
    // Release the track
    pthread_mutex_unlock(train->trackMutex);
    
    pthread_exit(NULL);
}

int main() {
    pthread_t trains[NUM_TRAINS];
    Train train_data[NUM_TRAINS];
    
    srand(time(NULL));

    for (int i = 0; i < NUM_TRAINS; i++) {
        Train *train = &train_data[i];
        train->train_number = i + 1;
        
        int randomCase = rand() % 6;

        switch (randomCase) {
            case 0:
                train->origin = "City A";
                train->destination = "City B";
                break;
            case 1:
                train->origin = "City B";
                train->destination = "City C";
                break;
            case 2:
                train->origin = "City B";
                train->destination = "City D";
                break;
            case 3:
                train->origin = "City C";
                train->destination = "City B";
                break;
            case 4:
                train->origin = "City D";
                train->destination = "City B";
                break;
            case 5:
                train->origin = "City B";
                train->destination = "City A";
                break;
        }
        
        pthread_mutex_init(&trackMutexes[i], NULL);
        if(train->origin == "City A")
            train->trackMutex = &trackMutexes[0];
        else if(train->origin == "City B" && train->destination == "City C")
            train->trackMutex = &trackMutexes[1];
        else if(train->origin == "City B" && train->destination == "City D")
            train->trackMutex = &trackMutexes[2];
        else if(train->origin == "City C" && train->destination == "City B")
            train->trackMutex = &trackMutexes[1];
        else if(train->origin == "City D" && train->destination == "City B")
            train->trackMutex = &trackMutexes[2];
        else if(train->origin == "City B" && train->destination == "City A")
            train->trackMutex = &trackMutexes[0];
        

        pthread_create(&trains[i], NULL, simulateTrain, train);
    }
    

    for (int i = 0; i < NUM_TRAINS; i++) {
        pthread_join(trains[i], NULL);
    }
    
    for (int i = 0; i < NUM_TRACKS; i++) {
        pthread_mutex_destroy(&trackMutexes[i]);
    }
    
    return 0;
}
