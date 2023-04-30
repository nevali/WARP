#ifndef WARP_BEACON_MUXDELEGATE_HH_
# define WARP_BEACON_MUXDELEGATE_HH_

# include "WARP/Flux/Channel.hh"

# include "WARP/Beacon/Packet.hh"

namespace WARP
{
	namespace Beacon
	{
		class MuxDelegate: public WARP::Flux::ChannelDelegate
		{
		public:
			/* Packet has been read and demultiplexed */
			virtual void packetRead(Flux::Object *sender, Flux::Channel *channel, Packet *packet);
			/* Packet has been multiplexed and written */
			virtual void packetWritten(Flux::Object *sender, Packet::Encoded *encoded);
		};
	}
}

#endif /*!WARP_BEACON_MUXDELEGATE_HH_*/
