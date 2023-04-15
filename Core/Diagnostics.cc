#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <cassert>
#include <cctype>

#include <unistd.h>

#include "WARP/Core/Diagnostics.hh"

#include "WARP/Buffer.hh"

using namespace WARP;

#define DUMPWIDTH 16

#  if __LLP64__
#   define ADDRFORMAT "%04llx"
#   define ADDRCAST unsigned long long
#  else
#   define ADDRFORMAT "%04lx"
#   define ADDRCAST unsigned long
#  endif

#define LINEWIDTH ((DUMPWIDTH * 4) + 12 + 4)

typedef uintptr_t ADDR;

static const uint8_t *dump_row(const uint8_t *base, size_t width, ADDR *length, size_t skip, ADDR baseaddr);
static void diag_prefix(DiagnosticLevel level, char *buf, size_t bufsize);
static void writeBuf(void);

static Buffer diagbuf;
static DiagnosticLevel diagLevel = DIAG_TRACE;


void
WARP::diag(DiagnosticLevel level, const char *str)
{
	size_t size;
	char *ptr;

	if(level < diagLevel)
	{
		return;
	}
	ptr = diagbuf.base();
	size = strlen(diagbuf.base());
	ptr[0] = 0;
	diag_prefix(level, ptr, size);
	ptr = strchr(ptr, 0);
	size -= (ptr - diagbuf.base());
	snprintf(ptr, size, "%s\n", str);
	writeBuf();
}

void
WARP::vdiagf(DiagnosticLevel level, const char *format, va_list ap)
{
	size_t size;
	char *ptr;

	if(level < diagLevel)
	{
		return;
	}
	ptr = diagbuf.base();
	size = diagbuf.size();
	ptr[0] = 0;
	diag_prefix(level, ptr, size);
	ptr = strchr(ptr, 0);
	size -= (ptr - diagbuf.base());
	ptr[0] = 0;
	vsnprintf(ptr, size, format, ap);
	writeBuf();
}

void
WARP::diagf(DiagnosticLevel level, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	WARP::vdiagf(level, format, ap);
	va_end(ap);
}

void
WARP::debugf(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	WARP::vdiagf(DIAG_DEBUG, format, ap);
	va_end(ap);
}

void
WARP::tracef(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	WARP::vdiagf(DIAG_TRACE, format, ap);
	va_end(ap);
}

/* Dump a region of memory starting at base and extending for length bytes
 *
 * The bytes are displayed in hexadecimal, with the printable contents shown
 * along the side. Non-printable characters are rendered as '.', e.g:
 *
 * 000104b141e0 10 11 12 13 14 15 16 17-18 19 1A 1B 1C 1D 1E 1F ................
 * 000104b141f0 20 21 22 23 24 25 26 27-28 29 2A 2B 2C 2D 2E 2F  !"#$%&'()*+,-./
 *
 */
void
WARP::dump(const uint8_t *base, size_t length, size_t baseaddr)
{
	ADDR offset;
	
	offset = ((uintptr_t) base) % DUMPWIDTH;
//	debugf("offset = %d\n", (int) offset);
//	baseaddr = (ADDR) base;
	while(length)
	{
		base = dump_row(base, DUMPWIDTH, &length, offset, baseaddr);
		baseaddr += 16 - offset;
		offset = 0;
	}
}

static void
diag_prefix(DiagnosticLevel level, char *buf, size_t bufsize)
{
	switch(level)
	{
		case DIAG_TRACE:
			snprintf(buf, bufsize, "[Trace]    ");
			break;
		case DIAG_DEBUG:
			snprintf(buf, bufsize, "[Debug]    ");
			break;
		case DIAG_INFO:
			snprintf(buf, bufsize, "           ");
			break;
		case DIAG_HINT:
			snprintf(buf, bufsize, "HINT:      ");
			break;
		case DIAG_NOTICE:
			snprintf(buf, bufsize, "NOTICE:    ");
			break;
		case DIAG_WARNING:
			snprintf(buf, bufsize, "WARNING:   ");
			break;
		case DIAG_ERROR:
			snprintf(buf, bufsize, "ERROR:     ");
			break;
		case DIAG_CRITICAL:
			snprintf(buf, bufsize, "CRITICAL:  ");
			break;
		case DIAG_EMERGENCY:
			snprintf(buf, bufsize, "EMERGENCY: ");
			break;

		default:
			snprintf(buf, bufsize, "<%d> ", (int) level);
	}
}

