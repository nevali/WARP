#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "WARP/Core/Object.hh"

using namespace WARP::Core;

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
		delete this;
		return 0;
	}
	_refCount--;
	return _refCount;
}