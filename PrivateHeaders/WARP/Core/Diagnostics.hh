#ifndef WARP_CORE_DIAGNOSTICS_HH_
# define WARP_CORE_DIAGNOSTICS_HH_

# include <cstdarg>
# include <cstdint>

namespace WARP
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

	void dump(uint8_t *base, size_t length, size_t baseaddr = 0);

	void diag(DiagnosticLevel level, const char *str);
	void vdiagf(DiagnosticLevel level, const char *format, va_list ap);
	void diagf(DiagnosticLevel level, const char *format, ...);

	void debugf(const char *format, ...);
	void tracef(const char *format, ...);
}

#endif /*!WARP_CORE_DIAGNOSTICS_HH_*/
