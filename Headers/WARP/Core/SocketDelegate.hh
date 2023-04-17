#ifndef WARP_CORE_SOCKETDELEGATE_HH_
# define WARP_CORE_SOCKETDELEGATE_HH_

# include <sys/types.h>

# include "Object.hh"

namespace WARP
{
	namespace Core
	{
		class Socket;

		class SocketDelegate
		{
		public:
			/* Invoked when a socket has been created */
			virtual void socketOpened(Object *sender, Socket *socket);
			/* Invoked when a socket has been closed */
			virtual void socketClosed(Object *sender, Socket *socket);
			/* Invoked when there is some kind of activity on a socket */
			virtual void socketActivity(Object *sender, Socket *socket);
			/* Invoked when data has been read from a socket; on return, *buflen
			* should be set to the number of bytes from the buffer that were
			* consumed
			*/
			virtual void socketReadBuffer(Object *sender, Socket *socket, const void *buf, size_t *buflen);
		};
	}
}

#endif /*!WARP_CORE_SOCKETDELEGATE_HH_*/
