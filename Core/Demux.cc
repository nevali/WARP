#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cerrno>

#include <fcntl.h>
#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Mux.hh"

using namespace WARP;

Demux::Demux(MuxDelegate *delegate):
	Listener(delegate),
	_delegate(delegate)
{
	int flags;

	_fd = STDIN_FILENO;

	/* set non-blocking mode on the input descriptor so that reads don't hang */	
	flags = fcntl(_fd, F_GETFL, 0);
	fcntl(_fd, F_SETFL, flags | O_NONBLOCK);
}

Demux::~Demux()
{
}

void
Demux::processSet(fd_set *set)
{
	ssize_t r;

	if(_fd < 0)
	{
		return;
	}
	tracef("Demux: processing started\n");
	if(FD_ISSET(_fd, set))
	{
		if(_delegate)
		{
			_delegate->socketActivity(this);
		}
		do
		{
			r = read(_fd, _readBuf.base(), _readBuf.size());
		}
		while(r == -1 && (errno == EINTR || errno == EAGAIN));
		if(r <= 0)
		{
			diagf(DIAG_CRITICAL, "Demux: pipe error: %s\n", strerror(errno));
			abort();
		}
		debugf("Demux: read %d bytes from source\n", (int) r);
		/* use our socketReadBuffer instead of the delegate's */
		socketReadBuffer(this, _readBuf.base(), r);
	}
}

Packet::Encoded *
Demux::demux(const void *buf, size_t buflen)
{
	Packet::Encoded *encoded; 

	if(buflen < Packet::Encoded::PACKET_HEADER)
	{
		debugf("Demux: buffer is too small to contain a packet\n");
		return NULL;
	}
	encoded = (Packet::Encoded *) buf;
	/* Validate that the packet is valid and we have all of it */
	if(encoded->length >= buflen)
	{
		debugf("Demux: buffer does not contain whole packet (expected %u bytes, have %u bytes)\n", (unsigned) encoded->length, (unsigned) buflen);
		return NULL;
	}
	if(encoded->length < Packet::Encoded::PACKET_HEADER)
	{
		debugf("Demux: packet length is too small to be valid\n");
		return NULL;
	}
	if(!supportedPacketType(encoded->type))
	{
		debugf("Demux: packet type (%d) is not supported\n", (int) encoded->type);
		return NULL;
	}
	return encoded;
}

/* Apply an allow-list to incoming packet types */
bool
Demux::supportedPacketType(Packet::Type type)
{
	switch(type)
	{
		case Packet::PKT_PAYLOAD:
			return true;
		default:
			return false;
	}
}

Listener::Client *
Demux::accept(void)
{
	return NULL;
}

void
Demux::socketReadBuffer(Socket *socket, const void *buf, size_t buflen)
{
	Packet *packet = NULL;
	Packet::Encoded *encoded;

	debugf("Demux: have buffer of %d bytes\n", (int) buflen);
	encoded = demux(buf, buflen);
	if(!encoded)
	{
		debugf("Demux: failed to demux packet\n");
		return;
	}
	packet = Packet::decode(encoded);
	if(!packet)
	{
		debugf("Demux: failed to decode packet\n");
		return;
	}
	if(_delegate)
	{
		_delegate->packetRead(socket, packet);
	}
	delete packet;
}

