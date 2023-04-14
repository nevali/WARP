#ifndef WARP_MUX_HH_
# define WARP_MUX_HH_

#include "WARP/Listener.hh"
#include "WARP/Packet.hh"
#include "WARP/MuxDelegate.hh"

namespace WARP
{
	class Mux: public Listener
	{
		public:
			Mux(MuxDelegate *delegate);
			virtual ~Mux();
		public:
			virtual void addSet(fd_set *fds);
			virtual bool mux(Packet *packet);
			virtual bool mux(Packet::Encoded *encoded);
			virtual bool payload(int connection, const void *buf, size_t buflen);
		protected:
			virtual Client *accept(void);
		protected:
			MuxDelegate *_delegate;
			Buffer _writeBuf;
	};

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

#endif /*!WARP_MUX_HH_*/
