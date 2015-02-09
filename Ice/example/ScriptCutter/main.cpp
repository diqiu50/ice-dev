#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "IceUtil/NumStr.h"
#include "IceUtil/Type.h"

using namespace std;

const I32 MAX_LINE = 20;
const I8 * const DEF_FILENAME = "typescript";

int main()
{
	string cmd = "rm -f ";
	cmd = cmd + DEF_FILENAME + "_*";
	I32 rslt = system(cmd.data());
	cout << rslt << endl;
	if (rslt == -1)
		return 0;
	ifstream inFile(DEF_FILENAME, ios::binary);
	ofstream outFile;
 	
	I32 lines = 0;
	I32 fileSeqno = 0;
	I8 c;

	while(inFile.is_open())
	{
		if (lines == 0)
		{
			string filename = DEF_FILENAME;
			filename = filename + "_" + ntos(fileSeqno++);
			outFile.open(filename.data(),ios::binary);
			lines++;
		}
		if ((c=inFile.get()) == '\n')
			lines++;

		if (!inFile.eof())
			outFile.put(c);
		else
			break;

		if (lines == MAX_LINE+1)
		{
			lines = 0;
			outFile.close();
		}
	}
}
