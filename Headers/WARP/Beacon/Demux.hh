#ifndef WARP_BEACON_DEMUX_HH_
# define WARP_BEACON_DEMUX_HH_

#include "WARP/Flux/Listener.hh"

#include "Packet.hh"
#include "MuxDelegate.hh"

namespace WARP
{
	namespace Beacon
	{
		class Demux: public Core::Listener
		{
			public:
				Demux(MuxDelegate *delegate, int fd = 0);
				virtual ~Demux();
			public:
				virtual void processSet(fd_set *fds);
				virtual Packet::Encoded *demux(const void *buf, size_t buflen);
				virtual void socketReadBuffer(Core::Socket *socket, const void *buf, size_t buflen);
			protected:
				virtual Client *accept(void);
				virtual bool supportedPacketType(Packet::Type type);
			protected:
				MuxDelegate *_muxDelegate;
		};
	}
}

#endif /*!WARP_BEACON_DEMUX_HH_*/
