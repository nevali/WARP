#ifndef WARP_CORE_OBJECT_HH_
# define WARP_CORE_OBJECT_HH_

namespace WARP
{
	namespace Core
	{
		class Object
		{
			protected:
				Object();
			protected:
				virtual ~Object(void);
			public:
				unsigned retain(void);
				unsigned release(void);
			private:
				unsigned _refCount;
		};
	}
}

#endif /*!WARP_CORE_OBJECT_HH_*/