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

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("192.168.99.184");
	servaddr.sin_port = htons(5000);

	//const char *buff = "hello udp!";
	char buff[BUFFLEN];
	for (int i=0; i<2000; i++)
	{
		buff[i] = 'b';
	}

	struct msghdr msg;
	struct iovec iov[3];
	bzero(&msg, sizeof(msg));
	msg.msg_name = (struct sockaddr *)&servaddr;
	msg.msg_namelen = sizeof(sockaddr);
	iov[0].iov_base = (void*)"aaaa";
	iov[0].iov_len = 4;
	iov[1].iov_base = buff;
	iov[1].iov_len = 2000;
	iov[2].iov_base = (void*)"cccc";
	iov[2].iov_len = 4;
	msg.msg_iov = iov;
	msg.msg_iovlen = 3;

	int n = sendmsg(sockfd, &msg, 0);

	//int n = sendto(sockfd, buff, BUFFLEN, 0, (struct sockaddr *)&servaddr, sizeof(sockaddr));

	if (n<0) 
	{
		perror("send failed!");
		return 0;
	}
	printf("send msg[%s:%d]:%d\n", inet_ntoa(servaddr.sin_addr), servaddr.sin_port, n);
	char buff2[1024];
	n = recv(sockfd, buff2, 1024, 0);

	printf("recieve msg:%s\n", buff2);
	return 0;
}
