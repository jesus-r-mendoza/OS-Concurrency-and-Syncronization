#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

#define BUFF_SIZE 4

//char chars[] =  {'!','#','$','%','&','(',')','*','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?','@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','[',']','^','_','`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
char chars[] = {'0','1','2','3','4','5','6','7','8','9'};
int charsSize = strlen(chars) -1;

char buffer[BUFF_SIZE];

pthread_mutex_t printLock;
sem_t empty, full;

int currIndex = -1;

char getNextChar() {
    pthread_mutex_lock(&printLock);
    if ( currIndex >= charsSize ) {
        cout << "Exited Producer Thread" << endl;
        pthread_mutex_unlock(&printLock);
        return '\\';
    }
    currIndex++;
    cout << "Procuder grabbed: " << chars[currIndex] << endl;
    pthread_mutex_unlock(&printLock);
    return chars[currIndex];
}

void* produce(void* arg) {
    int in = 0;
    while(true) {
        sem_wait(&empty);
        char next = getNextChar();
        if ( next != '\\' ) {
            buffer[in] = next;
            in = (in + 1) % BUFF_SIZE;
            sem_post(&full);
        } else {
            break;
        }
    }
}

int useNextChar(int out) {
    pthread_mutex_lock(&printLock);
    if ( currIndex >= charsSize ) {
        cout << "Exited Consumer Thread" << endl;
        pthread_mutex_unlock(&printLock);
        //pthread_exit(0);
        return -1;
    }
    cout << "Consumer grabbed: " << buffer[out] << endl;
    pthread_mutex_unlock(&printLock);
    return 0;
}

void* consume(void* arg) {
    int out = 0;
    while(true) {
        sem_wait(&full);
        int code = useNextChar(out);
        if ( code != -1 ) {
            out = (out + 1) % BUFF_SIZE;
            sem_post(&empty);
        } else {
            break;
        }
    }
}

int main() {
    pthread_mutex_init(&printLock, NULL);

    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFF_SIZE);

    pthread_t producer;
    pthread_create(&producer, NULL, produce, NULL);
    consume(NULL);

    return 0;
}