#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstdlib>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/SocketDelegate.hh"
#include "WARP/Socket.hh"

using namespace WARP;

void
SocketDelegate::socketOpened(Socket *socket)
{
	tracef("SocketDelegate: socket #%d opened\n", socket->id());
}

void
SocketDelegate::socketClosed(Socket *socket)
{
	tracef("SocketDelegate: socket #%d closed\n", socket->id());
}

/* Invoked when there's activity on a socket (client or listener) */
void
SocketDelegate::socketActivity(Socket *socket)
{
	tracef("SocketDelegate: activity on socket #%d\n", socket->id());
}

/* Invoked when data has been read from a socket */
void
SocketDelegate::socketReadBuffer(Socket *socket, const void *buf, size_t buflen)
{
	(void) socket;
	(void) buf;

	tracef("SocketDelegate: read %lu bytes from socket #%d\n", (unsigned long) buflen, socket->id());
}
