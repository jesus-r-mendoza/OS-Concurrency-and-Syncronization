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

pthread_mutex_t printLock;

sem_t readyToCheckBags;
sem_t readyForSecurity;
sem_t readyToBeSeated;
sem_t readyForTakeOff;

int psgrsArrived = 0;
int psgrsChecked = 0;
int psgrsScreened = 0;
int psgrsSeated = 0;

vector<int> peopleQueue;
vector<int> arrivalQueue;
vector<int> bagCheckedQueue;
vector<int> screenedQueue;

void* doWaitInLine(void* arg) {
    pthread_mutex_lock(&printLock);
    srand(time(0));
    int size = peopleQueue.size();
    int index = rand() % size;
    cout << "Passanger #" << peopleQueue[index] << " has arrived to the Terminal." << endl;
    arrivalQueue.push_back(peopleQueue[index]);
    peopleQueue.erase(peopleQueue.begin()+index);
    psgrsArrived++;
    pthread_mutex_unlock(&printLock);
    sem_post(&readyToCheckBags);
    pthread_exit(0);
}

void* doBagCheck(void* arg) {
    while(psgrsChecked < passangers) {
        sem_wait(&readyToCheckBags);

        pthread_mutex_lock(&printLock);
        srand(time(0));
        int size = arrivalQueue.size();
        int index = rand() % size;        
        cout << "Passanger #" << arrivalQueue[index] << " is waiting for baggage." << endl;
        bagCheckedQueue.push_back(arrivalQueue[index]);
        arrivalQueue.erase(arrivalQueue.begin()+index);
        psgrsChecked++;        
        pthread_mutex_unlock(&printLock);

        sem_post(&readyForSecurity);
    }
    pthread_exit(0);
}

void* doSecurityScreen(void* arg) {
    while(psgrsScreened < passangers) {
        sem_wait(&readyForSecurity);

        pthread_mutex_lock(&printLock);
        srand(time(0));
        int size = bagCheckedQueue.size();
        int index = rand() % size;
        cout << "Passanger #" << bagCheckedQueue[index] << " is waiting for security." << endl;
        screenedQueue.push_back(bagCheckedQueue[index]);
        bagCheckedQueue.erase(bagCheckedQueue.begin()+index);
        psgrsScreened++;
        pthread_mutex_unlock(&printLock);

        sem_post(&readyToBeSeated);
    }
    pthread_exit(0);
}

void* doSeatPassengers(void* arg) {
    while(psgrsSeated < passangers) {
        sem_wait(&readyToBeSeated);

        pthread_mutex_lock(&printLock);
        srand(time(0));
        int size = screenedQueue.size();
        int index = rand() % size;
        cout << "Passanger #" << screenedQueue[index] << " is seated and relaxing." << endl;
        screenedQueue.erase(screenedQueue.begin()+index);
        psgrsSeated++;
        pthread_mutex_unlock(&printLock);
    }
    sem_post(&readyForTakeOff);
    pthread_exit(0);
}

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
    
    if ( argc != 5 ) {
        cerr << "\n** Make Sure you use 4 arguments **\n\n";
        return -1;
    }

    passangers = stoi(args[1]);
    baggers = stoi(args[2]);
    screeners = stoi(args[3]);
    attendants = stoi(args[4]);

    cout << "\n\nThere are " << passangers << " Passangers," << endl;
    cout << baggers << " Baggage Checkers," << endl;
    cout << screeners << " Security Screeners," << endl;
    cout << "and " << attendants << " Flight Attendants.\n" << endl;

    pthread_mutex_init(&printLock, NULL);
    sem_init(&readyToCheckBags, 0, 0);
    sem_init(&readyForSecurity, 0, 0);
    sem_init(&readyToBeSeated, 0, 0);
    sem_init(&readyForTakeOff, 0, 0);

    randomArrival();

    pthread_t fligAttendants[attendants];
    for ( int i = 0; i < attendants; i++ )
        pthread_create(&fligAttendants[i], NULL, doSeatPassengers, NULL);

    pthread_t secScreeners[screeners];
    for ( int i = 0; i < screeners; i++ )
        pthread_create(&secScreeners[i], NULL, doSecurityScreen, NULL);

    pthread_t bagHandlers[baggers];
    for ( int i = 0; i < baggers; i++ )
        pthread_create(&bagHandlers[i], NULL, doBagCheck, NULL);

    pthread_t passgrs[passangers];
    for ( int i = 0; i < passangers; i++ )
        pthread_create(&passgrs[i], NULL, doWaitInLine, NULL);

    sem_wait(&readyForTakeOff);

    cout << "The plane has taken off" << endl;
    
    return 0;
}