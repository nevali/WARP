#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Core/SocketBuffer.hh"
#include "WARP/Core/Socket.hh"

using namespace WARP::Core;

SocketBuffer::SocketBuffer(BufferDelegate *delegate, size_t nbytes, bool autoDrain):
	Buffer(delegate, nbytes),
	_autoDrain(autoDrain)
{
}

SocketBuffer::~SocketBuffer()
{
}

void
SocketBuffer::socketActivity(Object *sender, Socket *socket)
{
	int fd, len;
	size_t size;
	ssize_t r;

	fd = socket->id();
	tracef("SocketBuffer::socketActivity(): activity on socket #%d\n", fd);
	len = 0;
	if(::ioctl(fd, FIONREAD, &len) < 0)
	{
		/* XXX EINTR */
		debugf("SocketBuffer::socketActivity: ioctl() failed on connection #%d\n", fd);
		return;
	}
	if(len <= 0)
	{
		debugf("SocketBuffer::socketActivity: len = %d\n", len);
//		socket->close();
		return;
	}
	size = remainingWrite();
	if((size_t) len > size)
	{
		len = size;
	}
	debugf("SocketBuffer::socketActivity: reading %d bytes from connection #%d\n", len, fd);
	do
	{
		r = ::recv(fd, writePointer(), len, MSG_WAITALL);
	}
	while(r == -1 && (errno == EINTR || errno == EAGAIN));
	if(r == -1)
	{
		debugf("SocketBuffer::socketActivity: recv failed on connection #%d: %s\n", fd, strerror(errno));
//		socket->close();
		return;
	}
	debugf("SocketBuffer::socketActivity: read %ld bytes from connection #%d\n", (long) r, fd);
	advanceWrite(r);
}
