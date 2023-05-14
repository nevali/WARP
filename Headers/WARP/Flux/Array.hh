#ifndef WARP_FLUX_ARRAY_HH_
# define WARP_FLUX_ARRAY_HH_

# include <cstddef>

# include "Object.hh"

namespace WARP
{
	namespace Flux
	{
		namespace Internal
		{
			struct ArrayData;

			class Array: public Object
			{
				public:
					static const size_t NOTFOUND = (size_t)(-1);
				protected:
					Array(void);
					virtual ~Array();
				public:
					virtual Kind kind(void) const;
					virtual size_t count(void) const;
				protected:
					virtual size_t indexOfPointer(void *ptr) const;
					virtual void *pointerAtIndex(size_t index) const;
					virtual size_t addPointer(void *ptr);
					virtual bool removePointer(void *ptr);
					virtual void pointerWasRemoved(void *ptr) const;
					virtual void forEachPointer(bool (*callback)(void *ptr, void *ctx), void *ctx);
				private:
					ArrayData *_data;
			};

			template <class T> class TArray: public Array
			{
				public:
					TArray(): Array()
					{
					}

				protected:
					virtual ~TArray()
					{
					}

				public:
					/* adding an object retains it */
					virtual size_t add(T *object)
					{
						if(object && indexOfObject(object) == NOTFOUND)
						{
							object->retain();
							return Array::addPointer(object);
						}
						return NOTFOUND;
					}

					/* removing an object releases it */
					virtual bool remove(T *object)
					{
						return Array::removePointer(object);
					}

					virtual size_t indexOfObject(T *object)
					{
						return Array::indexOfPointer(object);
					}

					/* obtaining an object retains it for you -- you must release when done */
					virtual T *objectAtIndex(size_t index) const __attribute__ ((warn_unused_result))
					{
						T *object = static_cast<T *>(Array::pointerAtIndex(index));
						if(object)
						{
							object->retain();
						}
						return object;
					}
				protected:
				 	/* invoked by the base implementation automatically
					 * whenever a pointer is deleted from the array
					 */
					virtual void pointerWasRemoved(void *ptr) const
					{
						static_cast<T *>(ptr)->release();
					}

			};

		}

		using Internal::TArray;

		typedef TArray<Object> Array;
	}
}

#endif /*!WARP_FLUX_ARRAY_HH_*/