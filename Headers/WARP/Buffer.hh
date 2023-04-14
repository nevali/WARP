#ifndef WARP_BUFFER_HH_
# define WARP_BUFFER_HH_

# include <sys/types.h>

namespace WARP
{
	class Buffer
	{
	public:
		static const size_t DEFSIZE = 4096;

		Buffer(size_t nbytes = DEFSIZE);
		~Buffer();

		inline char *base(void) const { return _base; };
		inline size_t size(void) const { return _size; };
		inline size_t remaining(const void *ptr) const
		{
			if(_base && ptr > _base && ptr < _base + _size )
			{
				return _size - ((const char *) _base - (const char *) ptr);
			}
			else
			{
				return 0;
			}
		}
		void reset(void);
	protected:
		char *_base;
		size_t _size;
	};
}

#endif /*!WARP_BUFFER_HH_*/
