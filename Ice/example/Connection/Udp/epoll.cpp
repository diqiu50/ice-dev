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
	int i, maxi, listenfd, connfd, sockfd,epfd,nfds, portnumber;
	ssize_t n;
	char line[MAXLINE];
	socklen_t clilen;

	struct epoll_event ev,events[20];

	epfd=epoll_create(256);
	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	setnonblocking(listenfd);
	ev.data.fd=listenfd;

	ev.events=EPOLLIN|EPOLLET;
	//ev.events=EPOLLIN;
	epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	const char *local_addr = "0.0.0.0";
	inet_aton(local_addr,&(serveraddr.sin_addr));//htons(portnumber);
	struct linger ling;
	ling.l_onoff = 0;
	ling.l_linger = 0;
	setsockopt(listenfd, SOL_SOCKET, SO_LINGER, (char *) &ling, sizeof(ling));
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *) NULL, 0);

	serveraddr.sin_port=htons(5530);
	int rslt = bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));
	if (rslt == -1)
	{
		perror("listen:");
		exit(1);
	}
	listen(listenfd, LISTENQ);
	maxi = 0;
	for ( ; ; ) {
		nfds=epoll_wait(epfd,events,20,-1);

		for(i=0;i<nfds;++i)
		{
			if(events[i].data.fd==listenfd)
			{
				clilen = (socklen_t)sizeof(clientaddr);
				connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
				if(connfd<0){
					perror("connfd<0");
					exit(1);
				}
				//setnonblocking(connfd);

				char *str = inet_ntoa(clientaddr.sin_addr);
				cout << "accapt a connection :" << connfd << endl;

				ev.data.fd=connfd;
				ev.events=EPOLLIN|EPOLLET;
				epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
			}
			else if(events[i].events&EPOLLIN)
			{
				/*
				if ( (sockfd = events[i].data.fd) < 0)
					continue;
				if ( (n = read(sockfd, line, MAXLINE)) < 0) {
					if (errno == ECONNRESET) {
						close(sockfd);
						events[i].data.fd = -1;
					} else
						std::cout<<"readline error"<<std::endl;
				} else if (n == 0) {
					close(sockfd);
					events[i].data.fd = -1;
				}
				*/
				line[1000-1] = '\0';
				sockfd = events[i].data.fd;
				bool rslt = false;
				n = read(sockfd, line, 1000);
				if (n<0)
				{
					cout << "close Conn:" << sockfd << endl; 
					close(sockfd);
					continue;
				}
				cout << line << endl;
				//ev.data.fd=sockfd;
				//ev.events=EPOLLOUT|EPOLLET;

				//epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
			}
			else if(events[i].events&EPOLLOUT) // 如果有数据发送
			{
				sockfd = events[i].data.fd;
				write(sockfd, line, n);

				ev.data.fd=sockfd;
				ev.events=EPOLLIN|EPOLLET;

				epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
			}
		}
	}
	return 0;
}
//g++ -g epoll.cpp -o epoll.exe
