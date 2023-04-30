#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>
#include <cstring>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/Buffer.hh"

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
}

Buffer::~Buffer()
{
	::free(_base);
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
	if(_readPos + bytes < _size)
	{
		_readPos += bytes;
		return true;
	}
	return false;
}

bool
Buffer::advanceWrite(size_t bytes)
{
	if(_writePos + bytes < _size)
	{
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
Buffer::drain(void)
{
	size_t size, consumed;
	char *ptr;

	if(!_bufferDelegate)
	{
		debugf("Buffer::drain(): no delegate to receive buffer\n");
		reset();
		return;
	}
	size = _writePos;
	_readPos = 0;
	do
	{
		consumed = size;
		_bufferDelegate->bufferWritten(this, this, _base + _readPos, &consumed);
		if(consumed > size)
		{
			consumed = size;
		}
		if(!consumed)
		{
			break;
		}
		size -= consumed;
		debugf("Buffer::drain(): advancing buffer by %lu bytes\n", consumed);
		_readPos += consumed;
	}
	while(size);
	debugf("Buffer:drain: %lu bytes remain\n", size);

}