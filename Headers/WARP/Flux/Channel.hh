#ifndef WARP_FLUX_CHANNEL_HH_
# define WARP_FLUX_CHANNEL_HH_

# include <cstddef>

# include "EventSource.hh"
# include "BufferDelegate.hh"
# include "ChannelDelegate.hh"

namespace WARP
{
	namespace Flux
	{
		struct ChannelDelegate;
		class ChannelSet;
		class Buffer;
		
		/* A Channel represents a open file descriptor (file, socket, one end
		 * of a pipe, etc.)
		 *
		 * Channels are designed to work with Buffers, and (with the aid of
		 * ChannelSets), RunLoops.
		 * 
		 * adding a Channel to a ChannelSet or RunLoop will put it into non-
		 * blocking mode
		 * 
		 * a Buffer can be used as a delegate for a Channel (meaning the Buffer
		 * will be filled automatically when data is read)
		 * 
		 * a Channel can be used as a delegate for a Buffer (meaning the
		 * Buffer's contents will be written to the Channel whenever it needs
		 * to be drained)
		 */
		class Channel: public EventSource, public BufferDelegate
		{
		public:
			Channel(ChannelDelegate *delegate = NULL, int descriptor = -1);
			virtual ~Channel();
		public:
			virtual Kind kind(void) const;
			
			inline int descriptor(void) const
			{
				return _descriptor;
			}

			inline bool closed(void) const
			{
				return (_descriptor < 0) ? true : false;
			}

			inline ChannelDelegate *channelDelegate(void) const
			{
				return _delegate;
			}

			inline void setChannelDelegate(ChannelDelegate *delegate)
			{
				if(!_delegate)
				{
					_delegate = delegate;
					retain();
					_delegate->becameChannelDelegateFor(this, this);
					release();
				}
			}

			inline void clearChannelDelegate(ChannelDelegate *delegate)
			{
				if(_delegate == delegate)
				{
					_delegate = NULL;
				}
			}

			/* readyToReceive() allows the delegate to implement flow
			 * control; if the delegate returns false via this method,
			 * then the ChannelSet won't check for pending read activity
			 */
			bool readyToReceive(void);

			/* readPending() is true when there may be data pending a read()
			 * operation (or is false when no data is pending)
			 */
			inline bool readPending(void) const
			{
				return _readPending;
			}

			/* writeReady() is true when data can be written to the Channel
			 * (or is false when data cannot currently be written to Channel) */
			inline bool writeReady(void) const
			{
				return _writePending;
			}
		public:
			/* basic operations */
			virtual bool close(void);
			virtual ssize_t read(void *buf, size_t nbyte);
			virtual ssize_t read(Buffer *buf);
			virtual ssize_t write(const void *buf, size_t nbyte);
			virtual ssize_t write(Buffer *buf);
			
			virtual int setNonBlocking(bool opt = true);
			virtual int setCloseOnExec(bool opt = true);
			/* F_GETFD */
			virtual int flags(void);
			/* F_GETFL */
			virtual int statusFlags(void);
			/* F_SETFD */
			virtual int setFlags(int flags);
			/* F_SETFL */
			virtual int setStatusFlags(int statusFlags);
		public:
			/* BufferDelegate */
			virtual void bufferFilled(Object *sender, Buffer *buffer, char *pos, size_t *nbytes);
		protected:
			friend class ChannelSet;

			virtual void setReadPending(void);
			virtual void setWriteReady(void);
		private:
			ChannelDelegate *_delegate;
			int _descriptor;
			bool _readPending;
			bool _writePending;
		};
	}
}

#endif /*!WARP_FLUX_CHANNEL_HH_*/