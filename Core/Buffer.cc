#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>
#include <cstring>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Buffer.hh"

using namespace WARP;

Buffer::Buffer(size_t nbytes):
	_size(nbytes)
{
	_base = (char *) ::calloc(1, _size);
	if(!_base)
	{
		abort();
	}
}

Buffer::~Buffer()
{
	free(_base);
}

void
Buffer::reset(void)
{
	::memset(_base, 0, _size);
};
