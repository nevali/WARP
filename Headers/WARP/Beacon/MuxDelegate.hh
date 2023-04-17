#ifndef WARP_BEACON_MUXDELEGATE_HH_
# define WARP_BEACON_MUXDELEGATE_HH_

# include "WARP/Core/Socket.hh"

# include "WARP/Beacon/Packet.hh"

namespace WARP
{
	namespace Beacon
	{
		class MuxDelegate: public WARP::Core::SocketDelegate
		{
		public:
			/* Packet has been read and demultiplexed */
			virtual void packetRead(Core::Object *sender, Core::Socket *socket, Packet *packet);
			/* Packet has been multiplexed and written */
			virtual void packetWritten(Core::Object *sender, Packet::Encoded *encoded);
		};
	}
}

#endif /*!WARP_BEACON_MUXDELEGATE_HH_*/
