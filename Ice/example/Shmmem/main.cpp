#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h> 


int main(int argc, char** argv)
{
	int shmid;
	struct shmid_ds dsbuf;
	int size1 = getpagesize()*80000;
	key_t k1;
	k1 = ftok("a",0);
	//if (shmctl(shmid, IPC_RMID, NULL))
	//	return true;
	if ((shmid=shmget(k1,size1, 0664|IPC_CREAT))<0)
	{
		perror("share memery allocate error!");
		exit(1);
	}
	shmctl(shmid, IPC_STAT, &dsbuf); 
	char *firstAddr = (char*)shmat(shmid, NULL, 0);

	printf("share memery allocate %d  %p\n", shmid, firstAddr);
	if ((int)firstAddr==-1)
	{
		perror("share memery allocate error!");
		exit(1);
	}

	if (shmdt(firstAddr))
	{
		perror("share memery release error!");
		exit(1);
	}

	return 0;
}
