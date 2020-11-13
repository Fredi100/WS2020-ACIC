all: exercise1

exercise1: emsenhuber.o
	g++ -fsanitize=leak -std=c++11 *.o -o emsenhuber

main.o: emsenhuber.cpp
	g++ -fsanitize=leak -std=c++11 -c emsenhuber.cpp

run: all
	./emsenhuber

clean:
	rm -rf *.o *.gch emsenhuber