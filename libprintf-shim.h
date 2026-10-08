/*
 * libprintf-shim.h
 *
 * Lightweight static-analysis-friendly shim that declares a broad set of
 * printf-family symbols. It exists so scanners can include this single header
 * instead of fighting the system libc headers directly when they only care
 * about call-site discovery and format-string shape.
 *
 * It is intentionally not a libc replacement. Do not link real code against
 * this header as if it were stdio.h.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* Core stream + string printers. */
int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);

/* va_list variants. Declared with the system va_list so this shim stays
 * compatible with the surrounding libc headers when it is included after
 * stdio.h or when it is included on its own by an analysis tool. */
int vprintf(const char *format, va_list ap);
int vfprintf(FILE *stream, const char *format, va_list ap);
int vsprintf(char *str, const char *format, va_list ap);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);

/* Wide-character printf family. */
int wprintf(const wchar_t *format, ...);
int fwprintf(FILE *stream, const wchar_t *format, ...);
int swprintf(wchar_t *str, size_t size, const wchar_t *format, ...);
int vwprintf(const wchar_t *format, va_list ap);
int vfwprintf(FILE *stream, const wchar_t *format, va_list ap);
int vswprintf(wchar_t *str, size_t size, const wchar_t *format, va_list ap);

/* GNU extensions commonly encountered in real codebases. */
int asprintf(char **str, const char *format, ...);
int vasprintf(char **str, const char *format, va_list ap);

#if defined(__cplusplus)
}
#endif
