#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/RunLoopDelegate.hh"

using namespace WARP::Flux;

void
RunLoopDelegate::runLoopIsStarting(Object *sender, RunLoop *loop)
{
	(void) sender;
	(void) loop;
}

void
RunLoopDelegate::runLoopEnded(Object *sender, RunLoop *loop)
{
	(void) sender;
	(void) loop;
}

void
RunLoopDelegate::runLoopPass(Object *sender, RunLoop *loop)
{
	(void) sender;
	(void) loop;
}

bool
RunLoopDelegate::shouldRunLoopTerminate(Object *sender, RunLoop *loop)
{
	(void) sender;
	(void) loop;

	return false;
}
