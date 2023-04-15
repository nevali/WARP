#ifndef WARP_SOCKETDELEGATE_HH_
# define WARP_SOCKETDELEGATE_HH_

# include <sys/types.h>

namespace WARP
{
	class Socket;

	class SocketDelegate
	{
	public:
		/* Invoked when a socket has been created */
		virtual void socketOpened(Socket *socket);
		/* Invoked when a socket has been closed */
		virtual void socketClosed(Socket *socket);
		/* Invoked when there is some kind of activity on a socket */
		virtual void socketActivity(Socket *socket);
		/* Invoked when data has been read from a socket */
		virtual void socketReadBuffer(Socket *socket, const void *buf, size_t buflen);
	};
}

#endif /*!WARP_SOCKETDELEGATE_HH_*/
