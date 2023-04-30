#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstdlib>

#include "WARP/Flux/Diagnostics.hh"
#include "WARP/Flux/ChannelDelegate.hh"
#include "WARP/Flux/Channel.hh"

using namespace WARP::Flux;

void
ChannelDelegate::channelOpened(Object *sender, Channel *channel)
{
	tracef("ChannelDelegate: channel #%d opened\n", channel->descriptor());
}

void
ChannelDelegate::channelClosed(Object *sender, Channel *channel)
{
	tracef("ChannelDelegate: channel #%d closed\n", channel->descriptor());
}

/* Invoked when there's activity on a channel (client or listener) */
void
ChannelDelegate::channelActivity(Object *sender, Channel *channel)
{
	tracef("ChannelDelegate: activity on channel #%d\n", channel->descriptor());
}

/* Invoked when data has been read from a channel */
void
ChannelDelegate::channelReadBuffer(Object *sender, Channel *channel, const void *buf, size_t *buflen)
{
	(void) channel;
	(void) buf;

	tracef("ChannelDelegate: read %lu bytes from channel #%d\n", (unsigned long) *buflen, channel->descriptor());
}
