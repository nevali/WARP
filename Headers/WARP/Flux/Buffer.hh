#ifndef WARP_BUFFER_HH_
# define WARP_BUFFER_HH_

# include <cstddef>

# include <sys/types.h>

# include "Object.hh"
# include "BufferDelegate.hh"

namespace WARP
{
	namespace Flux
	{
		class Buffer: public Object
		{
		public:
			static const size_t DEFSIZE = 4096;

			Buffer(BufferDelegate *delegate = NULL, size_t nbytes = DEFSIZE);
		protected:
			virtual ~Buffer();
		public:
			virtual Kind kind(void) const;

			inline char *base(void) const { return _base; };
			inline size_t size(void) const { return _size; };

			inline char *readPointer(void) const { return _base + _readPos; }
			inline char *writePointer(void) const { return _base + _writePos; }
			inline size_t readPosition(void) const { return _size - _readPos; }
			inline size_t writePosition(void) const { return _size - _writePos; }
			inline size_t remainingRead(void) const { return _size - _readPos; }
			inline size_t remainingWrite(void) const { return _size - _writePos; }

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

			bool advanceRead(size_t bytes);
			bool advanceWrite(size_t bytes);
			void rewindRead(size_t bytes);
			void rewindRead(void);
			void rewindWrite(size_t bytes);
			void rewindWrite(void);
			void consume(size_t bytes);
			void drain(void);
		protected:
			BufferDelegate *_bufferDelegate;
			char *_base;
			size_t _size;
			size_t _readPos;
			size_t _writePos;
		};

		class StaticBuffer: public Buffer
		{
		public:
			StaticBuffer(BufferDelegate *delegate = NULL, size_t nbytes = DEFSIZE): Buffer(delegate, nbytes) { }
			virtual ~StaticBuffer() { }
		private:
			using Buffer::retain;
			using Buffer::release;
		};

	}
}

#endif /*!WARP_BUFFER_HH_*/
