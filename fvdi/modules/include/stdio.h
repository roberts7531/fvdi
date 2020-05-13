#include "libkern.h"

long ksprintf(char *str, const char *format, ...) __attribute__((format(printf, 2, 3)));
long kvsprintf(char *str, const char *format, va_list args) __attribute__((format(printf, 2, 0)));
long kprintf(const char *format, ...) __attribute__((format(printf, 1, 2)));

int sprintf(char *str, const char *format, ...) __attribute__((format(printf, 2, 3)));

/* The possibilities for the third argument to `fseek'.
   These values should not be changed.  */
#define	SEEK_SET	0	/* Seek from beginning of file. */
#define	SEEK_CUR	1	/* Seek from current position. */
#define	SEEK_END	2	/* Seek from end of file. */
