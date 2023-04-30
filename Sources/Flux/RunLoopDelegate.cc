#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/RunLoopDelegate.hh"

using namespace WARP::Flux;

bool
RunLoopDelegate::shouldRunLoopTerminate(Object *sender, RunLoop *loop)
{
	(void) sender;
	(void) loop;

	return false;
}
