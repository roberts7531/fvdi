/*
 * uaegfx.h - General functions and macros
 * This is part of the WinUAE RTG driver for fVDI
 *
 * Copyright (C) 2017 Vincent Riviere
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef UAEGFX_H
#define UAEGFX_H

#include "types.h"

/* Utility functions */
void uaegfx_puts(const char* message);
void panic(const char* message);
void panic_help(const char* message);

/*
 * This tiny debug framework is borrowed from EmuTOS.
 * https://sourceforge.net/p/emutos/code/ci/master/tree/include/kprint.h
 */

/* Attribute to check parameters of printf-like functions at compile time */
#ifdef __GNUC__
#define PRINTF_STYLE __attribute__ ((format (printf, 1, 2)))
#else
#define PRINTF_STYLE
#endif

/* You can use EmuTOS kprintf() to display text in the WinUAE debug log.
 * To enable it, you must compile EmuTOS yourself, get the _kprintf address
 * from emutos.map, and replace it below. */
typedef void kprintf_t(const char *fmt, ...) PRINTF_STYLE;
/*#define kprintf (*(kprintf_t*)0x00fc52c0)*/

/* KINFO(()) outputs to the debugger, if kprintf() is available */
#ifdef kprintf
#define KINFO(args) kprintf args
#else
#define KINFO(args)
#endif

/* KDEBUG(()) may call KINFO(()) when locally enabled */
#ifdef ENABLE_KDEBUG
#define KDEBUG(args) KINFO(args)
#else
#define KDEBUG(args)
#endif

long CDECL c_expand_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour);
long CDECL c_blit_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation);

void CDECL initialize_palette(Virtual *vwk, long start, long entries, short requested[][3], Colour palette[]);
void CDECL c_initialize_palette(Virtual *vwk, long start, long entries, short requested[][3], Colour palette[]);

long CDECL c_line_draw(Virtual *vwk, long x1, long y1, long x2, long y2, long pattern, long colour, long mode);
long CDECL c_fill_area(Virtual *vwk, long x, long y, long w, long h, short *pattern, long colour, long mode, long interior_style);
long CDECL c_write_pixel(Virtual *vwk, MFDB *dst, long x, long y, long colour);
long CDECL c_read_pixel(Virtual *vwk, MFDB *src, long x, long y);
long CDECL c_get_colour(Virtual *vwk, long colour);
void CDECL c_set_colours(Virtual *vwk, long start, long entries, unsigned short *requested, Colour palette[]);
long CDECL c_mouse_draw(Workstation *wk, long x, long y, Mouse *mouse);

#endif /* UAEGFX_H */
