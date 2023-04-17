
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <fcntl.h>
#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"
#include "WARP/Core/Pipe.hh"

using namespace WARP::Core;

static bool
setCloseOnExec(int fd)
{
	int flags;
	
	flags = fcntl(fd, F_GETFD, 0);
	if(flags < 0)
	{
		perror("fcntl(F_GETFD)");
		return false;
	}
//	debugf("Pipe::setCloseOnExec: flags = %d\n", flags);
	flags |= FD_CLOEXEC;
	if(fcntl(fd, F_SETFD, flags))
	{
		perror("fcntl(F_SETFD)");
		return false;
	}
	return true;
}

Pipe::Pipe()
{
	int r;
	
	do
	{
		r = pipe(_filedes);
	}
	while(r && errno == EINTR);
	if(r)
	{
		perror("pipe");
		_filedes[0] = -1;
		_filedes[1] = -1;
		return;
	}
	debugf("%s: pipe opened { %d, %d }\n", __FUNCTION__, _filedes[0], _filedes[1]);
	setCloseOnExec(_filedes[0]);
	setCloseOnExec(_filedes[1]);
}

Pipe::~Pipe()
{
	debugf("%s: closing pipe { %d, %d }\n", __FUNCTION__, _filedes[0], _filedes[1]);
	close(_filedes[0]);
	close(_filedes[1]);
}

int
Pipe::receiver(void) const
{
	return _filedes[1];
}

int
Pipe::emitter(void) const
{
	return _filedes[0];
}
