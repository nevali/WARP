#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "WARP/Flux/Object.hh"

using namespace WARP::Flux;

Object::Object():
	_refCount(1)
{
}

Object::~Object()
{
}

unsigned
Object::retain(void)
{
	_refCount++;
	return _refCount;
}

unsigned
Object::release(void)
{
	if(_refCount == 1)
	{
		_refCount--;
		delete this;
		return 0;
	}
	_refCount--;
	return _refCount;
}

Object::Kind
Object::kind(void) const
{
	return OBJECT;
}