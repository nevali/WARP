#ifndef WARP_FLUX_DIAGNOSTICS_HH_
# define WARP_FLUX_DIAGNOSTICS_HH_

# include <cstdarg>
# include <cstdint>
# include <sys/types.h>

namespace WARP
{
	namespace Flux
	{
		namespace Diagnostics
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

			class Diagnostic
			{
				public:
					virtual DiagnosticLevel level(void) const = 0;
					virtual const char *message(void) const = 0;
			};

			typedef void (*DiagnosticCallback)(const Diagnostic *diag, void *context);

			bool setDiagnosticCallback(DiagnosticCallback cb, void *context);

			DiagnosticLevel setDiagnosticLevel(DiagnosticLevel level);
			DiagnosticLevel diagnosticLevel(void);

			void dump(const uint8_t *base, size_t length, size_t baseaddr = 0);

			void diag(DiagnosticLevel level, const char *str);
			void vdiagf(DiagnosticLevel level, const char *format, va_list ap); 
			void diagf(DiagnosticLevel level, const char *format, ...) __attribute__ (( format(printf, 2, 3) ));

			void debugf(const char *format, ...) __attribute__ (( format(printf, 1, 2) ));
			void tracef(const char *format, ...) __attribute__ (( format(printf, 1, 2) ));
		}
	}
}

#endif /*!WARP_FLUX_DIAGNOSTICS_HH_*/
