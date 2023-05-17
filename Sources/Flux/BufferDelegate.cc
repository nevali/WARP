#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/BufferDelegate.hh"
#include "WARP/Flux/Buffer.hh"

using namespace WARP::Flux;

BufferDelegate::~BufferDelegate()
{
	if(_bufferDelegateFor)
	{
		tracef("BufferDelegate::%s() ceasing to be delegate for Buffer<%p>\n", __FUNCTION__, _bufferDelegateFor);
		_bufferDelegateFor->clearBufferDelegate(this);
		_bufferDelegateFor = NULL;
	}
}

void
BufferDelegate::becameBufferDelegateFor(Object *sender, Buffer *buffer)
{
	(void) sender;

	_bufferDelegateFor = buffer;
	tracef("BufferDelegate::%s() are now delegate for Buffer<%p>\n", __FUNCTION__, _bufferDelegateFor);
}

void
BufferDelegate::bufferFilled(Object *sender, Buffer *buffer, char *base, size_t *nbytes)
{
	(void) sender;
	(void) buffer;
	(void) base;

	tracef("BufferDelegate::bufferFilled() %ld bytes available in buffer\n", *nbytes);
}

void
BufferDelegate::bufferDrained(Object *sender, Buffer *buffer, char *base, size_t nbytes)
{
	(void) sender;
	(void) buffer;
	(void) base;

	tracef("BufferDelegate::bufferDrained() %ld bytes consumed from buffer\n", nbytes);
}

void
BufferDelegate::sourceOpened(Object *sender, Object *source)
{
	tracef("BufferDelegate: Source<%p> opened (Sender = Object<%p>)\n", source, sender);
}

void
BufferDelegate::sourceClosed(Object *sender, Object *source)
{
	tracef("BufferDelegate: Source<%p> closed (Sender = Object<%p>)\n", source, sender);
}
