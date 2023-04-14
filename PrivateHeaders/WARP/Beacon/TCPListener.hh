#ifndef BEACON_TCPLISTENER_HH_
# define BEACON_TCPLISTENER_HH_

# include <netinet/in.h>
# include <arpa/inet.h>

# include "WARP/Socket.hh"
# include "WARP/Listener.hh"


namespace WARP
{
	namespace Beacon
	{
		class TCPListener: public WARP::Listener
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

#endif /*!BEACON_TCPLISTENER_HH_*/
