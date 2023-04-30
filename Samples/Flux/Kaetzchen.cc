/* Kätzchen {n} [DE] - [EN] kitten; little cat
 *
 * a small program that reads from standard input and writes to standard
 * output in very small chunks; a kind of stripped-down version of the
 * Unix "cat" utility
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "WARP/Flux/Flux.h"

using namespace WARP::Flux;

class Kaetzchen: public RunLoopDelegate
{
	public:
		Kaetzchen()
		{
			/* create a RunLoop to manage the channels, we are its delegate */
			runLoop = new RunLoop(this);
			/* create a Channel for standard output (file descriptor #1) */
			output = new Channel(NULL, 1);
			/* create a small Buffer to hold the data we pass from standard input to
			 * standard output, using the output channel as its delegate
			 */
			buf = new Buffer(output, 16);
			/* create a Channel for standard input (file descriptor #0) with
			 * the Buffer as its delegate
			 */
			input = new Channel(buf, 0);
			/* add standard input to the RunLoop so that delegate events fire
			 * when data is available
			 */
			runLoop->add(input);
		}

		~Kaetzchen()
		{
			runLoop->release();
			input->release();
			output->release();
			buf->release();
		}

		int run(void)
		{
			runLoop->runToCompletion();
			return 0;
		}

		virtual bool shouldRunLoopTerminate(Object *sender, RunLoop *loop)
		{
			(void) sender;
			(void) loop;

			return input->closed() || output->closed();
		}
		
		protected:
			RunLoop *runLoop;
			Channel *input;
			Channel *output;
			Buffer *buf;
};

int
main(int argc, char **argv)
{
	Kaetzchen cat;

	(void) argc;
	(void) argv;

	return cat.run();
}
