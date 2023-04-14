#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstdlib>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/SocketDelegate.hh"

using namespace WARP;

/* Invoked when there's activity on a socket (client or listener) */
void
SocketDelegate::socketActivity(Socket *socket)
{
	(void) socket;

	debugf("SocketDelegate: activity on socket\n");
}

/* Invoked when data has been read from a socket */
void
SocketDelegate::socketReadBuffer(Socket *socket, const void *buf, size_t buflen)
{
	(void) socket;
	(void) buf;

	debugf("SocketDelegate: read %lu bytes from socket\n", (unsigned long) buflen);
}
