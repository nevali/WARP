#ifndef WARP_FLUX_SUBTASK_HH_
# define WARP_FLUX_SUBTASK_HH_

#include <spawn.h>

#include "EventSource.hh"
#include "Pipe.hh"

namespace WARP
{
	namespace Flux
	{
		class SubTask: public EventSource
		{
			public:
				SubTask();
			protected:
				virtual ~SubTask();
			public:
				virtual Kind kind(void) const;
				virtual void processPendingEvents(void);
			protected:
				bool launch(void);
				int kill(int signo);
				void bindDescriptor(int parent, int child);
				void bindInput(Pipe *pipe);
				void bindOutput(Pipe *pipe);
			protected:
				virtual void processPipeEvents(void);
				virtual void processChildEvents(void);
				virtual void childWasSignalled(pid_t pid, int status);
			protected:
				char *_name;
				bool _usePath;
				pid_t _pid;
				posix_spawn_file_actions_t _file_actions;
				posix_spawnattr_t _attrs;
		};
	}
}

#endif /*!WARP_CORE_SUBTASK_HH_*/
