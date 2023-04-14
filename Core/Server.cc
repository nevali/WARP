#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <sys/select.h>
#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Server.hh"

using namespace WARP;

Server::Server():
	_listeners()
{
}

Server::~Server()
{
}

void
Server::add(Listener *listener)
{
	_listeners.add(listener);
}

int
Server::run(void)
{
	while(true)
	{
		fd_set fds;

		FD_ZERO(&fds);
		_listeners.addSet(&fds);
		::select(FD_SETSIZE, &fds, NULL, NULL, NULL);
		_listeners.processSet(&fds);
	}
}
