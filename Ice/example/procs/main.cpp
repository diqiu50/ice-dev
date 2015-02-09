#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <proc/readproc.h>
#include <proc/sysinfo.h>

using namespace std;

map<string, unsigned long long> getNetworkSpeed();

jiff getTotalCpuTime()
{
	jiff cuse, cice, csys, cide, ciow, cxxx, cyyy, czzz;
	unsigned long pin, pout, s_in, sout;
	unsigned intr, ctxt;
	unsigned int running, blocked;
	unsigned int btime, processes;
	getstat(&cuse, &cice, &csys, &cide, &ciow, &cxxx, &cyyy, 
			&czzz, &pin, &pout, &s_in, &sout, &intr, &ctxt, &running, &blocked, &btime, &processes);
	return cuse+cice+csys+cide+ciow+cxxx+cyyy+czzz;
}

jiff getProcessCpuTime()
{
	pid_t pid = getpid();
	cout << pid << endl;
	PROCTAB* proctab = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_PID, &pid, 1);
	proc_t* proc = new proc_t;
	memset(proc, 0, sizeof(proc_t));
	proc_t* rslt = readproc(proctab, proc);
	cout <<  proc->utime << " : " <<  proc->stime << " : " <<  proc->cutime << " : " <<  proc->cstime << endl;
	jiff cputime = proc->utime + proc->stime + proc->cutime + proc->cstime;
	closeproc(proctab);
	delete proc;
	return cputime;
}

map<int, jiff> getTotalTasks()
{
	pid_t pid = getpid();
	PROCTAB* proctab = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_PID, &pid, 1);
	proc_t* proc = new proc_t;
	memset(proc, 0, sizeof(proc_t));
	proc_t* rslt = readproc(proctab, proc);
	proc_t* task = new proc_t;
	memset(task, 0, sizeof(proc_t));
	task = readtask(proctab, proc, task);
	map<int, jiff> tasks;
	while(task)
	{
		jiff cputime = task->utime + task->stime;
		tasks.insert(make_pair(task->tid, cputime));
		task = readtask(proctab, proc, task);
	}
	closeproc(proctab);
	delete proc;
	delete task;
	return tasks;
}

void printTasksCpu(
		map<int, jiff> &tasks1, 
		map<int, jiff> &tasks2, 
		jiff ct2, 
		jiff ct1)
{
	// first,all point to begin
	// compare the two itr's taskid
	// the low to ++
/*	
	map<int, jiff>::iterator itr1 = tasks1.begin();
	map<int, jiff>::iterator itr2 = tasks2.begin();
	while(1)
	{
		if (itr1 == tasks1.end() && itr2 == tasks2.end())
		{
			break;
		}
		if (itr1->first > itr2->first)
		{
			itr2
		}
	}
	*/
}

void* test(void *)
{
	int i=0;
	do
	{
	}while(1);
}

int main(int argc, char** argv)
{
	getNetworkSpeed();
	return 0;
	pthread_t t0, t1, t2, t3;
	pthread_create(&t0, NULL, test, NULL);
	pthread_create(&t1, NULL, test, NULL);
	pthread_create(&t2, NULL, test, NULL);
	pthread_create(&t3, NULL, test, NULL);

	while(1)
	{
		jiff pt1 = getProcessCpuTime();
		jiff ct1 = getTotalCpuTime();
		map<int, jiff> tasks1 = getTotalTasks();
		int i=1;
		sleep(1);
		jiff pt2 = getProcessCpuTime();
		jiff ct2 = getTotalCpuTime();
		map<int, jiff> tasks2 = getTotalTasks();
		
		int cpucores = sysconf(_SC_NPROCESSORS_ONLN);
		double cpu = cpucores * 100*(pt2-pt1)/(double)(ct2-ct1);
		cout << "pt2 : " << pt2 << " pt1 : " << pt1 << " ct2 : " << ct2 << " ct1 : " << ct1 << " cpu : " << cpu << endl;

		printTasksCpu(tasks1, tasks2, ct2, ct1);
	}
	pthread_join(t0, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	return 0;
}



map<string, unsigned long long> getNetworkSpeed()
{
	static int fd;  
	static char buff[BUFFSIZE];
	if(fd){
		lseek(fd, 0L, SEEK_SET);
	}else{ fd = open("/proc/net/dev", O_RDONLY, 0); if(fd == -1)
		{ perror("/proc/net/dev"); exit(1); }
	}
	read(fd,buff,BUFFSIZE-1);
	char *startpos;
	startpos = strstr(buff, "compressed\n");
	if (startpos) 
	{
		static int len = strlen("compressed");
		startpos += len;
	}
	char ethname[32];
	unsigned long long send, recv, invalid;
	map<string, unsigned long long> networks;
	while (startpos)
	{
		startpos++;
		sscanf(startpos, "%*[ ]%[^:]", ethname);
		startpos = strstr(startpos, ":");
		if (startpos == NULL) break;
		startpos++;
		sscanf(startpos, "%Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu", &recv, &invalid, &invalid,
				&invalid, &invalid, &invalid, &invalid, &invalid, &send);
		networks.insert(make_pair(ethname, recv));
		cout << "ethname :" << ethname << " "  << send << " " << recv << endl;
		startpos = strstr(startpos, "\n");
	}
	return networks;
}

//g++ main.cpp -I /home/ice/SofeWare/procps-3.2.8 libproc.a -lpthread
