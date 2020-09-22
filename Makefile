all: server client

server: server.o
	g++ -o server myserver.cpp

client: client.o
	g++ -o client myclient.cpp

server.o: myserver.cpp
	g++ -std=c++14 -c myserver.cpp

client.o: myclient.cpp
	g++ -std=c++14 -c myclient.cpp

clean:
	rm -rf *o client server