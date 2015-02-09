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

typedef unsigned long long u64;
typedef unsigned int u32;
inline u64 OmnGetTimestamp()
{
	    timeval t;
		gettimeofday(&t, 0);
		return (((u64)t.tv_sec) * 1000000) + (u32)t.tv_usec;
}                                                       

int fd = -1;
sem_t sem1, sem2;

int  thrdfun()
{
	char a[1024];
	memset(a,0, 1024);
	int num = 0;
	while(1)
	{
		sem_wait(&sem1);
		int i = 1024;
		read(fd, &i, 4);
		//usleep(1000000);
		if (!(rand()%10)) usleep(2);
		read(fd, a, 1024);
		//usleep(1000000);
		if (!(rand()%20)) usleep(4);
		sem_post(&sem2);
	}
}


int main()
{
	pthread_t p_thread1;
	pthread_create(&p_thread1,NULL, (void* (*)(void *))thrdfun,NULL);
	sem_init(&sem1, 0, 0);
	sem_init(&sem2, 0, 0);

	struct sockaddr_in serveraddr;

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	const char *local_addr = "127.0.0.1";
	inet_aton(local_addr,&(serveraddr.sin_addr));
	serveraddr.sin_port=ntohs(5530);

	fd = socket(AF_INET, SOCK_STREAM, 0);
	int rslt = connect(fd, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (rslt)
	{
		perror("faild:");
		exit(1);
	}

	char a[1028];
	memset(a,0, 1028);
	int num = 0;
	u64 t0 = OmnGetTimestamp();
	
	u32 trys = 100;
	//while(trys--)
	while(1)
	{
		int i = 1024;
		sem_post(&sem1);
		
		write(fd, &i, 4);
		//usleep(2);
		write(fd, a, 1024);
		
		sem_wait(&sem2);
	
		/*
		//if (!(rand()%20)) usleep(4);
		//usleep(2);
		read(fd, &i, 4);
		//if (!(rand()%10)) usleep(2);
		//usleep(2);
		read(fd, a, 1024);
		//usleep(4);
		//if (!(rand()%20)) usleep(4);
		//if (num++ == 10000) {sleep(1); num =0;}
		*/
	}
	close(fd);
}
//g++ -g client.cpp -o client.exe 