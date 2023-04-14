#ifndef WARP_SERVER_HH_
# define WARP_SERVER_HH_

# include "Listener.hh"

namespace WARP
{
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

#endif /*!WARP_SERVER_HH_*/
