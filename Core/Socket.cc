#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Socket.hh"

using namespace WARP;

Socket::Socket(SocketDelegate *delegate, int fd):
	_fd(fd),
	_delegate(delegate)
{
}

Socket::~Socket()
{
	if(_fd >= 0)
	{
		::close(_fd);
	}
}

void
Socket::addSet(fd_set *fds)
{
	if(_fd >= 0)
	{
		FD_SET(_fd, fds);
	}
}

void
Socket::processSet(fd_set *fds)
{
	if(_fd >= 0 && FD_ISSET(_fd, fds))
	{
		if(_delegate)
		{
			_delegate->socketActivity(this);
		}
	}
}

int
Socket::id(void)
{
	return _fd;
}
