#ifndef WARP_FLUX_CHANNELSET_HH_
# define WARP_FLUX_CHANNELSET_HH_

# include "Array.hh"
# include "EventSource.hh"
# include "Channel.hh"

namespace WARP
{
	namespace Flux
	{
		/* a ChannelSet is an array of Channels which is also an
		 * EventSource and implements the processEventsWithTimeout()
		 * method
		 */
		class ChannelSet: public TArray<Channel>, virtual protected EventSource
		{
			public:
				ChannelSet();
			protected:
				virtual ~ChannelSet();
			public:
				virtual unsigned retain(void);
				virtual unsigned release(void);
				virtual Kind kind(void) const;
			protected:
				friend class RunLoop;

				virtual void processEventsWithTimeout(struct timeval *tv = NULL);
				virtual void processPendingEvents(void);
		};
	}
}

#endif /*!WARP_FLUX_CHANNELSET_HH_*/