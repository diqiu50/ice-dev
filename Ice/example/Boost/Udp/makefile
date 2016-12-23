all: server.out client.out

server.out:server.cpp
	g++ -g -o server.out server.cpp -std=c++11  -lboost_system -lboost_thread

client.out:client.cpp
	g++ -g -o client.out client.cpp -std=c++11  -lboost_system -lboost_thread



clean:
	rm server.out
	rm client.out


