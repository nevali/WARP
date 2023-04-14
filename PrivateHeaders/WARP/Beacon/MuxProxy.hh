#ifndef BEACON_MUXPROXY_HH_
# define BEACON_MUXPROXY_HH_

# include "WARP/Listener.hh"
# include "WARP/Mux.hh"
# include "WARP/Server.hh"

namespace WARP
{
	namespace Beacon
	{
		/* Beacon::MuxProxy is a special kind of server that multiplexes any data
		* it receives from its listeners */
		class MuxProxy: public Server, public MuxDelegate
		{
			public:
				MuxProxy();
				virtual ~MuxProxy();
			public:
			public:
				/* SocketDelegate */
				virtual void socketReadBuffer(Socket *socket, const void *buf, size_t buflen);
				/* MuxDelegate */
				virtual void packetRead(Socket *socket, Packet *packet);
				Mux *_mux;
				Demux *_demux;
				bool _debug;
		};
	}
}

#endif /*!BEACON_MUXPROXY_HH_*/
