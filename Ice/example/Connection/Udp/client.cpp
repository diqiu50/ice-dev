#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFLEN  64535

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_DATAG, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("192.168.99.87");
	servaddr.sin_port = htons(5000);

	int n;
	//const char *buff = "hello udp!";
	char buff[BUFFLEN];
	for (int i=0; i<10000; i++)
	{
		buff[i] = 'a';
	}
	int total = 0;
	while(1)
	{
		int n = sendto(sockfd, buff, BUFFLEN, 0, (struct sockaddr *)&servaddr, sizeof(sockaddr));
		if (n<0) 
		{
			perror("send failed!");
			return 0;
		}
		total += n;
		if ((total%10000) == 0)printf("total send %dk\n", total);
	}
	printf("send msg[%s:%d]:%s\n", inet_ntoa(servaddr.sin_addr), servaddr.sin_port, buff);
	char buff2[1024];
	n = recv(sockfd, buff2, 1024, 0);
	printf("recieve msg:%s\n", buff2);
}
