#ifndef WARP_SOCKET_HH_
# define WARP_SOCKET_HH_

# include <sys/types.h>
# include <sys/socket.h>

# include "SocketSet.hh"
# include "SocketDelegate.hh"

namespace WARP
{
	class Socket
	{
	protected:
		Socket(SocketDelegate *delegate, int fd = -1);
		virtual ~Socket();

		friend class SocketSet;
	protected:
		int _fd;
		SocketDelegate *_delegate;
	public:
		virtual void addSet(fd_set *set);
		virtual void processSet(fd_set *set);
		virtual int id(void);
	};
}

#endif /*!WARP_SOCKET_HH_*/
