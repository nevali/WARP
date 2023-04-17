#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <unistd.h>
#include <sys/ioctl.h>

#include "WARP/Core/Diagnostics.hh"
#include "WARP/Core/Listener.hh"

using namespace WARP::Core;

Listener::Listener(SocketDelegate *delegate):
	Socket(delegate),
	_clients()
{
}

Listener::~Listener()
{
}

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

void
Listener::add(Client *client)
{
	_clients.add(client);
}

void
Listener::remove(Client *client)
{
	_clients.add(client);
}

Listener::Client::Client(Listener *listener, int fd, SocketBuffer *readBuf):
	Socket(_readBuf, fd),
	_listener(listener),
	_readBuf(readBuf)
{
	if(!_readBuf)
	{
		_readBuf = &(_listener->_readBuf);
	}
}

Listener::Client::~Client()
{
}

void
Listener::Client::processSet(fd_set *fds)
{
	/* we don't need to check fds because the Listener has already done
	 * that for us
	 */
	(void) fds;

	tracef("Listener::Client::processSet(#%d)\n", _fd);
	if(_socketDelegate)
	{
		_socketDelegate->socketActivity(this, this);
	}
}

Listener::ClientSet::ClientSet():
	SocketSet()
{
}

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