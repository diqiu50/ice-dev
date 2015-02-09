#include <iostream>
#include <string.h>

using namespace std;

int i = 0;
void call1(char*);
void call2(char*);
void call3(char*);
void call4(char*);
void call5(char*);
void call6(char*);
void call7(char*);
void call8(char*);

int main()
{
	const char *a = "aaaa";
	char *b = new char;
	call1(b);

	cout << "Code\t" << (long)main <<"," << hex << "0x" << (long)main << endl;
	cout << "Data\t" << dec << (long)a <<"," << hex << "0x" << (long)a << endl;
	cout << "BBS\t" << dec << (long)&i <<"," << hex << "0x" << (long)&i << endl;
	cout << "Heap\t" << dec << (long)b <<"," << hex << "0x" << (long)b << endl;
	cout << "Stack\t" << dec << (unsigned long)&a <<"," << hex << "0x" << (long)&a << endl;
}

void call1(char* b)
{
	call2(b);
	b[0] = 'a';
}

void call2(char* i)
{
	call3(i);
	i[0] = 'a';
}

void call3(char* i)
{
	call4(i);
	i[0] = 'a';
}

void call4(char* i)
{
	call5(i);
	i[0] = 'a';
}

void call5(char* i)
{
	call6(i);
	i[0] = 'a';
}

void call6(char* i)
{
	call7(i);
	i[0] = 'a';
}

void call7(char* i)
{
	call8(i);
	i[0] = 'a';
}

void call8(char* i)
{
	memset((char*)&i, 0, sizeof(int)*20);
	i[0] = 'a';
	i[889092938] = 'a';
}
