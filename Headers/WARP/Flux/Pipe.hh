#ifndef WARP_FLUX_PIPE_HH_
# define WARP_FLUX_PIPE_HH_

# include "Object.hh"

namespace WARP
{
	namespace Flux
	{
		class Pipe: public Object
		{
		private:
			int _filedes[2];
		public:
			Pipe();
		protected:		
			virtual ~Pipe();
		public:
			virtual Kind kind(void) const;
			virtual int receiver(void) const;
			virtual int emitter(void) const;
		};
	}
}

#endif /*!WARP_FLUX_PIPE_HH_*/
