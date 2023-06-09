#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdlib>
#include <cstring>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/Array.hh"

using namespace WARP::Flux::Internal;
using namespace WARP::Flux::Diagnostics;
using WARP::Flux::Object;

struct WARP::Flux::Internal::ArrayData
{
	static const int BLOCKSIZE = 16;

	size_t size;
	size_t count;
	void **ptr;
	bool destructing;

	ArrayData(): size(0), count(0), ptr(NULL), destructing(false) { }

	virtual ~ArrayData()
	{
		::free(ptr);
	}
	void expand(void);
};

Array::Array()
{
	_data = new ArrayData();
}

void
Array::objectWillBeDestroyed(void)
{
	_data->destructing = true;
	for(size_t index = 0; index < _data->size; index++)
	{
		if(_data->ptr[index])
		{
			pointerWasRemoved(_data->ptr[index]);
		}
	}
}

Array::~Array()
{
	_data->destructing = true;
	delete _data;
}

Object::Kind
Array::kind(void) const
{
	return ARRAY;
}

size_t
Array::count(void) const
{
	return _data->count;
}

size_t
Array::indexOfPointer(void *ptr) const
{
	if(!ptr || _data->destructing)
	{
		return Array::NOTFOUND;
	}
	for(size_t index = 0; index < _data->size; index++)
	{
		if(_data->ptr[index] == ptr)
		{
			return index;
		}
	}
	return NOTFOUND;
}

void *
Array::pointerAtIndex(size_t index) const
{
	if(_data->destructing)
	{
		return NULL;
	}
	if(index >= _data->size)
	{
		return NULL;
	}
	return _data->ptr[index];
}

size_t
Array::addPointer(void *ptr)
{
	size_t index;

	if(_data->destructing)
	{
		return NOTFOUND;
	}
	for(index = 0; index < _data->size; index++)
	{
		if(NULL == _data->ptr[index])
		{
			_data->ptr[index] = ptr;
			_data->count++;
			return index;
		}
	}
	_data->expand();
	_data->ptr[index] = ptr;
	_data->count++;
	return index;
}

bool
Array::removePointer(void *ptr)
{
	size_t index;
	bool removed;

	if(_data->destructing)
	{
		return false;
	}
	removed = false;
	for(index = 0; index < _data->size; index++)
	{
		if(ptr == _data->ptr[index])
		{
			_data->ptr[index] = NULL;
			if(!removed)
			{
				pointerWasRemoved(ptr);
				removed = true;
				_data->count--;
			}
		}
	}
	return removed;
}

void
Array::pointerWasRemoved(void *ptr) const
{
	(void) ptr;
}

void
Array::forEachPointer(bool (*callback)(void *ptr, void *ctx), void *ctx)
{
	size_t index;

	for(index = 0; index < _data->size; index++)
	{
		if(_data->ptr[index])
		{
			callback(_data->ptr[index], ctx);
		}
	}
}

void
ArrayData::expand(void)
{
	void **p;

	p = (void **) ::realloc(ptr, sizeof(void *) * (size + BLOCKSIZE));
	if(!p)
	{
		::abort();
	}
	ptr = p;
	p = &ptr[size];
	::memset(p, 0, sizeof(void *) * BLOCKSIZE);
	size += BLOCKSIZE;
}
