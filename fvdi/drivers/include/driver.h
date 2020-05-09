#ifndef DRIVER_H
#define DRIVER_H

#include <stdarg.h>

/* 
 * fVDI driver declarations, by Johan Klockars.
 *
 * Since it would be difficult to do without this file when
 * writing new device drivers, and to make it possible for
 * some such drivers to be commercial, this file is put in
 * the public domain. It's not copyrighted or under any sort
 * of license.
 */

#include "relocate.h"

#define MAGIC     "InitMagic"

#define A_MOUSE		0x01
#define A_TEXT		(A_MOUSE << 1)
#define A_BLIT		(A_TEXT << 1)
#define A_FILL		(A_BLIT << 1)
#define A_FILLPOLY	(A_FILL << 1)
#define A_EXPAND	(A_FILLPOLY << 1)
#define A_LINE		(A_EXPAND << 1)
#define A_SET_PAL	(A_LINE << 1)
#define A_GET_COL	(A_SET_PAL << 1)
#define A_SET_PIX	(A_GET_COL << 1)
#define A_GET_PIX	(A_SET_PIX << 1)

#define ACCEL_ALL	((A_LINE << 1) - 1)		/* Only functions covered by this may be turned off */

#ifdef FVDI_DEBUG
#  define unreachable() kprintf("%s: \"unreachable\" in %s line %d\n", driver_name, __FILE__, __LINE__)
#endif
#ifndef unreachable
#  ifdef __GNUC__
#    define unreachable() __builtin_unreachable()
#  endif
#endif

typedef struct _MBits {
	char *red;
	char *green;
	char *blue;
	char *alpha;
	char *genlock;
	char *unused;
} MBits;

typedef struct _Mode {
	short bpp;
	short flags;
	MBits bits;
	short code;		/* 0, DEPTH_SUPPORT_565, ? */
	short format;		/* 0 (interleaved), 2 (packed pixels) */
	short clut;		/* 1 (hardware), 2 (software) */
	short org;		/* 1 (usual bit order), 0x81 (Falcon 5+6+5 bit order, but Intel byte order), ? */
} Mode;

#ifdef FVDI_DEBUG
#define PUTS(x) access->funcs.puts(x)
#define PRINTF(x) kprintf x
#define KEY_WAIT(x) key_wait(x)
#else
#define PUTS(x)
#define PRINTF(x)
#define KEY_WAIT(x)
#endif


#endif
