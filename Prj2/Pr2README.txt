Project 2 README

Group Members:
Jesus Mendoza
Kimberly Reyes

Prj2.zip contents:

Pr2README.txt
airline.cpp
buff.cpp
mh.cpp
Makefile

In order to compile all of the files follow these steps:

1. Unzip Prj2.zip

2. In the terminal, move to Prj2 by using:
   cd Prj2

3. In the terminal, compile all the files using the make file by using:
   make

4. Now follow the next steps to run each file.

- - - - - - - - - - - - - - - 

To run the Produce-Consumer problem you will use the ./buff executable.
Use as follows:

==================================================================
./buff
==================================================================

This executable take no arguments. Because threads are undeterministic,
every time you run it you may get a different output.
In order to see the different cases ( producer waiting; consumer waiting;
both producer and consumer working ), simply run the file multiple times.
And note that the output varies, sometimes both the producer consumer will
be working, and other times the producer will be waiting for the consumer 
or vice versa.

- - - - - - - - - - - - - - - 

To run the Mother Hubbard problem, you will use the ./mh executable.
Use as follows:

==================================================================
./mh <days>
==================================================================

example:  ./mh 25
    25 - days
    // Correct program output will appear

This executable takes in a single INTEGER value as an argument, this argument
will be the number of days that will be simulated.

- - - - - - - - - - - - - - - 

To run the Airline problem, you will use the ./airline executable.
Use as follows:

==================================================================
./airline <passangers> <baggers> <screeners> <flight attendants>
==================================================================

example:  ./airline 100 6 3 5
    100 - passangers
    6   - baggage checkers
    3   - security screeners
    5   - flight attendants
    // Correct program output will appear

example:  ./airline 100 4 5
    ** Make sure you use 4 arguments **
    Usage: ./airline <int> <int> <int> <int>
    // Graceful exit

example:  ./airline p 2 4 5
    ** Make sure you use Integer values for each argument **
    Usage: ./airline <int> <int> <int> <int>
    // Graceful exit

This executable takes in 4 INTEGER values as arguments. Make sure that
when passing the arguments to this program, each INTEGER value is passed
in that order, since that is how the program will interpret them.

For this problem, we assume that all the passangers arrive at the airport
at the EXACT same time. However, they must line up to be processed. Since
they all arrive at the same time, the baggage handlers begin to process
passangers in no particular order (randomly). Then, those who have had their
bags checked, form another line to pass through security. The security screeners
do the same thing as the baggage handlers, and simply randomly process someone
that has ALREADY had their bags checked. Once those passangers are screened,
they go into a new line where, again, the flight attendants randomly select
a passanger (that has been screened) to board onto the plane. Once all the
passangers have gone through each line and has been seated. The plane takes off.

- - - - - - - - - - - - - - - 