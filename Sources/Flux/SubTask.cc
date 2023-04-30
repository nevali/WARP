#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cassert>
#include <cerrno>
#include <cstring>

#include <signal.h>
#include <spawn.h>
#include <unistd.h>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/SubTask.hh"
#include "WARP/Flux/Pipe.hh"

using namespace WARP::Flux;

SubTask::SubTask():
	_name(NULL),
	_usePath(false),
	_pid(0)
{
	posix_spawn_file_actions_init(&_file_actions);
	posix_spawnattr_init(&_attrs);
}

SubTask::~SubTask()
{
	kill(-9);
	posix_spawn_file_actions_destroy(&_file_actions);
	posix_spawnattr_destroy(&_attrs);
}

void
SubTask::processPendingEvents(void)
{
	tracef("SubTask::processEvents(): _pid = %d\n", (int) _pid);
	if(_pid == 0)
	{
		if(!launch())
		{
			return;
		}
	}
	if(_pid < 1)
	{
		return;
	}
	processPipeEvents();
	processChildEvents();
	return;
}

void
SubTask::processPipeEvents(void)
{
}

void
SubTask::processChildEvents(void)
{
	pid_t r;
	int status;
	
	if(_pid < 1)
	{
		return;
	}
	do
	{
		r = ::waitpid(_pid, &status, WNOHANG);
		debugf("SubTask: waitpid: r = %d, status = %d, errno = %d (%s)\n", (int) r, status, errno, strerror(errno));
	}
	while(r == -1 && errno == EINTR);
	if(r == -1 && errno == ECHILD)
	{
		debugf("SubTask: child %d no longer exists\n", (int) _pid);
		_pid = -1;
		return;
	}
	if(r < 0)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
	if(r > 0)
	{
		childWasSignalled(r, status);
	}
}

int
SubTask::kill(int signo)
{
	return ::kill(_pid, signo);
}

void
SubTask::bindDescriptor(int parent, int child)
{
	assert(_pid == 0);
	posix_spawn_file_actions_adddup2(&_file_actions, parent, child);
}

void
SubTask::bindInput(Pipe *pipe)
{
	/* bind the emitter end of the pipe to stdin */
	tracef("SubTask::bindInput: binding %d to #%d\n", pipe->emitter(), STDIN_FILENO);
	bindDescriptor(pipe->emitter(), STDIN_FILENO);
}

void
SubTask::bindOutput(Pipe *pipe)
{
	/* bind the receiver end of the pipe to stdin */
	tracef("SubTask::bindOutput: binding %d to #%d\n", pipe->receiver(), STDOUT_FILENO);
	bindDescriptor(pipe->receiver(), STDOUT_FILENO);
}

bool
SubTask::launch(void)
{
	char namebuf[32] = "Beacon/warp-beacon";
	char * const argv[] = { namebuf, NULL };
	const char *name = (_name ? _name : argv[0]);

	assert(_pid == 0);
	tracef("SubTask::launch: _name = '%s', argv[0] = '%s'\n", _name, argv[0]);
	if(_usePath)
	{
		if(posix_spawnp(&_pid, name, &_file_actions, &_attrs, argv, NULL))
		{
//			setStatus(ERROR);
			diagf(DIAG_ERROR, "%s: %s\n", name, strerror(errno));
			return false;
		}
	}
	else
	{
		if(posix_spawn(&_pid, name, &_file_actions, &_attrs, argv, NULL))
		{
//			setStatus(ERROR);
			diagf(DIAG_ERROR, "%s: %s\n", name, strerror(errno));
			return false;
		}
	}
	debugf("SubTask::launch: pid = %d\n", (int) _pid);
//	setStatus(RUNNING);
	return true;
}

void
SubTask::childWasSignalled(pid_t pid, int status)
{
	if(pid != _pid)
	{
		return;
	}
	_pid = 0;
	if(WIFEXITED(status))
	{
		debugf("%s: SubTask '%s' (%d) exited with status %d\n", __FUNCTION__, _name, (int) pid, WEXITSTATUS(status));
	}
	else if(WIFSIGNALED(status))
	{
		debugf("%s: SubTask '%s' (%d) terminated with signal %d\n", __FUNCTION__, _name, (int) pid, WTERMSIG(status));
	}
	else
	{
		/* stopped? */
	}
//	fprintf(stderr, "%s: PosixSubTask '%s' is now in state %s\n", __FUNCTION__, _name, statusString());
}
