#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Beacon/MuxDelegate.hh"

using namespace WARP;
using namespace WARP::Core;
using namespace WARP::Beacon;

void
MuxDelegate::packetRead(Object *sender, Socket *socket, Packet *packet)
{
	(void) sender;
	(void) socket;
	(void) packet;

	debugf("MuxDelegate: read a packet from the pipe\n");
}

void
MuxDelegate::packetWritten(Object *sender, Packet::Encoded *encoded)
{
	(void) sender;
	(void) encoded;

	debugf("MuxDelegate: written a packet to the pipe\n");
}
