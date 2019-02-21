all: sqlite.out

sqlite.out: main.cpp  sqlite3.o 
	g++ -g -std=c++11 -o sqlite.out main.cpp sqlite3.o -lpthread -ldl

sqlite3.o: sqlite3.c
	gcc -c -O2 sqlite3.c -o sqlite3.o

clean: 
	rm sqlite.out sqlite3.o
