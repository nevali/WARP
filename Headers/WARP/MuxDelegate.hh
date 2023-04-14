#ifndef WARP_MUXDELEGATE_HH_
# define WARP_MUXDELEGATE_HH_

# include "Socket.hh"
# include "Packet.hh"

namespace WARP
{
	class MuxDelegate: public SocketDelegate
	{
	public:
		/* Packet has been read and demultiplexed */
		virtual void packetRead(Socket *socket, Packet *packet);
		/* Packet has been multiplexed and written */
		virtual void packetWritten(Packet::Encoded *encoded);
	};
}

#endif /*!WARP_MUXDELEGATE_HH_*/
