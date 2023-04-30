#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#if BROKEN

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/ChannelBuffer.hh"
#include "WARP/Flux/Channel.hh"

using namespace WARP::Flux;

ChannelBuffer::ChannelBuffer(BufferDelegate *delegate, size_t nbytes, bool autoDrain):
	Buffer(delegate, nbytes),
	_autoDrain(autoDrain)
{
}

ChannelBuffer::~ChannelBuffer()
{
}

void
ChannelBuffer::channelActivity(Object *sender, Channel *channel)
{
	int fd, len;
	size_t size;
	ssize_t r;

	(void) sender;
	
	fd = channel->descriptor();
	tracef("ChannelBuffer::channelActivity(): activity on channel #%d\n", fd);
	len = 0;
	/* XXX THIS ONLY WORKS ON SOCKETS!! -- MOVE TO SOCKET IMPLEMENTATION */
	if(::ioctl(fd, FIONREAD, &len) < 0)
	{
		/* XXX EINTR */
		debugf("ChannelBuffer::channelActivity: ioctl() failed on connection #%d\n", fd);
		return;
	}
	if(len <= 0)
	{
		debugf("ChannelBuffer::channelActivity: len = %d\n", len);
//		channel->close();
		return;
	}
	size = remainingWrite();
	if((size_t) len > size)
	{
		len = size;
	}
	debugf("ChannelBuffer::channelActivity: reading %d bytes from connection #%d\n", len, fd);
	do
	{
		/* XXX do we really want MSG_WAITALL here?? */
		r = ::recv(fd, writePointer(), len, MSG_WAITALL);
	}
	while(r == -1 && (errno == EINTR || errno == EAGAIN));
	if(r == -1)
	{
		debugf("ChannelBuffer::channelActivity: recv failed on connection #%d: %s\n", fd, strerror(errno));
//		channel->close();
		return;
	}
	debugf("ChannelBuffer::channelActivity: read %ld bytes from connection #%d\n", (long) r, fd);
	advanceWrite(r);
}

#endif