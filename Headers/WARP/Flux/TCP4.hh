#ifndef WARP_BEACON_TCP4_HH_
# define WARP_BEACON_TCP4_HH_

# include <netinet/in.h>
# include <arpa/inet.h>

# include "Socket.hh"
# include "Listener.hh"

namespace WARP
{
	namespace Flux
	{
		class TCPListener: public Listener
		{
		protected:
			class TCPClient: public Client
			{
				public:
					TCPClient(Listener *listener, int fd, struct sockaddr_in *sin, socklen_t sinlen);
					virtual ~TCPClient(void);
				protected:
					struct sockaddr_in _sin;
			};
		public:
			TCPListener(SocketDelegate *delegate, uint32_t addr = INADDR_ANY, uint16_t port = 0);
			virtual ~TCPListener();
			virtual int listen(int backlog = 0);
		protected:
			int _err;

			virtual Client *accept(void);
		};
	}
}

#endif /*!WARP_BEACON_TCPLISTENER_HH_*/
