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

#include <netinet/tcp.h>

using namespace std;

typedef unsigned long long u64;
typedef unsigned int u32;


int main()
{
	struct sockaddr_in serveraddr;

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	const char *local_addr = "127.0.0.1";
	//const char *local_addr = "192.168.99.85";
	inet_aton(local_addr,&(serveraddr.sin_addr));
	serveraddr.sin_port=ntohs(5531);

	int fd = socket(AF_INET, SOCK_STREAM, 0);
	
	// Disable the Nagle (TCP No Delay) algorithm 
	/*
	int flag = 1;
	int ret = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) );
	if (ret == -1)
	{
		 printf("Couldn't setsockopt(TCP_NODELAY)\n");
		 exit(-1);
	}
	*/

	int rslt = connect(fd, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (rslt)
	{
		perror("faild:");
		exit(1);
	}

	sleep(5);

	//char a[20004];
	char a[1028];
	memset(a,0, 1028);
	int num = 0;
	
	u32 trys = 100;
	//while(trys--)
	while(1)
	{
		write(fd, a, 4);
		write(fd, a, 100);
		//write(fd, a, 104);

		read(fd, a, 104);

		sleep(2);
	}

	close(fd);
}
//g++ -g client.cpp -o client.exe 
