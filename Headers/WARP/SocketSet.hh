#ifndef WARP_SOCKETSET_HH_
# define WARP_SOCKETSET_HH_

# include <sys/types.h>
# include <sys/socket.h>

namespace WARP
{
	class Socket;

	class SocketSet
	{
		public:
			SocketSet();
			virtual ~SocketSet();
		public:
			void add(Socket *client);
			void remove(Socket *client);
			void addSet(fd_set *set);
			void processSet(fd_set *set);
		protected:
			Socket **_clients;
			size_t _nclients;
	};
}

#endif /*!WARP_SOCKETSET_HH_*/
