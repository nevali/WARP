#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cerrno>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Beacon/MuxProxy.hh"
#include "WARP/Beacon/TCPListener.hh"

using namespace WARP;
using namespace WARP::Beacon;

int
main(int argc, const char *argv[])
{
	Beacon::MuxProxy proxy;

	(void) argc;
	(void) argv;

	TCPListener listener(&proxy);
	if(listener.listen() < 0)
	{
		diagf(DIAG_CRITICAL, "%s: failed to open listening socket: %s\n", argv[0], strerror(errno));
		return 1;
	}
	proxy.add(&listener);
	return proxy.run();
}
