#ifndef WARP_BEACON_MUXPROXY_HH_
# define WARP_BEACON_MUXPROXY_HH_

# include "WARP/Core/Listener.hh"
# include "WARP/Core/Server.hh"

# include "WARP/Beacon/Mux.hh"
# include "WARP/Beacon/Demux.hh"

namespace WARP
{
	namespace Beacon
	{
		/* Beacon::MuxProxy is a special kind of server that multiplexes any data
		* it receives from its listeners */
		class MuxProxy: public Core::Server, public MuxDelegate
		{
			public:
				MuxProxy();
				virtual ~MuxProxy();
			public:
			public:
				/* SocketDelegate */
				virtual void socketOpened(Core::Socket *socket);
				virtual void socketClosed(Core::Socket *socket);
				virtual void socketReadBuffer(Core::Socket *socket, const void *buf, size_t buflen);
				/* MuxDelegate */
				virtual void packetRead(Core::Socket *socket, Packet *packet);
				Mux *_mux;
				Demux *_demux;
				bool _debug;
		};
	}
}

#endif /*!WARP_BEACON_MUXPROXY_HH_*/
