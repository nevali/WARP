#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cerrno>

#include "WARP/Core/Diagnostics.hh"
#include "WARP/Beacon/DebugMux.hh"

using namespace WARP;
using namespace WARP::Core;
using namespace WARP::Beacon;

DebugMux::DebugMux(MuxDelegate *delegate):
	Mux(delegate)
{
}

bool
DebugMux::mux(Packet::Encoded *encoded)
{
	debugf("DebugMux::mux(): have %u bytes of encoded packet!\n", (unsigned int) encoded->length);
	debugf("---------------------------------------------------------------------\n");
	debugf("|                         Encoded packet                            |\n");
	debugf("---------------------------------------------------------------------\n");
	debugf("Packet sequence:    %04x\n", encoded->seq);
	debugf("Length:             %u bytes\n", (int) encoded->length);
	debugf("Type:               %02x\n", encoded->type);
	debugf("---------------------------------------------------------------------\n");
	dump((uint8_t *)((void *) encoded), encoded->length);
	debugf("---------------------------------------------------------------------\n");
//	encoded->send(1);
	return true;
}


DebugDemux::DebugDemux(MuxDelegate *delegate):
	Demux(delegate)
{
}

Packet::Encoded *
DebugDemux::demux(const void *buf, size_t buflen)
{
	debugf("DebugDemux::demux(%p, %u)\n", buf, (unsigned) buflen);
	debugf("---------------------------------------------------------------------\n");
	debugf("|                         Received packet                           |\n");
	debugf("---------------------------------------------------------------------\n");
	dump((uint8_t *) buf, buflen);
	debugf("---------------------------------------------------------------------\n");
	return NULL;
}
