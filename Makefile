all: exercise1

exercise1: main.o customString.o
	g++ -std=c++11 *.o -o Main

main.o: Main.cpp
	g++ -std=c++11 -c Main.cpp
	
customString.o: CustomString.hpp CustomString.cpp
	g++ -std=c++11 -c CustomString.cpp

run: all
	./Main

clean:
	rm -rf *.o *.gch Main