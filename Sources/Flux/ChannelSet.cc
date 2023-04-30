#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <sys/types.h>
#include <sys/select.h>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/ChannelSet.hh"
#include "WARP/Flux/Channel.hh"
#include "WARP/Flux/ChannelDelegate.hh"

using namespace WARP::Flux;

ChannelSet::ChannelSet():
	TArray<Channel>(),
	_maxFd(0),
	_children(NULL)
{
}

ChannelSet::~ChannelSet()
{
	if(_children)
	{
		_children->release();
	}
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

size_t
ChannelSet::add(Channel *channel)
{
	if(!channel)
	{
		debugf("ChannelSet::add(): attempt to add NULL Channel\n");
		return NOTFOUND;
	}
	if(channel->descriptor() < 0)
	{
		debugf("ChannelSet::add(): attempt to add closed Channel<%p>\n", channel);
		return NOTFOUND;
	}
	if(channel->setNonBlocking() < 0)
	{
		debugf("ChannelSet::add(): failed to set Channel<%p>[#%d] as non-blocking: %s\n", channel, channel->descriptor(), strerror(errno));
	}
	tracef("ChannelSet::add(): Channel<%p>[#%d]\n", channel, channel->descriptor());
	return TArray<Channel>::add(channel);
}

void
ChannelSet::processEventsWithTimeout(struct timeval *tv)
{
	int r;

	if(tv)
	{
		tracef("ChannelSet::processEventsWithTimeout({ %d, %d })\n", (int) tv->tv_sec, (int) tv->tv_usec);
	}
	else
	{
		tracef("ChannelSet::processEventsWithTimeout(NULL)\n");
	}
	_maxFd = 0;
	FD_ZERO(&_readers);
	FD_ZERO(&_writers);
	FD_ZERO(&_errors);

	forEachPointer(addChannelCallback, this);

	r = ::select(_maxFd, &_readers, &_writers, &_errors, tv);

	tracef("ChannelSet::processEventsWithTimeout(): select = %d\n", r);
	if(r < 0)
	{
		debugf("select() failed: %s\n", strerror(errno));
		/* XXX throw */
		return;
	}
	if(r == 0)
	{
		/* timer expired */
		return;
	}
	forEachPointer(processChannelCallback, this);
	if(_children)
	{
//		_children->forEachPointer(processChildrenCallback, this);
	}
}

bool
ChannelSet::addChannelCallback(void *ptr, void *ctx)
{
	Channel *channel;
	ChannelSet *set;
	int fd;

	channel = static_cast<Channel *>(ptr);
	set = static_cast<ChannelSet *>(ctx);

	fd = channel->descriptor();
	if(fd < 0)
	{
		return true;
	}
	tracef("ChannelSet::addChannelCallback(Channel<%p>[#%d], ChannelSet<%p>)\n", channel, fd, set);
	if(fd + 1 > set->_maxFd)
	{
		set->_maxFd = fd + 1;
	}
	FD_SET(fd, &(set->_errors));
	if(channel->readyToReceive())
	{
		FD_SET(fd, &(set->_readers));
	}
	if(!channel->writeReady())
	{
		FD_SET(fd, &(set->_writers));
	}
	return true;
}

bool
ChannelSet::processChannelCallback(void *ptr, void *ctx)
{
	Channel *channel;
	ChannelSet *set;
	int fd;

	channel = static_cast<Channel *>(ptr);
	set = static_cast<ChannelSet *>(ctx);

	fd = channel->descriptor();
	tracef("ChannelSet::processChannelCallback(Channel<%p>[#%d], ChannelSet<%p>)\n", channel, fd, set);

	if(FD_ISSET(fd, &(set->_errors)))
	{
		tracef("ChannelSet::processChannelCallback(): FD_ISSET(errors) is true for Channel<%p>(#%d)\n", channel, fd);
		channel->close();
		return true;
	}
	if(FD_ISSET(fd, &(set->_readers)))
	{
		channel->setReadPending();
	}
	if(FD_ISSET(fd, &(set->_writers)))
	{
		channel->setWriteReady();
	}
	return true;
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
