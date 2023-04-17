#ifndef WARP_CORE_SOCKETBUFFER_HH_
# define WARP_CORE_SOCKETBUFFER_HH_

#include "Buffer.hh"
#include "SocketDelegate.hh"

namespace WARP
{
	namespace Core
	{
		/* A buffer that can be used as a delegate for a socket, streaming
		* any incoming data into the buffer
		*/
		class SocketBuffer: public Buffer, public SocketDelegate
		{
			public:
				SocketBuffer(BufferDelegate *delegate = NULL,  size_t nbytes = Buffer::DEFSIZE, bool autoDrain = true);
				virtual ~SocketBuffer();
			public:
				/* SocketDelegate */
				virtual void socketActivity(Object *sender, Socket *socket);
			protected:
				bool _autoDrain;
		};
	}
}

#endif /*!WARP_CORE_SOCKETBUFFER_HH_*/