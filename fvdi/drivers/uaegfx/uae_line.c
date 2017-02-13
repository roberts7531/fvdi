/*
 * uae_line.c - Line functions
 * This is part of the WinUAE RTG driver for fVDI
 * Derived from Johan Klockars's example in ../16_bit/16b_line.c
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

/*#define ENABLE_KDEBUG*/

#include "fvdi.h"
#include "uaegfx.h"

extern void CDECL c_get_colour(Virtual *vwk, long colour, short *foreground, short *background);

extern long CDECL clip_line(Virtual *vwk, long *x1, long *y1, long *x2, long *y2);

/*
 * Make it as easy as possible for the C compiler.
 * The current code is written to produce reasonable results with Lattice C.
 * (long integers, optimize: [x xx] time)
 * - One function for each operation -> more free registers
 * - 'int' is the default type
 * - some compilers aren't very smart when it comes to *, / and %
 * - some compilers can't deal well with *var++ constructs
 */

static void replace(short *addr, short *addr_fast, int count,
                    int d, int incrE, int incrNE, int one_step, int both_step,
                    short foreground, short background)
{
	*addr = foreground;

	for(--count; count >= 0; count--) {
		if (d < 0) {
			d += incrE;
			addr += one_step;
		} else {
			d += incrNE;
			addr += both_step;
		}
		*addr = foreground;
	}
}

static void replace_p(short *addr, short *addr_fast, long pattern, int count,
                      int d, int incrE, int incrNE, int one_step, int both_step,
                      short foreground, short background)
{
	unsigned int mask = 0x8000;

	if (pattern & mask) {
		*addr = foreground;
	} else {
		*addr = background;
	}
		
	for(--count; count >= 0; count--) {
		if (d < 0) {
			d += incrE;
			addr += one_step;
		} else {
			d += incrNE;
			addr += both_step;
		}

		if (!(mask >>= 1))
			mask = 0x8000;

		if (pattern & mask) {
			*addr = foreground;
		} else {
			*addr = background;
		}
	}
}

static void transparent(short *addr, short *addr_fast, int count,
                        int d, int incrE, int incrNE, int one_step, int both_step,
                        short foreground, short background)
{
	*addr = foreground;

	for(--count; count >= 0; count--) {
		if (d < 0) {
			d += incrE;
			addr += one_step;
		} else {
			d += incrNE;
			addr += both_step;
		}
		*addr = foreground;
	}
}

static void transparent_p(short *addr, short *addr_fast, long pattern, int count,
                          int d, int incrE, int incrNE, int one_step, int both_step,
                          short foreground, short background)
{
	unsigned int mask = 0x8000;

	if (pattern & mask) {
		*addr = foreground;
	}
		
	for(--count; count >= 0; count--) {
		if (d < 0) {
			d += incrE;
			addr += one_step;
		} else {
			d += incrNE;
			addr += both_step;
		}

		if (!(mask >>= 1))
			mask = 0x8000;

		if (pattern & mask) {
			*addr = foreground;
		}
	}
}

static void xor(short *addr, short *addr_fast, int count,
                int d, int incrE, int incrNE, int one_step, int both_step,
                short foreground, short background)
{
	int v;

	v = ~*addr;
	*addr = v;

	for(--count; count >= 0; count--) {
		if (d < 0) {
			d += incrE;
			addr += one_step;
		} else {
			d += incrNE;
			addr += both_step;
		}
		v = ~*addr;
		*addr = v;
	}
}

static void xor_p(short *addr, short *addr_fast, long pattern, int count,
                  int d, int incrE, int incrNE, int one_step, int both_step,
                  short foreground, short background)
{
	int v;
	unsigned int mask = 0x8000;

	if (pattern & mask) {
		v = ~*addr;
		*addr = v;
	}
		
	for(--count; count >= 0; count--) {
		if (d < 0) {
			d += incrE;
			addr += one_step;
		} else {
			d += incrNE;
			addr += both_step;
		}

		if (!(mask >>= 1))
			mask = 0x8000;

		if (pattern & mask) {
			v = ~*addr;
			*addr = v;
		}
	}
}

