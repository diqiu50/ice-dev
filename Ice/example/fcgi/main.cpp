#include <fcgi_stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int count = 0;
	while(FCGI_Accept() >= 0)
	{
		printf("Content-type: text/html\r\n\r\n");
		printf("Hello Word , the %d visitor!", ++count);
		FCGI_Finish();
	}
}
