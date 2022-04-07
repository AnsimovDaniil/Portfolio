all: pr2 clean
	
clean:
	rm *.o
programm2.o: programm2.cpp
	gcc -c programm2.cpp -lstdc++ -pthread
Reciever.o: Reciever.cpp Reciever2.h
	gcc -c Reciever.cpp -lstdc++
pr2: programm2.o Reciever.o
	gcc -o pr2 Reciever.o programm2.o -lstdc++ -pthread
