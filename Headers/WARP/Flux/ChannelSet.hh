#ifndef WARP_FLUX_CHANNELSET_HH_
# define WARP_FLUX_CHANNELSET_HH_

# include <sys/types.h>
# include <sys/select.h>

# include "Array.hh"
# include "EventSource.hh"
# include "Channel.hh"

namespace WARP
{
	namespace Flux
	{
		/* a ChannelSet is an array of Channels which is also an
		 * EventSource and implements the processEventsWithTimeout()
		 * method to integrate with a RunLoop
		 */
		class ChannelSet: public TArray<Channel>, virtual protected EventSource
		{
			private:
				static bool addChannelCallback(void *ptr, void *ctx);
				static bool addChildrenCallback(void *ptr, void *ctx);
				static bool processChannelCallback(void *ptr, void *ctx);
				static bool processChildrenCallback(void *ptr, void *ctx);
			public:
				ChannelSet();
			protected:
				virtual ~ChannelSet();
			public:
				virtual unsigned retain(void);
				virtual unsigned release(void);
				virtual Kind kind(void) const;

				virtual size_t add(Channel *channel);
			protected:
				friend class RunLoop;

				virtual void processEventsWithTimeout(struct timeval *tv = NULL);
				virtual void processPendingEvents(void);
			private:
				fd_set _readers, _writers, _errors;
				int _maxFd;
				TArray<ChannelSet> *_children;
		};
	}
}

#endif /*!WARP_FLUX_CHANNELSET_HH_*/