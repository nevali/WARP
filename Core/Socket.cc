#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cerrno>

#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"
#include "WARP/Core/Socket.hh"

using namespace WARP::Core;

Socket::Socket(SocketDelegate *delegate, int fd):
	Object(),
	_fd(fd),
	_socketDelegate(delegate)
{
	if(_socketDelegate && _fd >= 0)
	{
		_socketDelegate->socketOpened(this, this);
	}
}

Socket::~Socket()
{
	close();
	if(_fd >= 0)
	{
		::close(_fd);
	}
}

int
Socket::close(void)
{
	int r;

	if(_fd < 0)
	{
		return 0;
	}
	do
	{
		r = ::close(_fd);
	}
	while(r == -1 && errno == EINTR);
	if(r < 0)
	{
		return r;
	}
	if(_socketDelegate)
	{
		_socketDelegate->socketClosed(this, this);
	}
	_fd = -1;
	return 0;
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
		if(_socketDelegate)
		{
			_socketDelegate->socketActivity(this, this);
		}
	}
}

int
Socket::id(void) const
{
	return _fd;
}
