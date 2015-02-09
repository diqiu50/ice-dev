all:client.out server.out


client.out:client.cpp
	g++ -g client.cpp -o client.out -lpthread

server.out:epoll2.cpp
	g++ -g epoll2.cpp -o server.out -lpthread


clean:
	rm server.out client.out
