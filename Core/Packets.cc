#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstring>
#include <cerrno>

#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Packets.hh"

using namespace WARP;

Packet::Encoded *
Packets::Ident::encode(void *buf, size_t bufsize)
{
	Packet::Encoded *encoded;
	size_t length;

	length = sizeof(Packet::Encoded) + strlen(_banner);

	if(bufsize < length)
	{
		errno = ENOMEM;
		return NULL;
	}
	encoded = (Packet::Encoded *) buf;
	memset(encoded, 0, length);
	encoded->type = _type;
	encoded->seq = ++_nextSeq;
	encoded->length = length;
	encoded->ident.magic = Packet::Encoded::MAGIC_BEACON;
	encoded->ident.version = _version;
	memcpy(encoded->ident.banner, _banner, strlen(_banner));
	return encoded;
}

Packet::Encoded *
Packets::ConnectionOpened::encode(void *buf, size_t bufsize)
{
	Packet::Encoded *encoded;

	if(bufsize < sizeof(Packet::Encoded))
	{
		errno = ENOMEM;
		return NULL;
	}
	encoded = (Packet::Encoded *) buf;
	memset(encoded, 0, sizeof(Packet::Encoded));
	encoded->type = _type;
	encoded->seq = ++_nextSeq;
	encoded->length = Packet::Encoded::PACKET_HEADER + sizeof(encoded->open);
	encoded->open.socket = _id;
	return encoded;
}

Packet::Encoded *
Packets::ConnectionClosed::encode(void *buf, size_t bufsize)
{
	Packet::Encoded *encoded;

	if(bufsize < sizeof(Packet::Encoded))
	{
		errno = ENOMEM;
		return NULL;
	}
	encoded = (Packet::Encoded *) buf;
	memset(encoded, 0, sizeof(Packet::Encoded));
	encoded->type = _type;
	encoded->seq = ++_nextSeq;
	encoded->length = Packet::Encoded::PACKET_HEADER + sizeof(encoded->closed);
	encoded->closed.socket = _id;
	return encoded;
}

Packet::Encoded *
Packets::Payload::encode(void *buf, size_t bufsize)
{
	Packet::Encoded *encoded;

	if(bufsize < sizeof(Packet::Encoded) + _buflen)
	{
		errno = ENOMEM;
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
