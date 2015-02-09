#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <semaphore.h>
#include <pthread.h>

#include <netinet/tcp.h>

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

void setnonblocking(int sock)
{
	int opts;
	opts=fcntl(sock,F_GETFL);
	if(opts<0)
	{
		perror("fcntl(sock,GETFL)");
		exit(1);
	}
	opts = opts|O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts)<0)
	{
		perror("fcntl(sock,SETFL,opts)");
		exit(1);
	}
}


int main(int argc, char* argv[])
{
	int i, maxi, listenfd, sockfd,epfd,nfds, portnumber;
	ssize_t n;
	char line[MAXLINE];
	socklen_t clilen;

	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
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

	int rslt = bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));
	if (rslt == -1)
	{
		perror("listen:");
		exit(1);
	}
	listen(listenfd, LISTENQ);

	int connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
	if(connfd<0){
		perror("connfd<0");
		exit(1);
	}
	clilen = (socklen_t)sizeof(clientaddr);
	char *str = inet_ntoa(clientaddr.sin_addr);
	cout << "accapt a connection :" << connfd << endl;
	
	// Disable the Nagle (TCP No Delay) algorithm
	/*
	int flag = 1;
	int ret = setsockopt(connfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) );
	if (ret == -1)
	{
		 printf("Couldn't setsockopt(TCP_NODELAY)\n");
		 exit(-1);
	}
	*/	

	//char buff[20004];		small bag is < mss. mss == 0.5 * win.
	char buff[1028];
	while(1)
	{
		int reads = 0;
		while(1)
		{
			if(reads < 104)
			{
				n = read(connfd, buff, 104 - reads);
				reads += n;
			}
			
			if(reads == 104) break;
		}
		cout << "svr read 104:" << endl;

		write(connfd, buff, 104);
	}

	return 0;
}
//g++ -g epoll.cpp -o epoll.exe
