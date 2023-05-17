#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstdlib>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/ChannelDelegate.hh"
#include "WARP/Flux/Channel.hh"

using namespace WARP::Flux;

ChannelDelegate::~ChannelDelegate()
{
	if(_channelDelegateFor)
	{
		tracef("ChannelDelegate::%s() ceasing to be delegate for Channel<%p>\n", __FUNCTION__, _channelDelegateFor);
		_channelDelegateFor->clearChannelDelegate(this);
		_channelDelegateFor = NULL;
	}
}

void
ChannelDelegate::becameChannelDelegateFor(Object *sender, Channel *channel)
{
	(void) sender;

	_channelDelegateFor = channel;
}

void
ChannelDelegate::channelOpened(Object *sender, Channel *channel)
{
	tracef("ChannelDelegate: Channel<%p>[#%d] opened (Sender = Object<%p>)\n", channel, channel->descriptor(), sender);
}

void
ChannelDelegate::channelClosed(Object *sender, Channel *channel)
{
	tracef("ChannelDelegate: Channel<%p>[#%d] closed (Sender = Object<%p>)\n", channel, channel->descriptor(), sender);
}

/* Invoked when a Channel is ready to be read from */
void
ChannelDelegate::channelReadPending(Object *sender, Channel *channel)
{
	tracef("ChannelDelegate: Channel<%p>[#%d] is ready for read (Sender = Object<%p>)\n", channel, channel->descriptor(), sender);
}

/* Invoked when a Channel is ready to be written to */
void
ChannelDelegate::channelWriteReady(Object *sender, Channel *channel)
{
	tracef("ChannelDelegate: Channel<%p>[#%d] is ready for write (Sender = Object<%p>)\n", channel, channel->descriptor(), sender);
}

/* Invoked to check whether a Channel can receive data */
bool
ChannelDelegate::isChannelReadyToReceive(Object *sender, Channel *channel)
{
	tracef("ChannelDelegate: Channel<%p>[#%d] is ready to receive (Sender = Object<%p>)\n", channel, channel->descriptor(), sender);
	return true;
}
