#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cerrno>

#include <unistd.h>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/Socket.hh"

using namespace WARP::Flux;
using namespace WARP::Flux::Diagnostics;

Socket::Socket(ChannelDelegate *delegate, int fd):
	Channel(delegate, fd)
{
}

Socket::~Socket()
{
}
