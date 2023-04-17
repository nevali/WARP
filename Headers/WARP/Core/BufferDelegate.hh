#ifndef WARP_CORE_BUFFERDELEGATE_HH_
# define WARP_CORE_BUFFERDELEGATE_HH_

# include "Object.hh"

namespace WARP
{
	namespace Core
	{
		class Buffer;

		class BufferDelegate
		{
			public:
				virtual void bufferWritten(Object *sender, Buffer *buffer, char *base, size_t *nbytes);
		};
	}
}

#endif /*!WARP_CORE_BUFFERDELEGATE_HH_*/
