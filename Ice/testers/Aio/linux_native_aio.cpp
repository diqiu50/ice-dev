#include <iostream>
#include <libaio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

using namespace std;


struct AioReq
{
	iocb mIocb;
	char*	mBuff;
	int64_t mOffset;
	int64_t mSize;

	AioReq(int filefd, int64_t offset, int64_t size, short flag):
		mBuff(NULL), mOffset(size), mSize(size)
	{
		posix_memalign((void**)&mBuff, sysconf(_SC_PAGESIZE), size);
		memset(&mIocb, 0, sizeof(iocb));
		mIocb.data = mBuff;
		mIocb.aio_lio_oprcode = flag;
		mIocb.aio_reqprio = 0;
		mIocb.aio_fildes = filefd;
		mIocb.u.c.buf = mBuff;
		mIocb.u.c.nbytes = (size/sysconf(_SC_PAGESIZE)+1)*sysconf(_SC_PAGESIZE);
		mIocb.u.c.offset = 0;
	}

	~AioReq()
	{
		free(mBuff);
		mBuff = 0;
	}
};

int main(int argc, char** argv)
{
	io_context_t context;
	unsigned int nr_events = 10;
	memset(&context, 0, sizeof(io_context_t));

	int rslt = io_setup(nr_events, &context);
	if (rslt != 0)
		cout << "error " << strerror(errno) << endl;

	int filefd = open("a000.txt", O_CREAT|O_DIRECT|O_RDWR, S_IRWXU|S_IRWXG|S_IROTH);
	if (filefd < 0)
		cout << "error " << strerror(errno) << endl;
	
	char *buff; 
	posix_memalign((void**)&buff, sysconf(_SC_PAGESIZE), sysconf(_SC_PAGESIZE));
	//strcpy(buff, "hello xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

		
	iocb *io_cb = (iocb*)malloc(sizeof(iocb));
	memset(io_cb, 0, sizeof(iocb));
	io_prep_pread(io_cb, filefd, (void*)buff, sysconf(_SC_PAGESIZE), 0);
	rslt = io_submit(context, 1, &io_cb);

	io_event events[nr_events];

	int n = io_getevents(context, 1, nr_events, events, 0);
	for (int i=0; i<n; i++)
	{
		io_event &event = events[i];
		iocb *cb = event.obj;
		if (event.res!=sysconf(_SC_PAGESIZE) || event.res2 != 0)
		{
			cout << "length" << event.res << endl;
			cout << "data: " << (char*)cb->u.c.buf << endl;
			free(cb->u.c.buf);
			close(cb->aio_fildes);
			free(cb);
		}
		else
		{
			cb->u.c.offset += event.res;
			io_submit(context, 1, &cb);			
		}
	}

	return 0;
}
