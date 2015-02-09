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
	int n, rc;
	int exit_code;

	printf("fork program starting\n");
	pid = fork();
	switch(pid)
	{
		case -1:
			perror("fork failed");
			exit(1);
		case 0:// child 
			message = "child";
			printf("child running... %d\n", getpid());
			//sleep(10);
			rc = execl("/bin/ls", "ls", NULL);
			if (rc == -1)
				perror("");
			n = 1;
			exit_code = 37;
			break;
		default:
			message = "parent";
			n = 1;
			exit_code = 0;
			break;    
	}

	if(pid != 0){
		int stat_val;
		pid_t child_pid;

		sleep(20);
		//for (int i=0; i<5; i++) {
		//	printf("parent running...\n");
		//	sleep(1);
		//}
		do {
			child_pid = waitpid(-1, NULL, WNOHANG);
			printf("rslt= %d\n", child_pid);
			sleep(1);
		}while(child_pid==0);
		printf("child has finished: PID = %d\n", child_pid);
	}
	exit(exit_code);
}

