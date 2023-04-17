#ifndef WARP_CORE_SOCKETSET_HH_
# define WARP_CORE_SOCKETSET_HH_

# include <sys/types.h>
# include <sys/socket.h>

# include "Object.hh"

namespace WARP
{
	namespace Core
	{
		class Socket;

		/* This is not a reference-counted object, it's designed to be 
		 * statically-allocated
		 */
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
}

#endif /*!WARP_CORE_SOCKETSET_HH_*/
