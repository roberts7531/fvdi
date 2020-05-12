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
	short format;	/* 0 (interleaved), 2 (packed pixels) */
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


/*
 * from common code
 */
extern Driver *me;
extern Access *access;
extern short *loaded_palette;
extern short default_vdi_colors[256][3];
extern long wk_extend;
extern Device device;
extern short debug;


long CDECL init(Access *_access, Driver *driver, Virtual *vwk, char *);

/*
 * from various spec.c
 */
extern const Mode *graphics_mode;
extern short accel_s;	/* Bit vector of available assembly acceleration routines */
extern short accel_c;	/* The same for C versions */
/*
 * some drivers hack this to match the configuration,
 * so it can't be const
 */
extern char driver_name[];

/*
 * function pointers that are called by
 * the assembly interface routines,
 * These must be initialized by the driver.
 */
extern long CDECL (*get_colour_r)(Virtual *vwk, long colour);
extern void CDECL (*get_colours_r)(Virtual *vwk, long colour, long *foreground, long *background);
extern void CDECL (*set_colours_r)(Virtual *vwk, long start, long entries, unsigned short *requested, Colour palette[]);
extern long CDECL (*write_pixel_r)(Virtual *vwk, MFDB *mfdb, long x, long y, long colour);
extern long CDECL (*read_pixel_r)(Virtual *vwk, MFDB *mfdb, long x, long y);
extern long CDECL (*line_draw_r)(Virtual * vwk, long x1, long y1, long x2, long y2, long pattern, long colour, long mode);
extern long CDECL (*expand_area_r)(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour);
extern long CDECL (*fill_area_r)(Virtual *vwk, long x, long y, long w, long h, short *pattern, long colour, long mode, long interior_style);
extern long CDECL (*fill_poly_r)(Virtual *vwk, short points[], long n, short index[], long moves, short *pattern, long colour, long mode, long interior_style);
extern long CDECL (*blit_area_r)(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation);
extern long CDECL (*text_area_r)(Virtual *vwk, short *text, long length, long dst_x, long dst_y, short *offsets);
extern long CDECL (*mouse_draw_r)(Workstation *wk, long x, long y, Mouse *mouse);

void CDECL check_linea(Workstation *);

long CDECL initialize(Virtual *vwk);
long check_token(char *, const char **);
long CDECL setup(long, long);
Virtual *CDECL opnwk(Virtual *);
void CDECL clswk(Virtual *);
void setup_scrninfo(Device *device, const Mode *graphics_mode);
long tokenize(const char *ptr);

void CDECL initialize_palette(Virtual *vwk, long start, long entries, short requested[][3], Colour palette[]);
void CDECL c_initialize_palette(Virtual *vwk, long start, long entries, short requested[][3], Colour palette[]);
long CDECL clip_line(Virtual *vwk, long *x1, long *y1, long *x2, long *y2);

/*
 * api entry points called from kernel
 */
/*
 * Necessary device driver functions
 */
void CDECL set_palette(Virtual *vwk, DrvPalette *palette_pars);
void CDECL c_set_palette(Virtual *vwk, DrvPalette *palette_pars);
long CDECL colour(Virtual *vwk, long colours);
long CDECL c_colour(Virtual *vwk, long colours);
void CDECL set_pixel(Virtual *vwk, MFDB *mfdb, long x, long y, long colour);
void CDECL c_set_pixel(Virtual *vwk, MFDB *mfdb, long x, long y, long colour);
long CDECL get_pixel(Virtual *vwk, MFDB *mfdb, long x, long y);
long CDECL c_get_pixel(Virtual *vwk, MFDB *mfdb, long x, long y);
/*
 * Acceleration functions
 */
long CDECL line(Virtual *vwk, DrvLine *pars);
long CDECL c_line(Virtual *vwk, DrvLine *pars);
void CDECL expand(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour);
void CDECL c_expand(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour);
void CDECL fill(Virtual *vwk, long x, long y, long w, long h, short *pattern, long colour, long mode, long interior_style);
void CDECL c_fill(Virtual *vwk, long x, long y, long w, long h, short *pattern, long colour, long mode, long interior_style);
void CDECL fillpoly(Virtual *vwk, short points[], long n, short index[], long moves, short *pattern, long colour, long mode, long interior_style);
void CDECL c_fillpoly(Virtual *vwk, short points[], long n, short index[], long moves, short *pattern, long colour, long mode, long interior_style);
long CDECL blit(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation);
long CDECL c_blit(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation);
void CDECL text(Virtual *vwk, short *text, long length, long dst_x, long dst_y, short *offsets);
void CDECL c_text(Virtual *vwk, short *text, long length, long dst_x, long dst_y, short *offsets);
void CDECL mouse(Workstation *wk, long x, long y, Mouse *mouse);
void CDECL c_mouse(Workstation *wk, long x, long y, Mouse *mouse);

#endif
