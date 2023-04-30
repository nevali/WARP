#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <sys/select.h>
#include <unistd.h>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/RunLoop.hh"
#include "WARP/Flux/EventSource.hh"
#include "WARP/Flux/Channel.hh"
#include "WARP/Flux/SubTask.hh"

using namespace WARP::Flux;

RunLoop::RunLoop():
	Object(),
	_terminated(false)
{
	_sources = new TArray<EventSource>();
	_channels = new ChannelSet();
}

RunLoop::~RunLoop()
{
	_channels->release();
	_sources->release();
}

Object::Kind
RunLoop::kind(void) const
{
	return RUNLOOP;
}

void
RunLoop::add(EventSource *eventSource)
{
	/* EventSources are sorted into different buckets for each of
	 * the supported specialised interfaces
	 */
	switch(eventSource->kind())
	{
		case CHANNEL:
			_channels->add(static_cast<Channel *>(eventSource));
			break;
		case CHANNELSET:
		default:
			_sources->add(eventSource);
			break;
	}
}

void
RunLoop::processEventsWithTimeout(struct timeval *tv)
{
/* 	fd_set fds;

		FD_ZERO(&fds);
		_listeners.addSet(&fds);
		::select(FD_SETSIZE, &fds, NULL, NULL, tv);
		_listeners.processSet(&fds); */
	_channels->processEventsWithTimeout(tv);
// 	foreach(_sources) ...	_sources->processPendingEvents();
}

void
RunLoop::runToCompletion(void)
{
	while(!_terminated)
	{
		struct timeval tv;

		tv.tv_sec = 0;
		tv.tv_usec = 250;
		processEventsWithTimeout(&tv);
	}
}

void
RunLoop::terminate(void)
{
	_terminated = true;
}
