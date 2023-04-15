#ifndef WARP_PACKET_HH_
# define WARP_PACKET_HH_

#include <cstddef>
#include <cstdint>
#include <sys/types.h>

namespace WARP
{
	class Packet
	{
		public:
			typedef enum
			{
				PKT_INVALID,
				PKT_IDENT,
				PKT_PAYLOAD
			} Type;

			/* The "wire format" between the Beacon and a server application
			 * is defined in the Encoded structure, below
			 */
#pragma pack(push, 1)
			struct Encoded
			{
				static const uint16_t MAGIC_BEACON = 0xBEAC;

				uint16_t seq;
				uint16_t length;
				Type type:8;
				uint8_t reserved;

				static const size_t PACKET_HEADER = 6;

				union
				{
					struct
					{
						uint16_t magic;
						uint16_t version;
						char banner[]; 
					} ident;
					struct
					{
						/* this implies a limit of 65536 multiplexed connections */
						uint16_t socket;
						uint8_t data[];
					} payload;
				};

				static const size_t PACKET_IDENT_HEADER = 4;
				static const size_t PACKET_PAYLOAD_HEADER = 2;

				ssize_t send(int fd);
			};
#pragma pack(pop)

		protected:
			Packet(Type type = PKT_INVALID);
		public:
			virtual ~Packet();

			/* Encode this packet using the buffer provided and return a
			 * pointer to it
			 */
		public:
			virtual Encoded *encode(void *buf, size_t bufsize) = 0;
			static Packet *decode(Packet::Encoded *encoded);
			virtual Type type(void) const;
		protected:
			Type _type;
			static uint16_t _nextSeq;
	};

	class PayloadPacket: public Packet
	{
		public:
			PayloadPacket(int connection, const void *buf, size_t buflen):
				Packet(PKT_PAYLOAD), _conn(connection), _buf(buf), _buflen(buflen) {}
			virtual Encoded *encode(void *buf, size_t bufsize);
			virtual const uint8_t *buffer(void) const;
			virtual size_t size(void) const;
		protected:
			int _conn;
			const void *_buf;
			size_t _buflen;
	};
}

#endif /*!WARP_PACKET_HH_*/
