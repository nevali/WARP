#ifndef WARP_BEACON_TCPLISTENER_HH_
# define WARP_BEACON_TCPLISTENER_HH_

# include <netinet/in.h>
# include <arpa/inet.h>

# include "WARP/Core/Socket.hh"
# include "WARP/Core/Listener.hh"


namespace WARP
{
	namespace Beacon
	{
		class TCPListener: public Core::Listener
		{
		protected:
			class TCPClient: public Client
			{
				public:
					TCPClient(Core::Listener *listener, int fd, struct sockaddr_in *sin, socklen_t sinlen);
					virtual ~TCPClient(void);
				protected:
					struct sockaddr_in _sin;
			};
		public:
			TCPListener(Core::SocketDelegate *delegate, uint32_t addr = INADDR_ANY, uint16_t port = 0);
			virtual ~TCPListener();
			virtual int listen(int backlog = 0);
		protected:
			int _err;

			virtual Client *accept(void);
		};
	}
}

#endif /*!WARP_BEACON_TCPLISTENER_HH_*/
