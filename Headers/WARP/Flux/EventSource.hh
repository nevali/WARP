#ifndef FLUX_WARP_EVENTSOURCE_HH_
# define FLUX_WARP_EVENTSOURCE_HH_

# include "Object.hh"

namespace WARP
{
	namespace Flux
	{
		/* An EventSource is any type of object that can be added to a
		 * RunLoop (see RunLoop::add())
		 */
		class EventSource: public Object
		{
		protected:
			friend class RunLoop;

			EventSource();
			virtual ~EventSource();
			
			virtual void processPendingEvents(void);
		};
	}
}

#endif /*!FLUX_WARP_EVENTSOURCE_HH_*/
