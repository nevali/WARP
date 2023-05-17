#ifndef WARP_FLUX_RUNLOOP_HH_
# define WARP_FLUX_RUNLOOP_HH_

# include <cstddef>

# include <sys/select.h>

# include "Object.hh"
# include "Array.hh"
# include "ChannelSet.hh"

namespace WARP
{
	namespace Flux
	{
		class EventSource;
		class SubTask;
		class Channel;
		struct RunLoopDelegate;

		/* A RunLoop repeatedly waits for one or more types of events to occur
		 * and then triggers processing via the respective object's delegates
		 */
		class RunLoop: public Object
		{
			public:
				static const int INTERVAL_SEC = 0;
				static const int INTERVAL_USEC = 100000;
			public:
				RunLoop(RunLoopDelegate *delegate = NULL);
			protected:
				virtual ~RunLoop();
			public:
				virtual Kind kind(void) const;
			public:
				/* note that this is a polymorphic interface but different types
				 * of object process events differently, and this behaviour is
				 * somewhat baked into the implementation of WARP::Flux::RunLoop
				 */
				virtual void add(EventSource *eventSource);
				virtual void remove(EventSource *eventSource);
				virtual void processEventsWithTimeout(struct timeval *tv = NULL);
				virtual void runToCompletion(void);
				virtual void terminate(void);
				virtual bool terminated(void);
			private:
				RunLoopDelegate *_runLoopDelegate;
				bool _terminated;
				TArray<EventSource> *_sources;
				ChannelSet *_channels;
		};
	}
}

#endif /*!WARP_FLUX_RUNLOOP_HH_*/
