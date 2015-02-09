all: server.out client.out

server.out : server.cpp
	g++ server.cpp -o server.o -g

client.out : client.cpp
	g++ client.cpp -o client.o -g

clean:
	rm server.out client.out *.sock
