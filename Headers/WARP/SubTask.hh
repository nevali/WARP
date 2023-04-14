#ifndef WARP_SUBTASK_HH_
# define WARP_SUBTASK_HH_

#include "Pipe.hh"
#include <spawn.h>

namespace WARP
{
	class SubTask
	{
		public:
			SubTask();
			virtual ~SubTask();
		public:
			virtual bool processEvents(void);
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

#endif /*!WARP_SUBTASK_HH_*/
