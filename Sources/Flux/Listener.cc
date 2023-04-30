#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <unistd.h>
#include <sys/ioctl.h>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/Listener.hh"
#include "WARP/Flux/ChannelDelegate.hh"

using namespace WARP::Flux;

Listener::Listener(ChannelDelegate *delegate):
	Socket(delegate),
	_clients()
{
}

Listener::~Listener()
{
}

#if BROKEN
void
Listener::addSet(fd_set *set)
{
	if(_fd < 0)
	{
		return;
	}
	FD_SET(_fd, set);
	_clients.addSet(set);
}

void
Listener::processSet(fd_set *set)
{
	if(_fd < 0)
	{
		return;
	}
	tracef("Listener: processing started\n");
	if(FD_ISSET(_fd, set))
	{
		tracef("Listener: new connection!\n");
		accept();
	}
	_clients.processSet(set);
	tracef("Listener: processing finished\n");
}
#endif

void
Listener::add(Client *client)
{
	_clients->add(client);
}

void
Listener::remove(Client *client)
{
	_clients->remove(client);
}

Listener::Client::Client(Listener *listener, int fd):
	Socket(listener->channelDelegate(), fd),
	_listener(listener)
{
	/* note that the Listener is not retained because its lifespan
	 * will always exceed that of the Client
	 */
}

Listener::Client::~Client()
{
}

void
Listener::Client::processSet(fd_set *fds)
{
	ChannelDelegate *delegate;

	/* we don't need to check fds because the Listener has already done
	 * that for us
	 */
	(void) fds;

	tracef("Listener::Client::processSet(#%d)\n", descriptor());
	if((delegate = channelDelegate()))
	{
		delegate->channelActivity(this, this);
	}
}

Listener::ClientSet::ClientSet():
	ChannelSet()
{
}

#if BROKEN
void
Listener::ClientSet::addSet(fd_set *set)
{
	tracef("Listener::ClientSet::addSet: nclients = %d\n", (int) _nclients);
	for(size_t n = 0; n < _nclients; n++)
	{
		Listener::Client *client = reinterpret_cast<Listener::Client *>(_clients[n]);

		if(client && client->_fd >= 0)
		{
			FD_SET(client->_fd, set);
		}
	}
}

void
Listener::ClientSet::processSet(fd_set *set)
{
	tracef("Listener::ClientSet::processSet: nclients = %d\n", (int) _nclients);
	for(size_t n = 0; n < _nclients; n++)
	{
		Listener::Client *client = reinterpret_cast<Listener::Client *>(_clients[n]);

		/* avoid a virtual method call in a tight loop when there's no
		* activity on the socket
		*/
		if(client && client->_fd >= 0 && FD_ISSET(client->_fd, set))
		{
			debugf("Listener::ClientSet: activity on connection #%d\n", client->_fd);
			client->processSet(set);
		}
	}
}
#endif