static void revtransp(short *addr, short *addr_fast, int count,
                      int d, int incrE, int incrNE, int one_step, int both_step,
                      short foreground, short background)
{
	*addr = foreground;

	for(--count; count >= 0; count--) {
		if (d < 0) {
			d += incrE;
			addr += one_step;
		} else {
			d += incrNE;
			addr += both_step;
		}
		*addr = foreground;
	}
}

static void revtransp_p(short *addr, short *addr_fast, long pattern, int count,
                        int d, int incrE, int incrNE, int one_step, int both_step,
                        short foreground, short background)
{
	unsigned int mask = 0x8000;

	if (!(pattern & mask)) {
		*addr = foreground;
	}
		
	for(--count; count >= 0; count--) {
		if (d < 0) {
			d += incrE;
			addr += one_step;
		} else {
			d += incrNE;
			addr += both_step;
		}

		if (!(mask >>= 1))
			mask = 0x8000;

		if (!(pattern & mask)) {
			*addr = foreground;
		}
	}
}

long CDECL c_line_draw(Virtual *vwk, long x1, long y1, long x2, long y2,
                       long pattern, long colour, long mode)
{
	Workstation *wk;
	short *addr, *addr_fast;
	short foreground, background;
  	int line_add;
	long pos;
	int x_step, y_step;
	int dx, dy;
	int one_step, both_step;
	int d, count;
	int incrE, incrNE;

	if ((long)vwk & 1) {
		return -1;			/* Don't know about anything yet */
	}

	KDEBUG(("c_line_draw %ld,%ld to %ld,%ld\n", x1, y1, x2, y2));

	if (!clip_line(vwk, &x1, &y1, &x2, &y2))
		return 1;

	c_get_colour(vwk, colour, &foreground, &background);

	wk = vwk->real_address;

	pos = (short)y1 * (long)wk->screen.wrap + x1 * 2;
	addr = wk->screen.mfdb.address;
	line_add = wk->screen.wrap >> 1;


	x_step = 1;
	y_step = line_add;
	
	dx = x2 - x1;
	if (dx < 0) {
		dx = -dx;
		x_step = -x_step;
	}
	dy = y2 - y1;
	if (dy < 0) {
		dy = -dy;
		y_step = -y_step;
	}
	
	if (dx > dy) {
		count = dx;
		one_step = x_step;
		incrE = 2 * dy;
		incrNE = 2 * dy - 2 * dx;
		d = 2 * dy - dx;
	} else {
		count = dy;
		one_step = y_step;
		incrE = 2 * dx;
		incrNE = 2 * dx - 2 * dy;
		d = 2 * dx - dy;
	}
	both_step = x_step + y_step;

	addr += pos >> 1;
	if ((pattern & 0xffff) == 0xffff) {
		switch (mode) {
		case 1:				/* Replace */
			replace(addr, addr_fast, count, d, incrE, incrNE, one_step, both_step, foreground, background);
			break;
		case 2:				/* Transparent */
			transparent(addr, addr_fast, count, d, incrE, incrNE, one_step, both_step, foreground, background);
			break;
		case 3:				/* XOR */
			xor(addr, addr_fast, count, d, incrE, incrNE, one_step, both_step, foreground, background);
			break;
		case 4:				/* Reverse transparent */
			revtransp(addr, addr_fast, count, d, incrE, incrNE, one_step, both_step, foreground, background);
			break;
		}
	} else {
		switch (mode) {
		case 1:				/* Replace */
			replace_p(addr, addr_fast, pattern, count, d, incrE, incrNE, one_step, both_step, foreground, background);
			break;
		case 2:				/* Transparent */
			transparent_p(addr, addr_fast, pattern, count, d, incrE, incrNE, one_step, both_step, foreground, background);
			break;
		case 3:				/* XOR */
			xor_p(addr, addr_fast, pattern, count, d, incrE, incrNE, one_step, both_step, foreground, background);
			break;
		case 4:				/* Reverse transparent */
			revtransp_p(addr, addr_fast, pattern, count, d, incrE, incrNE, one_step, both_step, foreground, background);
			break;
		}
	}

	return 1;		/* Return as completed */
}
