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
	struct sockaddr_un serveraddr;
	int fd ,cltfd;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	sprintf(serveraddr.sun_path, "%s", "test.sock");
	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd == -1) 
	{
		perror("faild to create socket");
		return -1;
	}

	unlink("test.sock");
	int rslt = bind(fd, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (rslt == -1)
	{
		perror("faild to bind");
		return -1;
	}

	rslt = listen(fd, 100);
	if (rslt == -1)
	{
		perror("faild to listen");
		return -1;
	}

	socklen_t len = sizeof(serveraddr);
	cltfd = accept(fd, (sockaddr*)&serveraddr, &len);
	if (cltfd == -1)
	{
		perror("faild to accept");
		return -1;
	}
	cout << "client path: " << serveraddr.sun_path << endl;

	u64 t0 = OmnGetTimestamp();
	int MAXLEN  = 1024*1024;
	char obj[MAXLEN];
	long long i=0;
	time_t t = time(NULL);

	long total = 0;
	while(1)
	{
		i++;
		if (i%100000 == 0)
		{
			int t2 = time(NULL);
			if(t2-t)
				cout << "total read: " << total << ",total per sec: " << total/(t2-t)/1024/1024 << endl;
		}
		int n = read(cltfd, obj, MAXLEN);
		assert(n>0);
		total+=n;

	}
	sleep(100);
	close(fd);
}
//g++ -g server.cpp -o client.exe 
