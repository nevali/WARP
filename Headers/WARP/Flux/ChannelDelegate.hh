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
			/* Invoked when a channel has been created */
			virtual void channelOpened(Object *sender, Channel *channel);
			/* Invoked when a channel has been closed */
			virtual void channelClosed(Object *sender, Channel *channel);
			/* Invoked when there is some kind of activity on a channel */
			virtual void channelActivity(Object *sender, Channel *channel);
			/* Invoked when data has been read from a channel; on return, *buflen
			* should be set to the number of bytes from the buffer that were
			* consumed
			*/
			virtual void channelReadBuffer(Object *sender, Channel *channel, const void *buf, size_t *buflen);
		};
	}
}

#endif /*!WARP_FLUX_CHANNELDELEGATE_HH_*/
