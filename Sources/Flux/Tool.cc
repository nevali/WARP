#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstddef>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/Task.hh"
#include "WARP/Flux/RunLoop.hh"

using namespace WARP::Flux;
using namespace WARP::Flux::Diagnostics;

Tool::Tool(int argc, char **argv):
	Task(),
	_argc(argc),
	_argv(argv),
	_mainLoop(NULL)
{
	if(argc < 0)
	{
		_argc = 0;	
	}
	_mainLoop = new RunLoop(this); 
}

Tool::~Tool()
{
	if(_mainLoop)
	{
		_mainLoop->release();
	}
}

size_t
Tool::argc(void) const
{
	return _argc;
}

const char *
Tool::argv(size_t index) const
{
	if(index > _argc)
	{
		return NULL;
	}
	return _argv[index];
}

RunLoop *
Tool::mainLoop(void) const
{
	if(!_mainLoop)
	{
		return NULL;
	}
	_mainLoop->retain();
	return _mainLoop;
}

void
Tool::add(EventSource *source)
{
	if(_mainLoop)
	{
		_mainLoop->add(source);
	}
}

void
Tool::remove(EventSource *source)
{
	if(_mainLoop)
	{
		_mainLoop->remove(source);
	}
}

int
Tool::run(void)
{
	int r;

	if((r = initialise()) != 0)
	{
		return r;
	}
	_mainLoop->runToCompletion();
	teardown();
	return r;
}

int
Tool::initialise(void)
{
	if(!processArgs())
	{
		return 1;
	}
	return 0;
}

void
Tool::teardown(void)
{
}

bool
Tool::processArgs(void)
{
	return true;
}

bool
Tool::shouldRunLoopTerminate(Object *sender, RunLoop *loop)
{
	(void) sender;
	(void) loop;
	
	return true;
}