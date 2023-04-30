#ifndef WARP_FLUX_OBJECT_HH_
# define WARP_FLUX_OBJECT_HH_

namespace WARP
{
	namespace Flux
	{
		/* A base for all ordinary Flux classes */
		class Object
		{
			public:
				typedef enum
				{
					OBJECT,
					ARRAY,
					BUFFER,
					CHANNEL,
					CHANNELSET,
					LISTENER,
					TASK,
					RUNLOOP		
				} Kind;
			protected:
				Object();
				virtual ~Object();
			public:
				virtual unsigned retain(void);
				virtual unsigned release(void);
				virtual Kind kind(void) const;
			private:
				unsigned _refCount;
		};
	}
}

#endif /*!WARP_FLUX_OBJECT_HH_*/