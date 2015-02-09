#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <aio.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <vector>
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>


using namespace std;

void aiotest();
void procThread();
void aio_callback(union sigval value);

const int NUMFILE = 1; 
int sgFilefds[NUMFILE];
const int FILESIZE = 1024*1024*1024;
const int BLOCSIZE = 1024*1024;
queue<aiocb*> sgQueue;
mutex sgMutex;

condition_variable sgCondVar;

int main(int argc, char** argv)
{

	char fname[16];
	char *buff = new char[1024*1024];
	memset(buff, 48, 1024*1024);
	for (int j=0; j<NUMFILE; j++)
	{
		sprintf(fname, "a%03d.txt", j);
		sgFilefds[j] = open(fname, O_CREAT|O_RDWR, 00644);
		cout << "open file: " << fname << endl;
		for (int i=0; i<1024; i++)
		{
			write(sgFilefds[j], buff, 1024*1024);
		}
	}
	aiotest();
	return 0;
}

void aiotest()
{
	thread t(procThread);
	
	/*
	struct sigaction sa;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa.sa_sigaction = aio_callback;
	sigaction(SIGUSR1, &sa, NULL);
	*/

	static int total = 0;
	for (int i=0; i<NUMFILE; i++)
	{
		for (int j=0; j<FILESIZE/BLOCSIZE; j++)
		{
			aiocb *aioreq = (aiocb*)malloc(sizeof(aiocb));
			memset(aioreq, 0, sizeof(aiocb));

			aioreq->aio_fildes = sgFilefds[i];	
			aioreq->aio_offset = j*BLOCSIZE;	
			char* buff = (char*)malloc(BLOCSIZE+1);
			buff[BLOCSIZE] = 0;
			aioreq->aio_buf = buff;	
			aioreq->aio_nbytes = BLOCSIZE;	
//	cout << "offset " << aioreq->aio_offset << ", size" << aioreq->aio_nbytes << endl;
			aioreq->aio_reqprio = 0;	
			/*
			aioreq->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
			aioreq->aio_sigevent.sigev_signo = SIGUSR1;
			aioreq->aio_sigevent.sigev_value.sival_ptr = aioreq;
			*/
			aioreq->aio_sigevent.sigev_notify = SIGEV_THREAD;
			aioreq->aio_sigevent.sigev_value.sival_ptr = aioreq;
			aioreq->aio_sigevent.sigev_notify_function = aio_callback;

			int rslt = aio_read(aioreq);
			if (rslt != 0)
			{
				cout << "error0 " << rslt << endl;
			}
			rslt = aio_read(aioreq);
			if (rslt != 0)
			{
				cout << "error1 " << rslt << endl;
			}
			total++;
		}
	}
cout << "total req: " << total << endl;
	t.join();
}

static int sgCalls = 0;
void aio_callback(union sigval value)
{
	aiocb *aioreq = (aiocb*)(value.sival_ptr);
	if (0 != aio_error(aioreq))
	{
		cerr << "error2" << endl;
		return;
	}
sgCalls++;
	unique_lock<mutex> sgLock(sgMutex);
	sgQueue.push(aioreq);
	sgCondVar.notify_one();
	return;
}

void procThread()
{
	aiocb *aioreq = NULL;
	static int total_req = NUMFILE*(FILESIZE/BLOCSIZE);
	while(total_req)
	{
		{
			unique_lock<mutex> sgLock(sgMutex);
			if(sgQueue.empty())
			{
				sgCondVar.wait(sgLock);
				continue;
			}
			else
			{
				aioreq = sgQueue.front();
				sgQueue.pop();
			}
		}

		if (aioreq)
		{
			int bytesread = aio_return(aioreq);
			if (bytesread==-1)
			{
				cout << "error3 " <<endl;
			}
			else
			{
				//cout << aioreq->aio_nbytes << (char*)aioreq->aio_buf << endl;
				total_req--;
static int xx = 0;
if ((xx++%3000) == 0)
{
	cout << "left req " << total_req << endl;
}
			}
		}
	}
}
