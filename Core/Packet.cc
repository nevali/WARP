#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstring>
#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Packet.hh"

using namespace WARP;

uint16_t Packet::_nextSeq = 0;

Packet::Packet(Type type): _type(type)
{
}

Packet::~Packet()
{
}

Packet *
Packet::decode(Packet::Encoded *encoded)
{
	Packet *packet = NULL;

	switch(encoded->type)
	{
		case Packet::PKT_INVALID:
			debugf("Packet: packet type is PKT_INVALID\n");
			break;
		case Packet::PKT_IDENT:
			debugf("Packet: packet type is PKT_IDENT\n");
			break;
		case Packet::PKT_PAYLOAD:
			debugf("Packet: packet type is PKT_PAYLOAD\n");
			packet = new PayloadPacket(encoded->payload.socket, encoded->payload.data, encoded->length - Packet::Encoded::PACKET_HEADER - Packet::Encoded::PACKET_PAYLOAD_HEADER);
			break;
		default:
			debugf("Packet: packet type is not handled (%d)\n", (int) encoded->type);
	}
	return packet;
}

Packet::Type
Packet::type(void) const
{
	return _type;
}

ssize_t
Packet::Encoded::send(int fd)
{
	return ::write(fd, this, length);
}

Packet::Encoded *
PayloadPacket::encode(void *buf, size_t bufsize)
{
	Packet::Encoded *encoded;

	if(bufsize < sizeof(Packet::Encoded) + _buflen)
	{
		return NULL;
	}
	encoded = (Packet::Encoded *) buf;
	memset(encoded, 0, sizeof(Packet::Encoded));
	encoded->type = _type;
	encoded->seq = ++_nextSeq;
	encoded->length = Packet::Encoded::PACKET_HEADER + Packet::Encoded::PACKET_PAYLOAD_HEADER + _buflen;
	encoded->payload.socket = _conn;
	memcpy(encoded->payload.data, _buf, _buflen);
	return encoded;
}

const uint8_t *
PayloadPacket::buffer(void) const
{
	return (const uint8_t *) _buf;
}

size_t
PayloadPacket::size(void) const
{
	return _buflen;
}
