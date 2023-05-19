#ifndef WARP_FLUX_CHANNELDELEGATE_HH_
# define WARP_FLUX_CHANNELDELEGATE_HH_

# include <cstddef>

# include "Object.hh"

namespace WARP
{
	namespace Flux
	{
		class Channel;
		
		struct ChannelDelegate
		{
			ChannelDelegate() { };
			virtual ~ChannelDelegate();

			virtual void becameChannelDelegateFor(Object *sender, Channel *channel);
			/* Invoked when a channel has been created */
			virtual void channelOpened(Object *sender, Channel *channel);
			/* Invoked when a channel has been closed */
			virtual void channelClosed(Object *sender, Channel *channel);
			/* Invoked when the channel is ready to be read from (data is pending) */
			virtual void channelReadPending(Object *sender, Channel *channel);
			/* Invoked when the channel is ready to be written to (data has been flushed) */
			virtual void channelWriteReady(Object *sender, Channel *channel);
			/* Invoked to determine whether the channel is ready to receive data */
			virtual bool isChannelReadyToReceive(Object *sender, Channel *channel);
		};

	}
}

#endif /*!WARP_FLUX_CHANNELDELEGATE_HH_*/
