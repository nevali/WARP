#ifndef WARP_SOCKETDELEGATE_HH_
# define WARP_SOCKETDELEGATE_HH_

# include <sys/types.h>

namespace WARP
{
	class Socket;

	class SocketDelegate
	{
	public:
		virtual void socketActivity(Socket *socket);
		virtual void socketReadBuffer(Socket *socket, const void *buf, size_t buflen);
	};
}

#endif /*!WARP_SOCKETDELEGATE_HH_*/
