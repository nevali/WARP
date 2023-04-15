#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cassert>
#include <signal.h>
#include <spawn.h>
#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/SubTask.hh"
#include "WARP/Packet.hh"
#include "WARP/Packets.hh"
#include "WARP/Demux.hh"

namespace WARP
{
	class Conduit: public SubTask, public MuxDelegate
	{
		public:
			Conduit();
			virtual ~Conduit();
		protected:
			virtual void processPipeEvents(void);
			virtual void processPayload(Packets::Payload *payload);
		protected:
			/* MuxDelegate */
			virtual void packetRead(Socket *socket, Packet *packet);
		protected:
			Pipe _inputPipe;
			Pipe _outputPipe;
			Demux *_demux;
	};
}

using namespace WARP;

Conduit::Conduit():
	SubTask(),
	_demux(NULL)
{
	bindInput(&_inputPipe);
	bindOutput(&_outputPipe);
	_demux = new Demux(this, _outputPipe.emitter());
}

Conduit::~Conduit()
{
	delete _demux;
}

void
Conduit::processPipeEvents(void)
{
	fd_set set;

	tracef("Conduit::processPipeEvents()\n");
	FD_ZERO(&set);
	_demux->addSet(&set);
	select(FD_SETSIZE, &set, NULL, NULL, NULL);
	debugf("Conduit: select() returned\n");
	_demux->processSet(&set);
}

void
Conduit::packetRead(Socket *socket, Packet *packet)
{
	(void) socket;
	(void) packet;

	tracef("Conduit::packetRead()\n");
	switch(packet->type())
	{
		case Packet::PKT_PAYLOAD:
			processPayload(static_cast<Packets::Payload *>(packet));
			break;
		default:
			diagf(DIAG_WARNING, "Conduit: unsupported packet type %d\n", (int) packet->type());
	}
}

void
Conduit::processPayload(Packets::Payload *payload)
{
	tracef("Conduit::processPayload(#%d, [%lu bytes])\n", payload->connection(), payload->size());
	debugf("---------------------------------------------------------------------\n");
	dump(payload->buffer(), payload->size());
	debugf("---------------------------------------------------------------------\n");
}

int
main(int argc, char **argv)
{
	Conduit c;

	(void) argc;
	(void) argv;

	for(;;)
	{
		if(!c.processEvents())
		{
			break;
		}
	}
	return 0;
}
