#ifndef BEACON_DEBUGMUX_HH_
# define BEACON_DEBUGMUX_HH_

# include "WARP/Mux.hh"
# include "WARP/Demux.hh"

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

#endif /*!BEACON_DEBUGMUX_HH_*/
