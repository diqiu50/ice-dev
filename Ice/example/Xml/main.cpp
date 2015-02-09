#include <iostream>
#include <libxml++-2.6/libxml++/libxml++.h>
#include <glibmm/ustring.h>
#include <stdio.h>
#include <time.h>

using namespace std;
using namespace xmlpp;
using namespace Glib;

int i = 0;

int main()
{
	cout << CLOCKS_PER_SEC << endl;
	const char *a = "aaaa";
	char *b = new char;

	cout << "Code\t" << (int)main <<"," << hex << "0x" << (int)main << endl;
	cout << "Data\t" << dec << (int)a <<"," << hex << "0x" << (int)a << endl;
	cout << "BBS\t" << dec << (int)&i <<"," << hex << "0x" << (int)&i << endl;
	cout << "Heap\t" << dec << (int)b <<"," << hex << "0x" << (int)b << endl;
	cout << "Stack\t" << dec << (unsigned int)&a <<"," << hex << "0x" << (int)&a << endl;

	cout << time(NULL) << endl;

	locale::global(locale(""));
	string xml = "<node attr=\"a1\" />";
	string uri;
	TextReader reader((unsigned char*)xml.data(), xml.size(),uri);
	reader.read();
	Node *node = reader.get_current_node();
	cout << node->get_name() << endl;
}

