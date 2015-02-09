#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

using namespace std;

int main()
{
	fstream file("a.txt",fstream::binary | fstream::binary | fstream::in | fstream::out);
	
	for (int i = 0; i < 50; i++)
		file.put(i);

	int offset = 10;
	char buff[10];
	file.seekg(offset);
	file.read(buff,10);

	for (int i=0; i<10;i++)
	{
		cout << (int)buff[i] << endl;
	}
	cout << endl;

	for (int i=0; i<10;i++)
	{
		buff[i]+=100;
	}

	cout << endl;

	file.seekp(offset);
	file.write(buff, 10);
	file.close();

	fstream file2("a.txt",fstream::binary | fstream::app| fstream::in | fstream::out);
	for (int i = 0; i<100; i++)
	{
		char b;
		file2.get(b);
		cout << (int)b << endl;
	}
	file2.close();
	return 0;
}

