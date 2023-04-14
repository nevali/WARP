#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/MuxDelegate.hh"

using namespace WARP;

void
MuxDelegate::packetRead(Socket *socket, Packet *packet)
{
	(void) socket;
	(void) packet;

	debugf("MuxDelegate: read a packet from the pipe\n");
}

void
MuxDelegate::packetWritten(Packet::Encoded *encoded)
{
	(void) encoded;

	debugf("MuxDelegate: written a packet to the pipe\n");
}
