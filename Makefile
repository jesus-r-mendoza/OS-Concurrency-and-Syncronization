all:
	g++ -o buff src/buff.cpp -lpthread
	g++ -o mh src/mh.cpp -lpthread
	g++ -o airline src/airline.cpp -lpthread