#ifndef WARP_CORE_BUFFERDELEGATE_HH_
# define WARP_CORE_BUFFERDELEGATE_HH_

# include <cstddef>

# include "Object.hh"

namespace WARP
{
	namespace Flux
	{
		class Buffer;

		struct BufferDelegate
		{
			BufferDelegate(): _bufferDelegateFor(NULL) { };
			virtual ~BufferDelegate();
			
			virtual void becameBufferDelegateFor(Object *sender, Buffer *buffer);
			/* invoked when the write position is advanced */
			virtual void bufferFilled(Object *sender, Buffer *buffer, char *pos, size_t *nbytes);
			/* invoked when the read position is advanced */
			virtual void bufferDrained(Object *sender, Buffer *buffer, char *pos, size_t nbytes);
			/* Invoked when a source has been created */
			virtual void sourceOpened(Object *sender, Object *source);
			/* Invoked when a source has been closed */
			virtual void sourceClosed(Object *sender, Object *source);
			private:
				Buffer *_bufferDelegateFor;
		};
	}
}

#endif /*!WARP_CORE_BUFFERDELEGATE_HH_*/
