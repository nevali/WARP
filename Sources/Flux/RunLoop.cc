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
#include "WARP/Flux/RunLoopDelegate.hh"

using namespace WARP::Flux;

RunLoop::RunLoop(RunLoopDelegate *delegate):
	Object(),
	_runLoopDelegate(delegate),
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
			tracef("RunLoop::add(): adding Channel<%p>\n", eventSource);
			_channels->add(static_cast<Channel *>(eventSource));
			break;
		default:
			tracef("RunLoop::add(): adding generic EventSource<%p>\n", eventSource);
			_sources->add(eventSource);
			break;
	}
}

void
RunLoop::processEventsWithTimeout(struct timeval *tv)
{
	if(tv)
	{
		tracef("RunLoop::processEventsWithTimeout({ %d, %d })\n", (int) tv->tv_sec, (int) tv->tv_usec);
	}
	else
	{
		tracef("RunLoop::processEventsWithTimeout(NULL)\n");
	}
	_channels->processEventsWithTimeout(tv);
// 	foreach(_sources) ...	_sources->processPendingEvents();
}

void
RunLoop::runToCompletion(void)
{
	tracef("RunLoop::runToCompletion(): run-loop starting\n");
	while(!terminated())
	{
		struct timeval tv;

		tv.tv_sec = INTERVAL_SEC;
		tv.tv_usec = INTERVAL_USEC;
		processEventsWithTimeout(&tv);
	}
	tracef("RunLoop::runToCompletion(): run-loop has been terminated\n");
}

void
RunLoop::terminate(void)
{
	_terminated = true;
}

bool
RunLoop::terminated(void)
{
	if(_terminated)
	{
		return true;
	}
	if(_runLoopDelegate)
	{
		return _runLoopDelegate->shouldRunLoopTerminate(this, this);
	}
	return false;
}
