#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstdlib>

#include "WARP/Core/Diagnostics.hh"
#include "WARP/Core/SocketDelegate.hh"
#include "WARP/Core/Socket.hh"

using namespace WARP::Core;

void
SocketDelegate::socketOpened(Object *sender, Socket *socket)
{
	tracef("SocketDelegate: socket #%d opened\n", socket->id());
}

void
SocketDelegate::socketClosed(Object *sender, Socket *socket)
{
	tracef("SocketDelegate: socket #%d closed\n", socket->id());
}

/* Invoked when there's activity on a socket (client or listener) */
void
SocketDelegate::socketActivity(Object *sender, Socket *socket)
{
	tracef("SocketDelegate: activity on socket #%d\n", socket->id());
}

/* Invoked when data has been read from a socket */
void
SocketDelegate::socketReadBuffer(Object *sender, Socket *socket, const void *buf, size_t *buflen)
{
	(void) socket;
	(void) buf;

	tracef("SocketDelegate: read %lu bytes from socket #%d\n", (unsigned long) *buflen, socket->id());
}
