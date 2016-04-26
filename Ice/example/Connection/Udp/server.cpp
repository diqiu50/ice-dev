#include <stdio.h>
#include <sys/types.h>   
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFLEN  1000000

int main(int argc, char **arvg)
{
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(5000);
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr));
	listen(sockfd, 100);

	int total = 0;
	int i;
	for (;;i++)
	{
		int n;
		char buff[BUFFLEN];
		socklen_t alen = 128;
		if ((n = recvfrom(sockfd, buff, BUFFLEN, 0, (struct sockaddr *)&cliaddr, &alen)) >= 0)
		{
			total +=n;
			buff[n] = 0;
			//if ((i%1000)==0) printf("total recv %dk\n", total/1000);
			printf("recieve msg[%s:%d]:(%d)%s\n", inet_ntoa(cliaddr.sin_addr), 
					//ntohs(cliaddr.sin_port), buff);
					ntohs(cliaddr.sin_port), n, buff);
			//sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&cliaddr, alen);
		}
		else
		{
			perror("read failed!");
			break;
		}
	}
}
