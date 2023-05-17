#ifndef WARP_TASK_HH_
# define WARP_TASK_HH_

# include "EventSource.hh"
# include "RunLoopDelegate.hh"

namespace WARP
{
	namespace Flux
	{
		class Task: public EventSource, public RunLoopDelegate
		{
			protected:
				Task();
				virtual ~Task();
			public:
				virtual int run(void) = 0;
		};

		class Tool: public Task
		{
			protected:
				Tool(int argc, char **argv);
				virtual ~Tool();
				virtual int initialise(void);
				virtual void teardown(void);
				virtual bool processArgs(void);
			public:
				virtual int run(void);
				virtual size_t argc(void) const;
				virtual const char *argv(size_t index) const;
				virtual RunLoop *mainLoop(void) const __attribute__ (( warn_unused_result ));
				virtual void add(EventSource *source);
				virtual void remove(EventSource *source);
			public:
				/* RunLoopDelagate */
				virtual bool shouldRunLoopTerminate(Object *sender, RunLoop *loop);
			private:
				size_t _argc;
				char **_argv;
				RunLoop *_mainLoop;
		};

	}
}

#endif /*WARP_TASK_HH_*/
