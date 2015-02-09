all:: test.o 

test.o : main.cpp
	g++ -g -I mongo-cxx-driver-v2.4/src -o test.o main.cpp mongo-cxx-driver-v2.4/libmongoclient.a \
	-lboost_system -lboost_filesystem -lboost_thread

clean:
	rm test.o
