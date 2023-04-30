#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstdlib>

#include <sys/types.h>
#include <sys/select.h>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/ChannelSet.hh"
#include "WARP/Flux/Channel.hh"

using namespace WARP::Flux;

ChannelSet::ChannelSet():
	TArray<Channel>()
{
}

ChannelSet::~ChannelSet()
{
}

unsigned
ChannelSet::retain()
{
	return TArray<Channel>::retain();
}

unsigned
ChannelSet::release()
{
	return TArray<Channel>::release();
}

Object::Kind
ChannelSet::kind(void) const
{
	return CHANNELSET;
}

void
ChannelSet::processEventsWithTimeout(struct timeval *tv)
{
	::select(0, NULL, NULL, NULL, tv);
}

void
ChannelSet::processPendingEvents(void)
{
	struct timeval tv;

	tv.tv_sec = 0;
	tv.tv_usec = 0;
	processEventsWithTimeout(&tv);
}

#if BROKEN

SocketSet::SocketSet():
	_clients(NULL),
	_nclients(0)
{
}

SocketSet::~SocketSet()
{
	for(size_t n = 0; n < _nclients; n++)
	{
		delete _clients[n];
	}
	free(_clients);
}

void
SocketSet::add(Socket *client)
{
	Socket **p;

	for(size_t n = 0; n < _nclients; n++)
	{
		if(_clients[n] == client)
		{
			return;
		}
	}
	for(size_t n = 0; n < _nclients; n++)
	{
		if(!_clients[n])
		{
			_clients[n] = client;
			return;
		}
	}
	p = (Socket **) realloc(_clients, sizeof(Socket *) * (_nclients + 2));
	if(!p)
	{
		abort();
	}
	_clients = p;
	p[_nclients] = client;
	_nclients++;
	p[_nclients] = NULL;
}

void
SocketSet::remove(Socket *client)
{
	(void) client;

	for(size_t n = 0; n < _nclients; n++)
	{
		if(_clients[n] == client)
		{
			_clients[n] = NULL;
		}
	}
}

void
SocketSet::addSet(fd_set *set)
{
	tracef("SocketSet::addSet: nclients = %d\n", (int) _nclients);
	for(size_t n = 0; n < _nclients; n++)
	{
		if(_clients[n] && _clients[n]->_fd >= 0)
		{
			_clients[n]->addSet(set);
		}
	}
}

void
SocketSet::processSet(fd_set *set)
{
	tracef("SocketSet::processSet: nclients = %d\n", (int) _nclients);
	for(size_t n = 0; n < _nclients; n++)
	{
		if(_clients[n] && _clients[n]->_fd >= 0)
		{
			_clients[n]->processSet(set);
		}
	}
}

#endif
