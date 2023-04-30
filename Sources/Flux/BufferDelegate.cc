#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/BufferDelegate.hh"

using namespace WARP::Flux;

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
