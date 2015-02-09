#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	const char *message;
	const char *b = new char[1];
	int n;
	int exit_code;

	printf("fork program starting\n");
	pid = fork();
	switch(pid)
	{
		case -1:
			perror("fork failed");
			exit(1);
		case 0:// first child 
			message = "child";
			n = 1;
			exit_code = 37;
			pid = fork();// second child
			if(pid == 0){
				message = "grandson";
				n = 8;
				exit_code = 38;
			}
			break;
		default:
			message = "parent";
			n = 3;
			exit_code = 0;
			break;    
	}
	for(; n>0; n--) {
		printf("%s, %d, %d\n",message, getpid(), getppid());
		sleep(1);
	}

	if(pid != 0){
		int stat_val;
		pid_t child_pid;

		printf("a: %d, %d\n", getpid(), getppid());
		child_pid = wait(&stat_val);
		printf("b: %d, %d\n", getpid(), getppid());

		printf("child has finished: PID = %d\n", child_pid);
		if(WIFEXITED(stat_val))
			printf("child exited with code %d\n", WEXITSTATUS(stat_val));
		else
			printf("child terminated abnormally\n");
	}

	exit(exit_code);
}