static void
writeBuf(void)
{
	size_t size;
	const char *ptr;
	ssize_t r;

	ptr = diagbuf.base();
	size = strlen(ptr);
	while(size)
	{
		do
		{
			r = write(STDERR_FILENO, ptr, size);
		}
		while(r == -1 && (errno == EINTR || errno == EAGAIN));
		if(r < 0)
		{
			return;
		}
		else if((size_t) r > size)
		{
			r = size;
		}
		size -= r;
		ptr += r;
	}
}

/* Dump a single row of width bytes, starting at base, decrementing
 * *length until it reaches zero
 *
 * If skip is nonzero, then output is left-padded by that many blanks
 *
 * If fewer than width bytes were written then output is right-padded
 * by that many bytes
 *
 * The return value is a pointer to the next unwritten byte from base
 */
static const uint8_t *
dump_row(const uint8_t *base, size_t width, ADDR *length, size_t skip, ADDR baseaddr)
{
	uint8_t byte;
	char strbuf[DUMPWIDTH + 1];
	char *ptr;
	size_t index;
	
	assert(width <= DUMPWIDTH);
	assert(skip <= width);
	diag_prefix(DIAG_DEBUG, diagbuf.base(), diagbuf.size());
	ptr = strchr(diagbuf.base(), 0);
//	debugf("base = %p, baseaddr = %p, skip = %u\n", base, baseaddr, skip);
	if(baseaddr - skip > baseaddr)
	{
		/* handle underflow slightly gracefully */
		snprintf(ptr, diagbuf.remaining(ptr), ADDRFORMAT " ", (ADDR) 0);
	}
	else
	{
		snprintf(ptr, diagbuf.remaining(ptr), ADDRFORMAT " ", (ADDR) (baseaddr - skip));
	}
	ptr = strchr(diagbuf.base(), 0);
	index = 0;
	/* if skip is nonzero, the first displayed byte is not aligned on
	 * width-sized boundary, and so the first row is indented so that
	 * the second and subsequent rows will line up properly
	 */
	if(skip)
	{
		*ptr = ' '; ptr++;
		*ptr = ' '; ptr++;
		strbuf[index] = ' ';
		for(index++; index < skip; ++index)
		{
			*ptr = ' '; ptr++;
			*ptr = ' '; ptr++;
			*ptr = ' '; ptr++;
			strbuf[index] = ' ';
		}
		*ptr = 0;
	}
	/* dump up to (width - skip) bytes  */
	for(;index < width && *length; ++index)
	{
		byte = base[0];
		base++;
		/* non-printable characters are rendered as a . */
		strbuf[index] = (isprint(byte) ? (char) byte : '.');
		/* before printing all but the first hex bytes, add a separator;
		 * the separator between the 8th and 9th bytes (i.e., half-way)
		 * is a dash instead of a space
		 */
		if(index)
		{
			*ptr = ((skip && !(skip % 8)) || index % 8) ? ' ' : '-';
			ptr++;
		}
		/* add the byte itself in uppercase hexadecimal */
		snprintf(ptr, diagbuf.remaining(ptr), "%02X", (int) byte);
		ptr += 2;
		(*length)--;
		*ptr = 0;
	}
	/* if we dumped fewer than width bytes, pad out the right-hand side */
	for(; index < width; ++index)
	{
		*ptr = ' '; ptr++;
		*ptr = ' '; ptr++;
		*ptr = ' '; ptr++;
		strbuf[index] = ' ';
	}
	/* null-terminate the ASCII buffer */
	strbuf[index] = 0;
	/* add a separator */
	*ptr = ' '; ptr++;
	/* append the ASCII buffer */
	strcpy(ptr, strbuf);
	/* locate the new end */
	ptr = strchr(ptr, 0);
	/* add a newline and terminate */
	*ptr = '\n'; ptr++;
	*ptr = 0;
	writeBuf();
	return base;
}
