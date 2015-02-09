#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

//#define FILESIZE  0x1fffffff
#define FILESIZE  0x10
#define PAGESIZE  0x1000

int main()
{
	remove("aa1.txt");
	FILE * pFile;
	pFile = fopen("aa1.txt", "w");
	cout << "create file " << FILESIZE << " bytes" << endl; 
	clock_t c1 = clock();
	time_t t1 = time(NULL);
	//for (unsigned long long i = 0; i <= FILESIZE; i++)
	//{
	//	fputc((int)i, pFile);
	//}
	fputc(49, pFile);
	fputc(50, pFile);
	fputc(51, pFile);
	fputc(52, pFile);
	fputc(53, pFile);
	clock_t c2 = clock();
	time_t t2 = time(NULL);
	cout << "file create cuptime: " << (c2-c1)/CLOCKS_PER_SEC << " sec " << endl;
	cout << "file create time: " << (t2-t1) << " sec " << endl;
	fclose(pFile);

	char *addr;
	int fd;
	int offset = 0;
	size_t length = sysconf(_SC_PAGE_SIZE);
	fd = open("aa1.txt", O_RDWR);
	addr = (char*)mmap(NULL, length, PROT_READ|PROT_WRITE,
			MAP_SHARED, fd, offset);

	if ((int)addr == -1)
		perror("mmap");
	addr[0]='A';
	return 0;
}
