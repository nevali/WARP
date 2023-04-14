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

namespace WARP
{
	class Conduit: public SubTask
	{
		public:
			Conduit();
			virtual ~Conduit();
		protected:
			virtual void processPipeEvents(void);
		protected:
			Pipe _inputPipe;
			Pipe _outputPipe;
	};
}

using namespace WARP;

Conduit::Conduit():
	SubTask()
{
	bindInput(&_inputPipe);
	bindOutput(&_outputPipe);
//	setNonBlocking(_inputPipe.receiver());
//	setNonBlocking(_outputPipe.emitter());
}

Conduit::~Conduit()
{
}

void
Conduit::processPipeEvents(void)
{
	fd_set set;

	tracef("Conduit::processPipeEvents()\n");
	FD_ZERO(&set);
	FD_SET(_outputPipe.emitter(), &set);
	select(_outputPipe.emitter() + 1, &set, NULL, NULL, NULL);
	debugf("Conduit: select() returned\n");
	if(FD_ISSET(_outputPipe.emitter(), &set))
	{
		debugf("Conduit: activity on pipe receiver!\n");
		
	}
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
