#ifndef WARP_CORE_PIPE_HH_
# define WARP_CORE_PIPE_HH_

# include "Object.hh"

namespace WARP
{
	namespace Core
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
			virtual int receiver(void) const;
			virtual int emitter(void) const;
		};
	}
}

#endif /*!WARP_CORE_PIPE_HH_*/
