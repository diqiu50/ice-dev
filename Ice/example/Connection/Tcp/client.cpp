#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <sys/time.h>
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
	struct sockaddr_in serveraddr;
	int fd = -1;

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	const char *local_addr = argv[1];
	inet_aton(local_addr,&(serveraddr.sin_addr));
	serveraddr.sin_port=ntohs(5531);

	fd = socket(AF_INET, SOCK_STREAM, 0);
	int rslt = connect(fd, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (rslt)
	{
		perror("faild:");
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
				cout << "total write, read: " << i/(t2-t) << ",totol per sec: " << i*MAXLEN/(t2-t) << endl;
		}
		write(fd, obj, MAXLEN);
		//read(fd, obj, MAXLEN);
	}
	sleep(100);
	close(fd);
}
//g++ -g client.cpp -o client.exe 
