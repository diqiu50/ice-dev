#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	struct sockaddr_in *sinp;
	const char *addr;
	int err;
	char abuf[INET_ADDRSTRLEN];

	hint.ai_flags = AI_CANONNAME;	
	hint.ai_family = 0;
	hint.ai_socktype = 0;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((err = getaddrinfo("www.qq.com", "ssh", &hint, &ailist)) != 0)
	{
		printf("%s\n", gai_strerror(err));
		return 0;
	}

	for (aip = ailist; aip != NULL; aip = aip->ai_next)
	{
		//printf("\nhost %s", aip->ai_canonname?aip->ai_canonname:"-");
		if (aip->ai_family == AF_INET)
		{
		printf("\nhost %s", aip->ai_canonname?aip->ai_canonname:"-");
			sinp = (struct sockaddr_in *)aip->ai_addr;
			addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
			printf(" address %s", addr?addr:"unknow");
			printf(" port %d", ntohs(sinp->sin_port));
			if (aip->ai_protocol == IPPROTO_TCP)
				printf(" protocol TCP\n");
			else if(aip->ai_protocol == IPPROTO_UDP)
				printf(" protocol UDP\n");
		}
		printf("===\n");
	}

	freeaddrinfo(ailist);
}
