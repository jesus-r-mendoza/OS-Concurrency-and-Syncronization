#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <vector>
using namespace std;

int passangers;
int baggers;
int screeners;
int attendants;

// This mutext is to ensure that only one action is
// printed to the screen at a time
pthread_mutex_t printLock;

// These are the semaphores that will be used to syncronize
// each passanger process in each section
sem_t readyToCheckBags;
sem_t readyForSecurity;
sem_t readyToBeSeated;
sem_t readyForTakeOff;

// These counters will help the threads keep track of how many passangers
// have been processed in their section, in order to know when to stop
int psgrsArrived = 0;
int psgrsChecked = 0;
int psgrsScreened = 0;
int psgrsSeated = 0;

// These are the queues (or lines) that will store the passanger id
// that is waiting in line
vector<int> peopleQueue;
vector<int> arrivalQueue;
vector<int> bagCheckedQueue;
vector<int> screenedQueue;

// This is the function that each Passanger thread will execute
void* doWaitInLine(void* arg) {
    pthread_mutex_lock(&printLock);
    // Here we will randomly select a passanger that has already
    // arrived to the airport, to escort them to the terminal and prevent overcrowding
    srand(time(0));
    int size = peopleQueue.size();
    int index = rand() % size;
    cout << "Passanger #" << peopleQueue[index] << " has arrived to the Terminal." << endl;
    arrivalQueue.push_back(peopleQueue[index]);
    peopleQueue.erase(peopleQueue.begin()+index);
    psgrsArrived++;
    // Incrementing the count means that the passanger is now waiting in line, at the correct terminal
    pthread_mutex_unlock(&printLock);
    sem_post(&readyToCheckBags);
    pthread_exit(0);
}

// This is the function that each BaggageChecker thread will execute
void* doBagCheck(void* arg) {
    while(psgrsChecked < passangers) {
        sem_wait(&readyToCheckBags);

        pthread_mutex_lock(&printLock);
        // Here we will randomly select a passanger that has already
        // arrived at the terminal, in order check their bags
        srand(time(0));
        int size = arrivalQueue.size();
        int index = rand() % size;        
        cout << "Passanger #" << arrivalQueue[index] << " is getting their baggage checked." << endl;
        bagCheckedQueue.push_back(arrivalQueue[index]);
        arrivalQueue.erase(arrivalQueue.begin()+index);
        psgrsChecked++;
        // Incrementing the count means that the passanger has successfully had their bags checked
        pthread_mutex_unlock(&printLock);

        sem_post(&readyForSecurity);
    }
    pthread_exit(0);
}

// This is the function that each SecurityScreener thread will execute
void* doSecurityScreen(void* arg) {
    while(psgrsScreened < passangers) {
        sem_wait(&readyForSecurity);

        pthread_mutex_lock(&printLock);
        // Here we will randomly select a passanger that has already
        // been checked, in order pass them through security
        srand(time(0));
        int size = bagCheckedQueue.size();
        int index = rand() % size;
        cout << "Passanger #" << bagCheckedQueue[index] << " is getting screened by security." << endl;
        screenedQueue.push_back(bagCheckedQueue[index]);
        bagCheckedQueue.erase(bagCheckedQueue.begin()+index);
        psgrsScreened++;
        // Incrementing the count means that the passanger has successfully passed security
        pthread_mutex_unlock(&printLock);

        sem_post(&readyToBeSeated);
    }
    pthread_exit(0);
}

// This is the function that each FlightAttendant thread will execute
void* doSeatPassengers(void* arg) {
    while(psgrsSeated < passangers) {
        sem_wait(&readyToBeSeated);

        pthread_mutex_lock(&printLock);
        // Here we will randomly select a passanger that has already
        // been screened, in order to seat them aboard the plane
        srand(time(0));
        int size = screenedQueue.size();
        int index = rand() % size;
        cout << "Passanger #" << screenedQueue[index] << " is seated and relaxing." << endl;
        screenedQueue.erase(screenedQueue.begin()+index);
        psgrsSeated++;
        // Incrementing the count means that the passanger has successfully boarded the plane
        pthread_mutex_unlock(&printLock);
    }
    sem_post(&readyForTakeOff);
    pthread_exit(0);
}

// This function will ensure that we simulate that all the
// passangers arrive at the airport (but not he terminal)
// at the same time
void randomArrival() {
    vector<int> people;
    for ( int i = 0; i < passangers; i++ )
        people.push_back(i);

    int size;
    while ( people.size() > 0 ) {
        srand(time(0));
        int size = people.size();
        int index = rand() % size;
        peopleQueue.push_back(people[index]);
        people.erase(people.begin()+index);
    }
}

int main(int argc, char* args[]) {
    
    // If the incorrect amount of arguments are passed, the we exit gracefully
    if ( argc != 5 ) {
        cerr << "\n** Make sure you use 4 arguments **\n";
        cerr << "Usage: ./airline <int> <int> <int> <int>\n\n";
        return -1;
    }
    try {
        passangers = stoi(args[1]);
        baggers = stoi(args[2]);
        screeners = stoi(args[3]);
        attendants = stoi(args[4]);
    } catch (invalid_argument) {
        // If non-integer value arguments are passed, then we exit gracefully
        cerr << "\n** Make sure you type in Integer values for each argument **\n";
        cerr << "Usage: ./airline <int> <int> <int> <int>\n\n";
        return -1;
    }
    cout << "\nThere are: \n" << passangers << " Passangers" << endl;
    cout << baggers << " Baggage Checkers" << endl;
    cout << screeners << " Security Screeners" << endl;
    cout << attendants << " Flight Attendants\n" << endl;

    // Here we initialize all of the semaphores and the mutex
    pthread_mutex_init(&printLock, NULL);
    sem_init(&readyToCheckBags, 0, 0);
    sem_init(&readyForSecurity, 0, 0);
    sem_init(&readyToBeSeated, 0, 0);
    sem_init(&readyForTakeOff, 0, 0);

    // Here we make it so that all the passangers arrive at the AIRPORT at the same time
    randomArrival();

    // Create all of the flight attendant threads
    pthread_t fligAttendants[attendants];
    for ( int i = 0; i < attendants; i++ )
        pthread_create(&fligAttendants[i], NULL, doSeatPassengers, NULL);

    // Create all of the security screener threads
    pthread_t secScreeners[screeners];
    for ( int i = 0; i < screeners; i++ )
        pthread_create(&secScreeners[i], NULL, doSecurityScreen, NULL);

    // Create all of the baggage checker threads
    pthread_t bagHandlers[baggers];
    for ( int i = 0; i < baggers; i++ )
        pthread_create(&bagHandlers[i], NULL, doBagCheck, NULL);

    // Create all of the passanger threads
    pthread_t passgrs[passangers];
    for ( int i = 0; i < passangers; i++ )
        pthread_create(&passgrs[i], NULL, doWaitInLine, NULL);

    // No thread joining is necessary since they will
    // individually exit when they are complete
    
    // This makes it so that the main thread waits for all other threads
    // to complete, (meaning that all the pasangers have been seated)
    // so that the plane may take off
    sem_wait(&readyForTakeOff);

    cout << "\nEvery passanger has boarded the plane." << endl;
    cout << "The plane is now taking off.\n" << endl;
    
    return 0;
}