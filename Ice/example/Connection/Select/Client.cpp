#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>


#define MAXBUF 1024

int main(int argc, char **argv)
{
	int sockfd, len;
	struct sockaddr_in dest;
	char buffer[MAXBUF+1];
	fd_set rfds;
	struct timeval tv;
	int retval, destport = 6666, maxfd=-1;
	
	if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) <0)
	{
		perror("socket");
		exit(errno);
	}
	
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(destport);
	dest.sin_addr.s_addr = inet_addr("192.168.99.13");
	
	
	if (connect(sockfd, (struct sockaddr *)&dest, sizeof(dest)))
	{
		perror("Connect");
		exit(errno);
	}
	
	while(1)
	{
		FD_ZERO(&rfds);
		FD_SET(0,&rfds);
		maxfd = 0;
		FD_SET(sockfd, &rfds);
		if (sockfd>maxfd)
		{
			maxfd = sockfd;
		}
		
		tv.tv_sec = 10;
		tv.tv_usec = 0;
		
		retval = select(maxfd+1, &rfds, NULL, NULL, &tv);
		if (retval == -1)
		{
			printf("select error %s", strerror(errno));
			break;
		}
		else if(retval == 0) 
		{
			continue;
		}
		else
		{
			if(FD_ISSET(sockfd, &rfds))
			{
				bzero(buffer, MAXBUF+1);
				len = recv(sockfd, buffer, MAXBUF, 0);
				if (len>0)
				{
					printf("receive message: %s , %d bytes\n", buffer, len);
				}
				else
				{
					printf("commnication abort\n");
				}
				break;
			}
		}
		
		if (FD_ISSET(0,&rfds))
		{
			bzero(buffer, MAXBUF+1);
			fgets(buffer, MAXBUF, stdin);
			if (!strncasecmp(buffer, "quit", 4))
			{
				printf("abort");
				break;
			}
			
			len = send(sockfd, buffer, strlen(buffer)-1, 0);
			if (len<0)
			{
				printf("send message failed!");
			}
			else
			{
				printf( "send: %s, %d bytes\n", buffer, len);
			}
		}
	}
}
