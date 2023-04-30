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
			virtual void bufferWritten(Object *sender, Buffer *buffer, char *base, size_t *nbytes);
		};
	}
}

#endif /*!WARP_CORE_BUFFERDELEGATE_HH_*/
