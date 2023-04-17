#ifndef WARP_CORE_SERVER_HH_
# define WARP_CORE_SERVER_HH_

# include "Listener.hh"

namespace WARP
{
	namespace Core
	{
		/* A server hosts one or more listeners and provides an event loop */
		class Server
		{
			public:
				Server();
				virtual ~Server();
			public:
				virtual int run(void);
				virtual void add(Listener *listener);
			protected:
				SocketSet _listeners;
		};
	}
}

#endif /*!WARP_CORE_SERVER_HH_*/
