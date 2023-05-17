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
			virtual void runLoopIsStarting(Object *sender, RunLoop *loop);
			virtual void runLoopEnded(Object *sender, RunLoop *loop);
			virtual void runLoopPass(Object *sender, RunLoop *loop);
			virtual bool shouldRunLoopTerminate(Object *sender, RunLoop *loop);
		};
	}
}

#endif /*!WARP_FLUX_RUNLOOPDELEGATE_HH_*/
