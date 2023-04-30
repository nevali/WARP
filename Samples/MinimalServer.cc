#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cassert>
#include <signal.h>
#include <spawn.h>
#include <unistd.h>

#include "WARP/Flux/Flux.h"

#include "WARP/Beacon/Packet.hh"
#include "WARP/Beacon/Packets.hh"
#include "WARP/Beacon/Demux.hh"

namespace WARP
{
	namespace Beacon
	{
		class Conduit: public Core::SubTask, public MuxDelegate
		{
			public:
				Conduit();
				virtual ~Conduit();
			protected:
				virtual void processPipeEvents(void);
				virtual void processIdent(Packets::Ident *packet);
				virtual void processConnectionOpened(Packets::ConnectionOpened *packet);
				virtual void processConnectionClosed(Packets::ConnectionClosed *packet);
				virtual void processPayload(Packets::Payload *packet);
			protected:
				/* MuxDelegate */
				virtual void packetRead(Core::Object *sender, Core::Socket *socket, Packet *packet);
			protected:
				Core::Pipe *_inputPipe;
				Core::Pipe *_outputPipe;
				Demux *_demux;
		};
	}
}

using namespace WARP::Core;
using namespace WARP::Beacon;

Conduit::Conduit():
	SubTask(),
	_demux(NULL)
{
	_inputPipe = new Pipe();
	bindInput(_inputPipe);
	_outputPipe = new Pipe();
	bindOutput(_outputPipe);
	_demux = new Demux(this, _outputPipe->emitter());
}

Conduit::~Conduit()
{
	delete _demux;
	_outputPipe->release();
	_inputPipe->release();
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
Conduit::packetRead(Object *sender, Socket *socket, Packet *packet)
{
	(void) sender;
	(void) socket;
	(void) packet;

	tracef("Conduit::packetRead()\n");
	switch(packet->type())
	{
		case Packet::PKT_IDENT:
			processIdent(static_cast<Packets::Ident *>(packet));
			return;
		case Packet::PKT_OPEN:
			processConnectionOpened(static_cast<Packets::ConnectionOpened *>(packet));
			return;
		case Packet::PKT_CLOSE:
			processConnectionClosed(static_cast<Packets::ConnectionClosed *>(packet));
			return;
		case Packet::PKT_PAYLOAD:
			processPayload(static_cast<Packets::Payload *>(packet));
			return;
		default:
			diagf(DIAG_WARNING, "Conduit: unsupported packet type %d\n", (int) packet->type());
	}
}

void
Conduit::processIdent(Packets::Ident *packet)
{
	(void) packet;
	
	tracef("Conduit::processIdent()\n");
}

void
Conduit::processConnectionOpened(Packets::ConnectionOpened *packet)
{
	tracef("Conduit::processConnectionOpened(#%d)\n", packet->id());
}

void
Conduit::processConnectionClosed(Packets::ConnectionClosed *packet)
{
	tracef("Conduit::processConnectionClosed(#%d)\n", packet->id());
}

void
Conduit::processPayload(Packets::Payload *packet)
{
	tracef("Conduit::processPayload(#%d, [%lu bytes])\n", packet->connection(), packet->size());
	debugf("---------------------------------------------------------------------\n");
	dump(packet->buffer(), packet->size());
	debugf("---------------------------------------------------------------------\n");
}

int
main(int argc, char **argv)
{
	WARP::Beacon::Conduit c;

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
