#ifndef WARP_LISTENER_HH_
# define WARP_LISTENER_HH_

# include <cstdlib>
# include <cstring>

# include "WARP/Buffer.hh"
# include "WARP/Socket.hh"

namespace WARP
{
	class Listener: public Socket
	{
	protected:
		class Client: public Socket
		{
			protected:
				friend class Listener;
				
				Client(Listener *listener, int fd = -1, Buffer *readBuf = NULL);
				virtual ~Client();

				virtual void processSet(fd_set *set);
			protected:
				Listener *_listener;
				Buffer *_readBuf;
		};

		class ClientSet: public SocketSet
		{
			public:
				ClientSet();
			public:
				virtual void addSet(fd_set *set);
				virtual void processSet(fd_set *set);
		};
	protected:
		Listener(SocketDelegate *delegate);
		virtual ~Listener();
	public:
		virtual void addSet(fd_set *set);
		virtual void processSet(fd_set *set);
	protected:
		virtual Client *accept(void) = 0;
		virtual void add(Client *client);
		virtual void remove(Client *client);
	protected:
		ClientSet _clients;
		Buffer _readBuf;
	};
}

#endif /*!WARP_LISTENER_HH_*/
