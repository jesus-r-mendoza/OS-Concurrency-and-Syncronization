#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

int passangers, baggers, screeners, attendants;

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

    return 0;
}