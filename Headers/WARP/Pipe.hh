#ifndef WARP_PIPE_HH_
# define WARP_PIPE_HH_            1

namespace WARP
{

	struct Pipe
	{
	private:
		int _filedes[2];
	public:
		Pipe();
	
		virtual ~Pipe();
	
		virtual int receiver(void) const;
		virtual int emitter(void) const;
	};

}

#endif /*!WARP_PIPE_HH_*/
