#include <linux/input.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void simulate_key(int fd, unsigned int keycode, int keyvalue)
{
	struct input_event event;

	event.type = EV_KEY;
	event.code = keycode;
	event.value = keyvalue;
	gettimeofday(&event.time, 0);

	if (write(fd, &event, sizeof(event)) < 0) {
		printf("simulate key error/n");
		return;
	}else
		printf("simuate key %d, %d/n", keycode, keyvalue);
}



int main ( int argc, char *argv[] )
{
	const char *path = "/dev/pts/1";
	int r = write(0, path, 10);
	printf("r%d\n", r);
	return 0;
	int fd = open(path, O_RDWR);
	if(fd <= 0) {
		perror("");
		return -1;
	}
	char buff[10];
	int n=read(fd, buff, 10);
	write(fd, buff, n);
	printf("%s, %d", buff, n);
	/*
	struct input_event event;
	event.type = EV_KEY;
	event.code = KEY_W;
	event.value = 0;
	gettimeofday(&event.time, 0);
	write(fd, &event, sizeof(event));
	*/
	//event.type = EV_KEY;
	//event.code = KEY_W;
	//event.value = 1;
	//gettimeofday(&event.time, 0);
	//write(fd, &event, sizeof(event));
	//write(fd, "w", 1);

	/*
	char *path = "/dev/input/event0";
	unsigned int key_code = KEY_CAPSLOCK;

	if (argc >= 3) {
		path = argv[1];
		key_code = atoi(argv[2]);
	}else if (argc >= 2)
		path = argv[1];

	int fd_kbd = open(path, O_RDWR);

	if(fd_kbd <= 0) {
		printf("error open keyboard:%s/n", strerror(errno));
		return -1;
	}

	simulate_key(fd_kbd, key_code, 1);
	sleep(5);
	simulate_key(fd_kbd, key_code, 0);

	close(fd_kbd);
	*/

	return 0;
}               /* ----------  end of function main  ---------- */


