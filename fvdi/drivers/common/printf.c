/*
 * fVDI generic device driver printf support
 *
 * This file is put in the public domain.
 * It's not copyrighted or under any sort of license.
 */

#include <stdarg.h>

#include "os.h"
#include "fvdi.h"
#include "relocate.h"
#include "driver.h"

#ifdef FVDI_DEBUG

void ultoa(char *buf, unsigned long un, unsigned long base);
#include "stdlib/ultoa.h"

void DRIVER_EXPORT copymem(const void *s, void *d, long n);
void copymem_aligned(const void *s, void *d, long n);
#include "string/memcpy.h"
#include "string/memmove.h"

#define strlen(s) access->funcs.length(s)

#include "stdio/printf.h"

#endif
