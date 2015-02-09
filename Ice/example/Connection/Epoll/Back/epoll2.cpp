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

sem_t sem1, sem2;
int connfd = -1;
int epfd2;

int  thrdfun()
{
	//struct epoll_event events[2];
	char buff[1028];
	int writes = 0;
	int n;
	while(1)
	{
		sem_wait(&sem1);
		
		//int nfds = epoll_wait(epfd2,events,2,-1);
		//if (nfds <=0) continue;

		/*
		//if(events[0].events&EPOLLOUT)
		{
			if (writes < 4)
			{
				n = write(connfd, buff, 4-writes);
				if (n <= 0)
				{
					if (n == 0)
					{
						cout << "close Conn:" << strerror(errno) << endl; 
						//goto acpt;
					}
					if(errno == EAGAIN)
						break;
				}
				writes += n;
			}

			n = write(connfd, buff, 1028 - writes);
			if (n <= 0)
			{
				if (n == 0)
				{
					cout << "close Conn:" << strerror(errno) << endl; 
					//goto acpt;
				}
				if(errno == EAGAIN)
					break;
			}
			writes += n;
			if (writes == 1028)
			{
				writes = 0;
				
				cout << "send finish" << endl;
			}
		}
		*/
		while(writes != 1028)
		{
			n = write(connfd, buff, 1028 - writes);
			writes += n;
			if(writes == 1028)
			{
				writes = 0;
				//cout << "send finish" << endl;
				break;
			}
		}
	
	}
}


int main(int argc, char* argv[])
{
	pthread_t p_thread1;
	pthread_create(&p_thread1,NULL, (void* (*)(void *))thrdfun,NULL);
	sem_init(&sem1, 0, 0);
	//sem_init(&sem2, 0, 0);
	
	//int i, maxi, listenfd, connfd, sockfd,epfd,nfds, portnumber;
	int i, maxi, listenfd, sockfd,epfd,nfds, portnumber;
	ssize_t n;
	char line[MAXLINE];
	socklen_t clilen;

	//struct epoll_event ev,events[2];
	//epfd=epoll_create(256);

	struct sockaddr_in clientaddr;
	struct sockaddr_in serveraddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	
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

acpt:
	connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
	if(connfd<0){
		perror("connfd<0");
		exit(1);
	}
	clilen = (socklen_t)sizeof(clientaddr);
	char *str = inet_ntoa(clientaddr.sin_addr);
	cout << "accapt a connection :" << connfd << endl;

	/*
	setnonblocking(connfd);

	ev.data.fd=connfd;
	ev.events=EPOLLIN;
	epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);

	struct epoll_event ev2;
	epfd2 = epoll_create(256);

	ev2.data.fd = connfd;
	ev2.events = EPOLLOUT;
	epoll_ctl(epfd2,EPOLL_CTL_ADD,connfd,&ev2);
	*/

	char buff[1028];
	int reads = 0;
	/*
	for ( ; ; ) 
	{
		//nfds=epoll_wait(epfd,events,2,-1);
		//if (nfds <=0) continue;

		//if(events[0].events&EPOLLIN)
		{
			while(1)
			{
				if (reads < 4)
				{
					n = read(connfd, buff, 4-reads);
					if (n <= 0)
					{
						if (n == 0)
						{
							cout << "close Conn:" << strerror(errno) << endl; 
							goto acpt;
						}
						if(errno == EAGAIN)
							break;
					}
					reads += n;
				}
					
				n = read(connfd, buff, 1028 - reads);
				if (n <= 0)
				{
					if (n == 0)
					{
						cout << "close Conn:" << strerror(errno) << endl; 
						goto acpt;
					}
					if(errno == EAGAIN)
						break;
				}
				reads += n;
				if (reads == 1028) 
				{
					cout << "read finish" << endl;
					reads = 0;
					sem_post(&sem1);
					cout << "sem_post" << endl;
				}
			}
		}
	}
	*/
	while(1)
	{
		if(reads != 1028)
		{
			n = read(connfd, buff, 1028 - reads);
			reads += n;
			if(reads == 1028)
			{
				reads = 0;
				sem_post(&sem1);
			}
		}
	}
	
	return 0;
}
//g++ -g epoll.cpp -o epoll.exe
