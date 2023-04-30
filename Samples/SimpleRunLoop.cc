#include "WARP/Flux/Flux.h"

int
main(int argc, char **argv)
{
	WARP::Flux::RunLoop *loop;
	
	(void) argc;
	(void) argv;

	loop = new WARP::Flux::RunLoop();
	loop->runToCompletion();
	loop->release();
	
	return 0;

}