#ifndef WARP_CORE_SOCKETBUFFER_HH_
# define WARP_CORE_SOCKETBUFFER_HH_

#include "Buffer.hh"
#include "ChannelDelegate.hh"

namespace WARP
{
	namespace Flux
	{
		/* a Buffer that can be used as a delegate for a Channel, receiving
		 * any incoming data
		 *
		 * the Buffer's own delegate can then be used to process the buffered
		 * data
		 */
		class ChannelBuffer: public Buffer, virtual public ChannelDelegate
		{
			public:
				ChannelBuffer(BufferDelegate *delegate = NULL,  size_t nbytes = Buffer::DEFSIZE, bool autoDrain = true);
				virtual ~ChannelBuffer();
			public:
				/* SocketDelegate */
				virtual void channelActivity(Object *sender, Channel *socket);
			protected:
				bool _autoDrain;
		};
	}
}

#endif /*!WARP_CORE_SOCKETBUFFER_HH_*/