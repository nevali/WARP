#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/Task.hh"

using namespace WARP::Flux;
using namespace WARP::Flux::Diagnostics;

Task::Task():
	EventSource()
{
}

Task::~Task()
{
}
