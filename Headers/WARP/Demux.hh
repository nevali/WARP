#ifndef WARP_DEMUX_HH_
# define WARP_DEMUX_HH_

#include "Listener.hh"
#include "Packet.hh"
#include "MuxDelegate.hh"

namespace WARP
{
	class Demux: public Listener
	{
		public:
			Demux(MuxDelegate *delegate);
			virtual ~Demux();
		public:
			virtual void processSet(fd_set *fds);
			virtual Packet::Encoded *demux(const void *buf, size_t buflen);
			virtual void socketReadBuffer(Socket *socket,const void *buf, size_t buflen);
		protected:
			virtual Client *accept(void);
			virtual bool supportedPacketType(Packet::Type type);
		protected:
			MuxDelegate *_delegate;
	};
}

#endif /*!WARP_DEMUX_HH_*/