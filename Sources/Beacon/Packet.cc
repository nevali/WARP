#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstring>
#include <cerrno>

#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Beacon/Packet.hh"
#include "WARP/Beacon/Packets.hh"

using namespace WARP::Core;
using namespace WARP::Beacon;

uint16_t Packet::_nextSeq = 0;

Packet::Packet(Type type): _type(type)
{
}

Packet::~Packet()
{
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
//			packet = new Packets::Ident(encoded->ident.version, encoded->payload.data);
			break;
		case Packet::PKT_OPEN:
			debugf("Packet: packet type is PKT_OPEN\n");
			packet = new Packets::ConnectionOpened(encoded->open.socket);
			break;
		case Packet::PKT_CLOSE:
			debugf("Packet: packet type is PKT_CLOSE\n");
			packet = new Packets::ConnectionClosed(encoded->closed.socket);
			break;
		case Packet::PKT_PAYLOAD:
			debugf("Packet: packet type is PKT_PAYLOAD\n");
			packet = new Packets::Payload(encoded->payload.socket, encoded->payload.data, encoded->length - Packet::Encoded::PACKET_HEADER - Packet::Encoded::PACKET_PAYLOAD_HEADER);
			break;
		default:
			debugf("Packet: packet type is not handled (%d)\n", (int) encoded->type);
	}
	return packet;
}

