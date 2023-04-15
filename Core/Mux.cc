#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cerrno>

#include <fcntl.h>
#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Mux.hh"
#include "WARP/Packets.hh"

using namespace WARP;

Mux::Mux(MuxDelegate *delegate):
	Listener(delegate),
	_muxDelegate(delegate),
	_writeBuf()
{
	_fd = STDOUT_FILENO;
}

Mux::~Mux()
{
}

void
Mux::addSet(fd_set *set)
{
	(void) set;
}

Listener::Client *
Mux::accept(void)
{
	return NULL;
}

/* Encode and multiplex a packet */
bool
Mux::mux(Packet *packet)
{
	Packet::Encoded *encoded = packet->encode(_writeBuf.base(), _writeBuf.size());

	if(!encoded)
	{
		/* XXX throw? */
		debugf("Mux: failed to encode packet\n");
		return false;
	}
	return mux(encoded);
}

/* Multiplex a packet */
bool
Mux::mux(Packet::Encoded *encoded)
{
	size_t size;
	const uint8_t *ptr;
	ssize_t r;

	debugf("Mux: writing encoded packet (%u bytes)\n", (unsigned int) encoded->length);
	ptr = (uint8_t *) ((void *) encoded);
	size = encoded->length;
	while(size)
	{
		do
		{
			r = ::write(_fd, ptr, size);
		}
		while(r == -1 && (errno == EINTR || errno == EAGAIN));
		if(r < 0)
		{
			debugf("Mux: write() failed: %s\n", strerror(errno));
			return false;
		}
		else if((size_t) r > size)
		{
			r = size;
		}
		size -= r;
		ptr += r;
	}
	if(_muxDelegate)
	{
		_muxDelegate->packetWritten(encoded);
	}
	return true;
}

/* Create and multiplex an Ident packet */
bool
Mux::ident(unsigned version, const char *name)
{
	bool r;
	Packet *packet = new Packets::Ident(version, name);
	r = mux(packet);
	delete packet;
	return r;
}

/* Create and multiplex a Connection Opened packet */
bool
Mux::connectionOpened(int id)
{
	bool r;
	Packet *packet = new Packets::ConnectionOpened(id);
	r = mux(packet);
	delete packet;
	return r;
}

/* Create and multiplex a Connection Opened packet */
bool
Mux::connectionClosed(int id)
{
	bool r;
	Packet *packet = new Packets::ConnectionClosed(id);
	r = mux(packet);
	delete packet;
	return r;
}

/* Create and multiplex a Payload packet */
bool
Mux::payload(int connection, const void *buf, size_t buflen)
{
	bool r;
	Packet *packet = new Packets::Payload(connection, buf, buflen);
	r = mux(packet);
	delete packet;
	return r;
}
