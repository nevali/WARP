#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <sys/select.h>
#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Beacon/Packet.hh"
#include "WARP/Beacon/MuxProxy.hh"
#include "WARP/Beacon/DebugMux.hh"

using namespace WARP;
using namespace WARP::Core;
using namespace WARP::Beacon;

MuxProxy::MuxProxy():
	Server(),
	_mux(NULL),
	_debug(false)
{
	/* Create a Demux instance to demultiplex and decode packets */
	if(::isatty(0) && ::isatty(1))
	{
		diagf(DIAG_NOTICE, "standard input and standard output are terminals; activating debug mode\n");
		_debug = true;
	}
	if(_debug)
	{
		_demux = new DebugDemux(this);
	}
	else
	{
		_demux = new Demux(this);
	}
	/* add the Demux instance as a listener */
	add(_demux);

	/* Create a Mux instance to encode and multiplex packets --
	 * the Demux instance is used as the Mux's delegate, so will be
	 * triggered whenever data is read from the pipe
	 */
	if(_debug)
	{
		_mux = new DebugMux(this);
	}
	else
	{
		_mux = new Mux(this);
	}
}

MuxProxy::~MuxProxy()
{
	delete _mux;
	delete _demux;
}

void
MuxProxy::socketOpened(Socket *socket)
{
	debugf("MuxProxy: new connection #%d established\n", socket->id());
	if(!_mux->connectionOpened(socket->id()))
	{
		debugf("MuxProxy: failed to mux new connection\n");
		/* XXX */
	}
}

void
MuxProxy::socketClosed(Socket *socket)
{
	debugf("MuxProxy: connection #%d closed\n", socket->id());
	if(!_mux->connectionClosed(socket->id()))
	{
		debugf("MuxProxy: failed to mux closed connection\n");
		/* XXX */
	}
}

void
MuxProxy::socketReadBuffer(Socket *socket, const void *buf, size_t buflen)
{
	debugf("MuxProxy: read %lu bytes from socket #%d\n", (unsigned long) buflen, socket->id());
	if(!_mux->payload(socket->id(), buf, buflen))
	{
		debugf("MuxProxy: failed to mux payload\n");
		/* XXX */
	}
}

void
MuxProxy::packetRead(Socket *socket, Packet *packet)
{
	(void) socket;
	(void) packet;

	debugf("MuxProxy: read packet from pipe\n");
}