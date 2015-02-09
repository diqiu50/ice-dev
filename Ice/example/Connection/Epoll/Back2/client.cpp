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
#include <semaphore.h>

using namespace std;

#define MAXLINE 1000
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

typedef unsigned long long u64;
typedef unsigned int u32;
inline u64 OmnGetTimestamp()
{
	    timeval t;
		gettimeofday(&t, 0);
		return (((u64)t.tv_sec) * 1000000) + (u32)t.tv_usec;
}                                                       

int main()
{
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	{
		int i, maxi, listenfd, sockfd,epfd,nfds, portnumber;
		ssize_t n;
		char line[MAXLINE];
		socklen_t clilen;

		struct sockaddr_in clientaddr;
		struct sockaddr_in serveraddr;
		//listenfd = socket(AF_INET, SOCK_STREAM, 0);
		//listenfd = socket(AF_INET, SOCK_DGRAM, 0);
		
		bzero(&serveraddr, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		const char *local_addr = "0.0.0.0";
		inet_aton(local_addr,&(serveraddr.sin_addr));//htons(portnumber);
		serveraddr.sin_port=htons(5531);
		struct linger ling;
		ling.l_onoff = 0;
		ling.l_linger = 0;
		setsockopt(listenfd, SOL_SOCKET, SO_LINGER, (char *) &ling, sizeof(ling));
		setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *) NULL, 0);

		int rslt = bind(fd,(sockaddr *)&serveraddr, sizeof(serveraddr));
		if (rslt == -1)
		{
			perror("listen:");
			exit(1);
		}
		//listen(listenfd, LISTENQ);
	}

	struct sockaddr_in serveraddr;
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	const char *local_addr = "127.0.0.1";
	inet_aton(local_addr,&(serveraddr.sin_addr));
	serveraddr.sin_port=ntohs(5530);


	//int fd = socket(AF_INET, SOCK_STREAM, 0);

	int rslt = connect(fd, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (rslt)
	{
		perror("faild:");
		exit(1);
	}


	char a[1028];
	memset(a,0, 1028);
	
	//u32 trys = 100;
	//while(trys--)
	while(1)
	{
		cout << "client write 5 and 1023:" << endl;
		write(fd, a, 5);
		write(fd, a, 1023);
		//write(fd, a, 1028);

		read(fd, a, 1028);
		sleep(5);
	}
	close(fd);
}
//g++ -g client.cpp -o client.exe 
