#ifndef WARP_BEACON_PACKETS_HH_
# define WARP_BEACON_PACKETS_HH_

#include "Packet.hh"

namespace WARP
{
	namespace Beacon
	{
		namespace Packets
		{
			/* This family of classes represents the packets exchanged between a
			* a server and the Beacon process
			*/
			class Ident: public Packet
			{
				public:
					Ident(unsigned version, const char *banner):
						Packet(PKT_IDENT), _version(version), _banner(banner) {}
					virtual Encoded *encode(void *buf, size_t bufsize);
				public:
					virtual unsigned version(void) const { return _version; };
					virtual const char *banner(void) const { return _banner; };
				protected:
					unsigned _version;
					const char *_banner;
			};

			class ConnectionOpened: public Packet
			{
				public:
					ConnectionOpened(int id): Packet(PKT_OPEN), _id(id) {}
					virtual Encoded *encode(void *buf, size_t bufsize);
				public:
					virtual int id(void) const { return _id; };
				protected:
					int _id;
			};

			class ConnectionClosed: public Packet
			{
				public:
					ConnectionClosed(int id): Packet(PKT_CLOSE), _id(id) {}
					virtual Encoded *encode(void *buf, size_t bufsize);
				public:
					virtual int id(void) const { return _id; };
				protected:
					int _id;
			};

			class Payload: public Packet
			{
				public:
					Payload(int connection, const void *buf, size_t buflen):
						Packet(PKT_PAYLOAD), _conn(connection), _buf(buf), _buflen(buflen) {}
					virtual Encoded *encode(void *buf, size_t bufsize);
				public:
					virtual int connection(void) const { return _conn; };
					virtual const uint8_t *buffer(void) const { return reinterpret_cast<const uint8_t *>(_buf); };
					virtual size_t size(void) const { return _buflen; };
				protected:
					int _conn;
					const void *_buf;
					size_t _buflen;
			};
		}
	}
}

#endif /*!WARP_BEACON_PACKETS_HH_*/