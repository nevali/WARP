#ifndef WARP_FLUX_LISTENER_HH_
# define WARP_FLUX_LISTENER_HH_

# include <cstddef>

# include "Buffer.hh"
# include "Socket.hh"
# include "ChannelSet.hh"

namespace WARP
{
	namespace Flux
	{
		/* a Listener is a type of socket which can accept client connections
		 * (which are created as new instances of a distinct subclass of
		 * Listener::Client), and are integrated with ChannelSet and RunLoop
		 * so that processing of client connection activity can happen
		 * automatically.
		 */
		class Listener: public Socket
		{
		protected:
			class Client: public Socket
			{
				protected:
					friend class Listener;
					
					Client(Listener *listener, int fd = -1);
					virtual ~Client();

					virtual void processSet(fd_set *set);
				protected:
					Listener *_listener;
					Buffer *_readBuf;
			};

			class ClientSet: public ChannelSet
			{
				public:
					ClientSet();
				protected:
					virtual ~ClientSet();
				public:
					virtual void addSet(fd_set *set);
					virtual void processSet(fd_set *set);
			};
		protected:
			Listener(ChannelDelegate *delegate);
			virtual ~Listener();
		public:
//			virtual void addSet(fd_set *set);
//			virtual void processSet(fd_set *set);
		protected:
			virtual Client *accept(void) = 0;
			virtual void add(Client *client);
			virtual void remove(Client *client);
		protected:
			ClientSet *_clients;
		};
	}
}

#endif /*!WARP_FLUX_LISTENER_HH_*/
