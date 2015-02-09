#include <iostream>
#include <thread>

using namespace std;

void test()
{
	static thread_local int i;
	cout << "xxxx" << endl;
}

int main()
{
	thread t(test);
	t.join();
	return 0;
}
