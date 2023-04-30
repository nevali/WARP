#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/EventSource.hh"

using namespace WARP::Flux;

EventSource::EventSource():
	Object()
{
}

EventSource::~EventSource()
{
}

void
EventSource::processPendingEvents(void)
{
}
