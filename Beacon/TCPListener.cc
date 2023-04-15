#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Beacon/TCPListener.hh"

using namespace WARP;
using namespace WARP::Beacon;

TCPListener::TCPListener(SocketDelegate *delegate, uint32_t addr, uint16_t port):
	Listener(delegate),
	_err(0)
{
	struct sockaddr_in sin;
	socklen_t sinlen;

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_len = sizeof(sin);
	sin.sin_addr.s_addr = htonl(addr);
	sin.sin_port = htons(port);

	_fd = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(_fd < 0)
	{
		/* XXX we should throw on errors here */
		_err = errno;
		return;
	}
	sinlen = sizeof(sin);
	if(::bind(_fd, reinterpret_cast<sockaddr *>(&sin), sinlen) < 0)
	{
		_err = errno;
		::close(_fd);
		debugf("failed to bind socket: %s\n", strerror(_err));
		_fd = -1;
		return;
	}
	/* XXX setsockopt() - SO_REUSEADDR */
	if(::getsockname(_fd, reinterpret_cast<sockaddr *>(&sin), &sinlen) < 0)
	{
		_err = errno;
		::close(_fd);
		debugf("failed to obtain socket name: %s\n", strerror(_err));
		_fd = -1;
		return;
	}
	/* XXX check that sinlen == sizeof(sin) */
	debugf("TCPListener: listening on port %d\n", (int) ntohs(sin.sin_port));
}

TCPListener::~TCPListener()
{
	if(_fd >= 0)
	{
		::close(_fd);
	}
}

int
TCPListener::listen(int backlog)
{
	if(_fd < 0)
	{
		errno = _err;
		return -1;
	}
	return ::listen(_fd, backlog);
}

Listener::Client *
TCPListener::accept(void)
{
	TCPClient *client;
	int clientfd;

	struct sockaddr_in sin;
	socklen_t sinlen;

	sinlen = sizeof(sin);
	memset(&sin, 0, sinlen);
	clientfd = ::accept(_fd, reinterpret_cast<sockaddr *>(&sin), &sinlen);
	if(clientfd < 0)
	{
		debugf("TCPListener::TCPClient: accept failed: %s\n", strerror(errno));
		return NULL;
	}
	if(sin.sin_family != AF_INET)
	{
		debugf("TCPListener::accept: sin_family != AF_INET?\n");
		::close(clientfd);
		return NULL;
	}
	if(sinlen != sizeof(struct sockaddr_in))
	{
		debugf("TCPListener::accept: sinlen != struct(sockaddr_in)\n");
		::close(clientfd);
		return NULL;
	}
	client = new TCPClient(this, clientfd, &sin, sinlen);
	add(client);
	return client;
}

TCPListener::TCPClient::TCPClient(Listener *listener, int fd, struct sockaddr_in *sin, socklen_t sinlen):
	Listener::Client(listener, fd)
{
	memset(&_sin, 0, sizeof(_sin));
	if(sinlen > sizeof(_sin))
	{
		sinlen = sizeof(_sin);
	}
	memcpy(&_sin, sin, sinlen);
	/* XXX inet_ntoa() is generally unsafe, it's used here only for debug output */
	debugf("TCPListener::TCPClient: new connection #%d from %s:%d\n", _fd, inet_ntoa(_sin.sin_addr), (int) htons(_sin.sin_port));
}

TCPListener::TCPClient::~TCPClient()
{
}
