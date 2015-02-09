#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <netinet/in.h>

#define MAXBUF 1024
#define BACKLOG 10

int main(int argc, char **argv)
{
	int sock_fd, new_fd;
	socklen_t len;
	struct sockaddr_in my_addr, their_addr;
	unsigned int myport, lisnum;
	char buf[MAXBUF+1];
	fd_set rfds;
	struct timeval tv;
	int retval, max_fd=-1;
	
	if (argv[1])
		myport = atoi(argv[1]);
	else
		myport = 6666;
		
	if (argv[2])
		lisnum = atoi(argv[2]);
	if (!lisnum)	lisnum = BACKLOG;
	
	if ((sock_fd = socket(PF_INET, SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(1);
	}
	
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = PF_INET;
	my_addr.sin_port = htons(myport);
	
	if (argv[3])
		my_addr.sin_addr.s_addr = inet_addr("192.168.99.13");
	else
		my_addr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(sock_fd,(struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1)
	{	
		perror("bind");
		exit(1);
	}
	
	printf("Server Listen: %d\n", myport);

	if(listen(sock_fd, lisnum) == -1)
	{
		perror("listen");
		exit(1);
	}
	
	while(1)
	{
		len = sizeof(struct sockaddr);	
		if((new_fd = accept(sock_fd, (struct sockaddr *)&their_addr,&len))==-1)
		{
			perror("accept");
			exit(errno);
		}
		else
			printf("Server:got connection from %s, prot %d, socket %d \n", 
				inet_ntoa(their_addr.sin_addr),
				ntohs(their_addr.sin_port), new_fd);
		
		printf("\nPlease input your message");
		
		while(1)
		{
			FD_ZERO(&rfds);
			FD_SET(0, &rfds);
			max_fd = 0;
			FD_SET(new_fd, &rfds);
			if (new_fd>max_fd)
				max_fd = new_fd;
			tv.tv_sec = 10;
			tv.tv_usec = 0;
			
			retval = select(max_fd+1, &rfds, NULL, NULL, &tv);
			if (retval == -1)
			{
				perror("selcct");
				break;
			}
			else if (retval == 0)
				continue;
			else
			{
				if(FD_ISSET(0, &rfds))
				{
					bzero(buf, MAXBUF+1);
					fgets(buf, MAXBUF, stdin);
					if (!strncasecmp(buf, "quit", 4))
					{
						printf("Exit");
						break;
					}
					
					len = send(new_fd, buf, strlen(buf)-1, 0);
					if(len>0)
						printf("Sent");
					else
					{
						printf("%d,%s", errno, strerror(errno));
						break;
					}
				}
			}
			
			if(FD_ISSET(new_fd, &rfds))
			{
				bzero(buf, MAXBUF+1);
				len = recv(new_fd, buf, MAXBUF, 0);
				if(len>0)
					printf("receive msg %s, len %d", buf,len);
				else
				{
					if(len<0)
						printf("receive faild! %s\n", strerror(errno));
					else
						printf("Exit");
					break;
				}
			}
		}
		close(new_fd);
    	printf("other user");
    	fflush(stdout);
    	bzero(buf, MAXBUF+1);
    	fgets(buf, MAXBUF, stdin);
    	if(!strncasecmp(buf, "no", 2))
    	{
    		printf("Abort!");
    		break;
    	}
	}
	
	close(sock_fd);
	return 0;
}

//g++ -g Server.cpp -Wall -o Server.exe
