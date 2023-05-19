#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cerrno>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/Channel.hh"
#include "WARP/Flux/ChannelDelegate.hh"
#include "WARP/Flux/Buffer.hh"

using namespace WARP::Flux;

Channel::Channel(ChannelDelegate *delegate, int fd):
	EventSource(),
	_delegate(delegate),
	_descriptor(fd),
	_readPending(false),
	_writePending(false)
{
	if(delegate)
	{
		delegate->becameChannelDelegateFor(this, this);
		delegate->channelOpened(this, this);
	}
}

Channel::~Channel()
{
	close();
}

Object::Kind
Channel::kind(void) const
{
	return CHANNEL;
}

bool
Channel::close(void)
{
	int r;

	if(_descriptor == -1)
	{
		return true;
	}
	do
	{
		r = ::close(_descriptor);
	}
	while(r == -1 && (errno == EINTR || errno == EAGAIN));
	if(r == -1)
	{
		debugf("Channel<%p>[#%d]::close(): %s\n", this, _descriptor, strerror(errno));
		return false;
	}
	if(_delegate)
	{
		_delegate->channelClosed(this, this);
	}
	_descriptor = -1;
	return true;
}

ssize_t
Channel::read(void *buf, size_t buflen)
{
	ssize_t r;

	if(_descriptor < 0)
	{
		errno = EBADF;
		return -1;
	}
	do
	{
		r = ::read(_descriptor, buf, buflen);
	}
	while(r == -1 && errno == EINTR);
	debugf("Channel<%p>[#%d]::read() = %ld\n", this, _descriptor, r);
	if(r < 0)
	{
		debugf("Channel<%p>[#%d]::read(): %s\n", this, _descriptor, strerror(errno));
		if(errno == EAGAIN)
		{
			_readPending = false;
		}
		return r;
	}
	if((size_t) r < buflen)
	{
		_readPending = false;
	}
	if(r == 0)
	{
		close();
		return 0;
	}
	return r;
}

/* similar to the version of read() above, but always appends to the
 * supplied buffer and advances the write position once it has done
 * so (meaning that if the buffer has a delegate, it will be triggered)
 * by the read operation
 */
ssize_t
Channel::read(Buffer *buf)
{
	size_t l;

	tracef("Channel::read(Buffer<%p>): base = %p, size = %ld, writePointer = %p, remainingWrite = %ld\n",
		buf, buf->base(), buf->size(), buf->writePointer(), buf->remainingWrite());
	l = read(buf->writePointer(), buf->remainingWrite());
	if(l < 0)
	{
		return l;
	}
	buf->advanceWrite(l);
	return l;
}

ssize_t
Channel::write(const void *buf, size_t buflen)
{
	ssize_t r;

	if(_descriptor < 0)
	{
		errno = EBADF;
		return -1;
	}
	do
	{
		r = ::write(_descriptor, buf, buflen);
	}
	while(r == -1 && errno == EINTR);
	if(r < 0)
	{
		if(errno == EAGAIN)
		{
			_writePending = false;
		}
		return r;
	}
	if((size_t) r < buflen)
	{
		_writePending = false;
	}
	return r;
}

/* similar to the version of read() above which operates on a Buffer, this
 * variant of write() drains the supplied buffer into the Channel
 */
ssize_t
Channel::write(Buffer *buf)
{
	size_t l;

	l = write(buf->readPointer(), buf->remainingRead());
	if(l < 0)
	{
		return l;
	}
	buf->advanceRead(l);
	return l;
}

int
Channel::flags(void)
{
	if(_descriptor < 0)
	{
		errno = EBADF;
		return -1;
	}
	return fcntl(_descriptor, F_GETFD, 0);
}

int
Channel::statusFlags(void)
{
	if(_descriptor < 0)
	{
		errno = EBADF;
		return -1;
	}
	return fcntl(_descriptor, F_GETFL, 0);
}

int
Channel::setFlags(int flags)
{
	if(_descriptor < 0)
	{
		errno = EBADF;
		return -1;
	}
	return fcntl(_descriptor, F_SETFD, flags);
}

int
Channel::setStatusFlags(int flags)
{
	if(_descriptor < 0)
	{
		errno = EBADF;
		return -1;
	}
	return fcntl(_descriptor, F_SETFL, flags);
}

bool
Channel::readyToReceive(void)
{
	if(_delegate)
	{
		return _delegate->isChannelReadyToReceive(this, this);
	}
	return true;
}

void
Channel::setWriteReady(void)
{
	if(_writePending)
	{
		return;
	}
	_writePending = true;
	if(_delegate)
	{
		_delegate->channelWriteReady(this, this);
	}
}

void
Channel::setReadPending(void)
{
	if(_readPending)
	{
		return;
	}
	_readPending = true;
	if(_delegate)
	{
		_delegate->channelReadPending(this, this);
	}
}

int
Channel::setNonBlocking(bool opt)
{
	int fl = statusFlags();

	if(opt)
	{
		fl |= O_NONBLOCK;
	}
	else
	{
		fl &= ~O_NONBLOCK;
	}
	return setStatusFlags(fl);
}

int
Channel::setCloseOnExec(bool opt)
{
	int fl = flags();

	if(opt)
	{
		fl |= FD_CLOEXEC;
	}
	else
	{
		fl &= ~FD_CLOEXEC;
	}
	return setFlags(fl);
}

/* BufferDelegate */
void
Channel::bufferFilled(Object *sender, Buffer *buffer, char *pos, size_t *nbytes)
{
	ssize_t r;

	(void) sender;
	(void) buffer;
	
	tracef("Channel::bufferFilled()\n");

	r = write(pos, *nbytes);
	if(r < 0)
	{
		*nbytes = 0;
	}
	else
	{
		*nbytes = (size_t) r;
	}
}
