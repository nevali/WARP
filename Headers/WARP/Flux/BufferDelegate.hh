#ifndef WARP_CORE_BUFFERDELEGATE_HH_
# define WARP_CORE_BUFFERDELEGATE_HH_

# include "Object.hh"

namespace WARP
{
	namespace Flux
	{
		class Buffer;

		struct BufferDelegate
		{
			/* invoked when the write position is advanced */
			virtual void bufferFilled(Object *sender, Buffer *buffer, char *pos, size_t *nbytes);
			/* invoked when the read position is advanced */
			virtual void bufferDrained(Object *sender, Buffer *buffer, char *pos, size_t nbytes);
		};
	}
}

#endif /*!WARP_CORE_BUFFERDELEGATE_HH_*/
