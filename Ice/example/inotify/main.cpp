#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>


int main(int argv, char** argc)
{
	int fd = inotify_init();
	if (fd == -1)
	{
		perror("");
		exit(0);
	}
	int watch;
	if ((watch = inotify_add_watch(fd, "/home/ice/WebSite", IN_MODIFY)) < 0)
	{
		perror("");
		exit(0);
	}

	char buff[1024];
	while(1)
	{
		int num = read(fd, buff, 1024);
		if (num == -1)
		{
			perror("");
			exit(0);
		}
		printf("Num reads %d\n", num); 
		//int      wd;       /* Watch descriptor */
		//uint32_t mask;     /* Mask of events */
		//uint32_t cookie;   /* Unique cookie associating related
		//						events (for rename(2)) */
		//uint32_t len;      /* Size of name field */
		//char     name[];   /* Optional null-terminated name */
		int pos = 0;
		do
		{
			inotify_event *event = (inotify_event *)(buff+pos);
			printf("Watch descriptor: %d\n", event->wd);
			printf("Len: %d\n", event->len);
			printf("Name: %s\n\n", event->name);
			pos += sizeof(inotify_event)+event->len;
		}while(pos<num);
	}
	inotify_rm_watch(fd, watch);
	close(fd);
	return 0;
}
