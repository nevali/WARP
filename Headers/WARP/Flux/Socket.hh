#ifndef WARP_FLUX_SOCKET_HH_
# define WARP_FLUX_SOCKET_HH_

# include <sys/types.h>
# include <sys/socket.h>

# include "Channel.hh"

namespace WARP
{
	namespace Flux
	{
		/* a Socket is a specialisation of Channel that exposes the
		 * capabilities of network sockets
		 */
		class Socket: public Channel
		{
		protected:
			Socket(ChannelDelegate *delegate, int fd = -1);
			virtual ~Socket();
		public:
		 	/* additional operations */
//			virtual int pending(void);
/*			virtual ssize_t read(void *buf, size_t length);
			virtual ssize_t read(void *buf, size_t length, int flags);
			virtual ssize_t write(const void *buf, size_t length);
			virtual ssize_t write(const void *buf, size_t length, int flags); */
		};
	}
}

#endif /*!WARP_FLUX_SOCKET_HH_*/
