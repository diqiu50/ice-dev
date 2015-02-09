#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <assert.h>

using namespace std;

typedef unsigned long long u64;
typedef unsigned int u32;
inline u64 OmnGetTimestamp()
{
	    timeval t;
		gettimeofday(&t, 0);
		return (((u64)t.tv_sec) * 1000000) + (u32)t.tv_usec;
}                                                       

int main(int argc, char** argv)
{
	struct sockaddr_un serveraddr, clientaddr;
	int fd;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	sprintf(serveraddr.sun_path, "%s", "test.sock");
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1) 
	{
		perror("faild to create socket");
		return 0;
	}

	bzero(&clientaddr, sizeof(clientaddr));
	clientaddr.sun_family = AF_UNIX;
	sprintf(clientaddr.sun_path, "%d%s", getpid(), ".sock");
	int rslt = bind(fd, (sockaddr*)&clientaddr, sizeof(clientaddr));
	if (rslt == -1)
	{
		perror("faild to bind");
		return -1;
	}

	rslt = connect(fd, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (rslt == -1)
	{
		perror("faild to connect");
		exit(1);
	}

	char a[100];
	char b;
	u64 t0 = OmnGetTimestamp();
	int MAXLEN  = 4096;
	char obj[MAXLEN];
	long long i=0;
	time_t t = time(NULL);
	while(1)
	{
		i++;
		if (i%100000 == 0)
		{
			int t2 = time(NULL);
			if(t2-t)
				cout << "total write: " << i/(t2-t) << ",totol per sec: " << i*MAXLEN/(t2-t) << endl;
		}
		write(fd, obj, MAXLEN);
		//read(fd, obj, MAXLEN);
	}
	sleep(100);
	close(fd);
}
//g++ -g client.cpp -o client.exe 
