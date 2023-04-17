#ifndef WARP_CORE_DIAGNOSTICS_HH_
# define WARP_CORE_DIAGNOSTICS_HH_

# include <cstdarg>
# include <cstdint>
# include <sys/types.h>

namespace WARP
{
	namespace Core
	{
		typedef enum
		{
			DIAG_INFO,
			DIAG_HINT,
			DIAG_NOTICE,
			DIAG_WARNING,
			DIAG_ERROR,
			DIAG_CRITICAL,
			DIAG_EMERGENCY,

			DIAG_DEBUG = -1,
			DIAG_TRACE = -7
		} DiagnosticLevel;

		void dump(const uint8_t *base, size_t length, size_t baseaddr = 0);

		void diag(DiagnosticLevel level, const char *str);
		void vdiagf(DiagnosticLevel level, const char *format, va_list ap); 
		void diagf(DiagnosticLevel level, const char *format, ...) __attribute__ (( format(printf, 2, 3) ));

		void debugf(const char *format, ...) __attribute__ (( format(printf, 1, 2) ));
		void tracef(const char *format, ...) __attribute__ (( format(printf, 1, 2) ));
	}
}

#endif /*!WARP_CORE_DIAGNOSTICS_HH_*/
