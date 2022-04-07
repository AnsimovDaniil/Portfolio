all: pr1 clean
	
clean:
	rm *.o
programm1.o: programm1.cpp
	gcc -c programm1.cpp -lstdc++ -pthread
Sender.o: Sender.cpp Sender.h
	gcc -c Sender.cpp -lstdc++
pr1: programm1.o Sender.o
	gcc -o pr1 Sender.o programm1.o -lstdc++ -pthread

