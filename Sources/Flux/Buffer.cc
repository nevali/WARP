#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>
#include <cstring>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/Buffer.hh"
#include "WARP/Flux/BufferDelegate.hh"
#include "WARP/Flux/Channel.hh"

using namespace WARP::Flux;

Buffer::Buffer(BufferDelegate *delegate, size_t nbytes):
	_bufferDelegate(delegate),
	_size(nbytes),
	_readPos(0),
	_writePos(0)
{
	_base = (char *) ::calloc(1, _size);
	if(!_base)
	{
		abort();
	}
	if(_bufferDelegate)
	{
		_bufferDelegate->becameBufferDelegateFor(this, this);
	}
}

Buffer::~Buffer()
{
	::free(_base);
}

Object::Kind
Buffer::kind(void) const
{
	return BUFFER;
}

void
Buffer::reset(void)
{
	::memset(_base, 0, _size);
	_readPos = 0;
	_writePos = 0;
};

bool
Buffer::advanceRead(size_t bytes)
{
	if(_readPos + bytes <= _size)
	{
		tracef("Buffer::advanceRead(): %lu bytes consumed from buffer\n", bytes);
		_readPos += bytes;
		return true;
	}
	return false;
}

bool
Buffer::advanceWrite(size_t bytes)
{
	if(_writePos + bytes  <= _size)
	{
		tracef("Buffer::advanceWrite(): %lu bytes appended to buffer\n", bytes);
		_writePos += bytes;
		return true;
	}
	return false;
}

void
Buffer::rewindRead(size_t bytes)
{
	if(bytes > _readPos)
	{
		_readPos = 0;
	}
	else
	{
		_readPos -= bytes;
	}
}

void
Buffer::rewindWrite(size_t bytes)
{
	if(bytes > _writePos)
	{
		_writePos = 0;
	}
	else
	{
		_writePos -= bytes;
	}
}

void
Buffer::rewindRead(void)
{
	_readPos = 0;
}

void
Buffer::rewindWrite(void)
{
	_writePos = 0;
}

void
Buffer::drain(Object *sender)
{
	size_t size, consumed;

	if(!_bufferDelegate)
	{
		debugf("Buffer::drain(): no delegate to receive buffer\n");
		reset();
		return;
	}
	tracef("Buffer::drain(): size=%lu, writePosition=%lu\n", _size, _writePos);
	/* retain ourselves to ensure that the delegate can't trigger our premature destruction */
	retain();
	size = _writePos;
	rewindRead();
	while(_readPos < _writePos)
	{
		consumed = size;
		debugf("Buffer::drain(): readPosition=%lu, writePosition=%lu, size=%lu\n", _readPos, _writePos, consumed);
		_bufferDelegate->bufferFilled(sender, this, _base + _readPos, &consumed);
		if(consumed > size)
		{
			consumed = size;
		}
		advanceRead(consumed);
		debugf("Buffer::drain(): %lu bytes were consumed by delegate\n", consumed);
		if(!consumed)
		{
			break;
		}
		size -= consumed;
		debugf("Buffer::drain(): advancing read cursor by %lu bytes\n", consumed);
	}
	consume(_readPos);
	debugf("Buffer::drain(): %lu bytes remain\n", size);
	release();
}

/* consume nbytes from the start of the buffer, moving whatever remains to
 * the start of the buffer and updating the read and write positions
 * accordingly
 * 
 * it's not possible to consume beyond the current write position
 * if the read position is within the consumed region, it will be moved to the
 * start
 */
void
Buffer::consume(size_t nbytes)
{
	tracef("Buffer::consume(%lu) size=%lu, readPosition=%lu, writePosition=%lu\n", nbytes, _size, _readPos, _writePos);
	if(nbytes > _writePos)
	{
		nbytes = _writePos;
	}
	if(!nbytes)
	{
		return;
	}
	memmove(_base, &_base[nbytes], _size - nbytes);
	if(_readPos < nbytes)
	{
		_readPos = 0;
	}
	else
	{
		_readPos -= nbytes;
	}
	_writePos -= nbytes;
	tracef("Buffer::consume(%lu) new readPosition=%lu, writePosition=%lu\n", nbytes, _readPos, _writePos);
}

/* ChannelDelegate */
void
Buffer::channelReadPending(Object *sender, Channel *channel)
{
	ssize_t l;

	tracef("Buffer<%p>: Channel<%p>[#%d] is ready for read (Sender = Object<%p>)\n", this, channel, channel->descriptor(), sender);
	while(channel->readPending() && remainingWrite())
	{
		l = channel->read(this);
		debugf("Buffer::channelReadPending(): read => %ld\n", l);
		if(l < 0)
		{
			break;
		}
		if(!remainingWrite() && _bufferDelegate)
		{
			debugf("Buffer::channelReadPending(): buffer is full, draining\n");
			drain(sender);
		}
		debugf("Buffer::channelReadPending(): remainingWrite = %lu\n", remainingWrite());
	}
	if(writePosition() && _bufferDelegate)
	{
		debugf("Buffer::channelReadPending(): draining remainder of buffer\n");
		drain(sender);
	}
}

bool
Buffer::isChannelReadyToReceive(Object *sender, Channel *channel)
{
	bool ready;

	ready = remainingWrite() ? true : false;
	tracef("Buffer<%p>: Channel<%p>[#%d] %s ready to receive (Sender = Object<%p>); remainingWrite = %lu\n", this, channel, channel->descriptor(), (ready ? " IS " : " IS NOT "), sender, remainingWrite());
	return ready;
}

void
Buffer::channelOpened(Object *sender, Channel *channel)
{
	if(_bufferDelegate)
	{
		_bufferDelegate->sourceOpened(sender, channel);
	}
}

void
Buffer::channelClosed(Object *sender, Channel *channel)
{
	if(_bufferDelegate)
	{
		_bufferDelegate->sourceClosed(sender, channel);
	}
}
