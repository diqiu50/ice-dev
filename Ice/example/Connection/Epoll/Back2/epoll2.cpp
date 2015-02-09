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

using namespace std;

#define MAXLINE 1000
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

typedef unsigned long long u64;                             
typedef unsigned int u32;

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
	//listenfd = socket(AF_INET, SOCK_STREAM, 0);
	listenfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	const char *local_addr = "0.0.0.0";
	inet_aton(local_addr,&(serveraddr.sin_addr));//htons(portnumber);
	serveraddr.sin_port=htons(5530);
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

	{
		struct sockaddr_in serveraddr;
		bzero(&serveraddr, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		const char *local_addr = "127.0.0.1";
		inet_aton(local_addr,&(serveraddr.sin_addr));
		serveraddr.sin_port=ntohs(5531);

		int rslt = connect(listenfd, (sockaddr*)&serveraddr, sizeof(serveraddr));
		if (rslt)
		{
			perror("faild:");
			exit(1);
		}
	}

	//int connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
	//if(connfd<0)
	//{
	//	perror("connfd<0");
	//	exit(1);
	//}
	//clilen = (socklen_t)sizeof(clientaddr);
	//char *str = inet_ntoa(clientaddr.sin_addr);
	//cout << "accapt a connection :" << connfd << endl;

	char buff[1028];
	while(1)
	{
		int reads = 0;
		while(1)
		{
			if(reads < 1028)
			{
				//n = read(connfd, buff, 1028 - reads);
				n = read(listenfd, buff, 1028 - reads);
				reads += n;
			}
			
			if(reads == 1028) break;
		}
		cout << "svr read 1028:" << endl;
	
		//write(connfd, buff, 1028);
		write(listenfd, buff, 1028);
	}

	return 0;
}
//g++ -g epoll.cpp -o epoll.exe
