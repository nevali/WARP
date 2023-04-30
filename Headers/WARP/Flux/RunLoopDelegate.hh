#ifndef WARP_FLUX_RUNLOOPDELEGATE_HH_
# define WARP_FLUX_RUNLOOPDELEGATE_HH_

namespace WARP
{
	namespace Flux
	{
		class Object;
		class RunLoop;

		struct RunLoopDelegate
		{
			virtual bool shouldRunLoopTerminate(Object *sender, RunLoop *loop);
		};
	}
}

#endif /*!WARP_FLUX_RUNLOOPDELEGATE_HH_*/
