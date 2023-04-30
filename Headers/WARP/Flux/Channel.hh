#ifndef WARP_FLUX_CHANNEL_HH_
# define WARP_FLUX_CHANNEL_HH_

# include <cstddef>

# include "EventSource.hh"

namespace WARP
{
	namespace Flux
	{
		struct ChannelDelegate;

		/* A Channel represents a open file descriptor (file, socket, one end
		 * of a pipe, etc.)
		 */
		class Channel: public EventSource
		{
		public:
			virtual Kind kind(void) const;
			virtual int descriptor(void) const;
		protected:
			Channel(ChannelDelegate *delegate = NULL, int fd = -1);
			virtual ~Channel();
		public:
			ChannelDelegate *channelDelegate(void) const;
		public:
			virtual bool close(void);
		private:
			ChannelDelegate *_delegate;
			int _fd;
		};
	}
}

#endif /*!WARP_FLUX_CHANNEL_HH_*/