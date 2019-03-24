#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

#define BUFF_SIZE 20

// these are the printable characters that will be used by the producer / consumer
char chars[] =  {'!','#','$','%','&','(',')','*','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?','@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','[',']','^','_','`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
//char chars[] = {'0','1','2','3','4','5','6','7','8','9'};
int charsSize = strlen(chars)-1;

char buffer[BUFF_SIZE];

pthread_mutex_t printLock;
sem_t empty, full;

int currIndex = -1;
int printedChars = 0;

// This function is used to get the next character to put into the buffer
char getNextChar() {
    pthread_mutex_lock(&printLock);
    if ( currIndex >= charsSize ) {
        cout << "Exited Producer Thread" << endl;
        pthread_mutex_unlock(&printLock);
        pthread_exit(0);
    }
    currIndex++;
    cout << "Procuder placed: " << chars[currIndex] << endl;
    pthread_mutex_unlock(&printLock);
    return chars[currIndex];
}

// This is the function that the producer will execute
void* produce(void* arg) {
    int in = 0;
    char next;
    while(true) {
        sem_wait(&empty);
        next = getNextChar();
        buffer[in] = next;
        in = (in + 1) % BUFF_SIZE;
        sem_post(&full);
    }
}

// This function is used to print the next character from the buffer
void useNextChar(int out) {
    pthread_mutex_lock(&printLock);
    cout << "Consumer printed: " << buffer[out] << endl;
    printedChars++;
    if ( printedChars == charsSize+1 ) {
        cout << "Exited Consumer Thread" << endl;
        pthread_mutex_unlock(&printLock);
        pthread_exit(0);
    }
    pthread_mutex_unlock(&printLock);
    
}

// This is the function that the consumer will execute
void* consume(void* arg) {
    int out = 0;
    while(true) {
        sem_wait(&full);
        useNextChar(out);
        out = (out + 1) % BUFF_SIZE;
        sem_post(&empty);
    }
}

int main() {
    // creating the mutex and semaphores
    pthread_mutex_init(&printLock, NULL);

    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFF_SIZE);

    // initializing each thread
    pthread_t producer, consumer;
    pthread_create(&producer, NULL, produce, NULL);
    pthread_create(&consumer, NULL, consume, NULL);
    
    // joining the threads back to main after theu are complete
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    return 0;
}