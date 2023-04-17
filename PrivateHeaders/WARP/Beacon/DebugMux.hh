#ifndef WARP_BEACON_DEBUGMUX_HH_
# define WARP_BEACON_DEBUGMUX_HH_

# include "WARP/Beacon/Mux.hh"
# include "WARP/Beacon/Demux.hh"

namespace WARP
{
	namespace Beacon
	{
		class DebugMux: public Mux
		{
			public:
				DebugMux(MuxDelegate *delegate);

				virtual bool mux(Packet::Encoded *encoded);
		};

		class DebugDemux: public Demux
		{
			public:
				DebugDemux(MuxDelegate *delegate);

				virtual Packet::Encoded *demux(const void *buf, size_t buflen);
		};
	}
}

#endif /*!WARP_BEACON_DEBUGMUX_HH_*/
