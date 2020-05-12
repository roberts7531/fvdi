/*
 * uae_fill.c - Fill functions
 * This is part of the WinUAE RTG driver for fVDI
 * Derived from Johan Klockars's example in ../16_bit/16b_fill.c
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
#include "driver.h"
#include "uaegfx.h"
#include "uaelib.h"


/*
 * Make it as easy as possible for the C compiler.
 * The current code is written to produce reasonable results with Lattice C.
 * (long integers, optimize: [x xx] time)
 * - One function for each operation -> more free registers
 * - 'int' is the default type
 * - some compilers aren't very smart when it comes to *, / and %
 * - some compilers can't deal well with *var++ constructs
 */

static void replace(short *addr, short *addr_fast, int line_add, short *pattern, int x, int y, int w, int h, short foreground, short background)
{
	int i, j;
	unsigned int pattern_word, mask;

	(void) addr_fast;
	i = y;
	h = y + h;
	x = 1 << (15 - (x & 0x000f));

	for(; i < h; i++) {
		pattern_word = pattern[i & 0x000f];
		switch (pattern_word) {
		case 0xffff:
			for(j = w - 1; j >= 0; j--) {
				*addr = foreground;
				addr++;
			}
			break;
		default:
			mask = x;
			for(j = w - 1; j >= 0; j--) {
				if (pattern_word & mask) {
					*addr = foreground;
					addr++;
				} else {
					*addr = background;
					addr++;
				}
				if (!(mask >>= 1))
					mask = 0x8000;
			}
			break;
		}
		addr += line_add;
	}
}

static void transparent(short *addr, short *addr_fast, int line_add, short *pattern, int x, int y, int w, int h, short foreground, short background)
{
	int i, j;
	unsigned int pattern_word, mask;

	(void) addr_fast;
	(void) background;
	i = y;
	h = y + h;
	x = 1 << (15 - (x & 0x000f));

	for(; i < h; i++) {
		pattern_word = pattern[i & 0x000f];
		switch (pattern_word) {
		case 0xffff:
			for(j = w - 1; j >= 0; j--) {
				*addr = foreground;
				addr++;
			}
			break;
		default:
			mask = x;
			for(j = w - 1; j >= 0; j--) {
				if (pattern_word & mask) {
					*addr = foreground;
					addr++;
				} else {
					addr++;
				}
				if (!(mask >>= 1))
					mask = 0x8000;
			}
			break;
		}
		addr += line_add;
	}
}

static void xor(short *addr, short *addr_fast, int line_add, short *pattern, int x, int y, int w, int h, short foreground, short background)
{
	int i, j;
	unsigned int pattern_word, mask, v;

	(void) addr_fast;
	(void) foreground;
	(void) background;
	i = y;
	h = y + h;
	x = 1 << (15 - (x & 0x000f));

	for(; i < h; i++) {
		pattern_word = pattern[i & 0x000f];
		switch (pattern_word) {
		case 0xffff:
			for(j = w - 1; j >= 0; j--) {
				v = ~*addr;
				*addr = v;
				addr++;
			}
			break;
		default:
			mask = x;
			for(j = w - 1; j >= 0; j--) {
				if (pattern_word & mask) {
					v = ~*addr;
					*addr = v;
					addr++;
				} else {
					addr++;
				}
				if (!(mask >>= 1))
					mask = 0x8000;
			}
			break;
		}
		addr += line_add;
	}
}

static void revtransp(short *addr, short *addr_fast, int line_add, short *pattern, int x, int y, int w, int h, short foreground, short background)
{
	int i, j;
	unsigned int pattern_word, mask;

	(void) addr_fast;
	(void) background;
	i = y;
	h = y + h;
	x = 1 << (15 - (x & 0x000f));

	for(; i < h; i++) {
		pattern_word = pattern[i & 0x000f];
		switch (pattern_word) {
		case 0x0000:
			for(j = w - 1; j >= 0; j--) {
				*addr = foreground;
				addr++;
			}
			break;
		default:
			mask = x;
			for(j = w - 1; j >= 0; j--) {
				if (!(pattern_word & mask)) {
					*addr = foreground;
					addr++;
				} else {
					addr++;
				}
				if (!(mask >>= 1))
					mask = 0x8000;
			}
			break;
		}
		addr += line_add;
	}
}

/* Determine if all the pattern lines are equal to the value */
static int is_solid_pattern(short *pattern, short value)
{
    int y;

    for (y = 0; y < 16; y++) {
        if (pattern[y] != value)
            return 0;
    }

    return 1;
}

long CDECL c_fill_area(Virtual *vwk, long x, long y, long w, long h, short *pattern, long colour, long mode, long interior_style)
{
	Workstation *wk;
	short *addr, *addr_fast;
    long colours;
	short foreground, background;
  	int line_add;
	long pos;

	KDEBUG(("c_fill_area %ld,%ld at %ld,%ld\n", w, h, x, y));

	(void) interior_style;
	if ((long)vwk & 1) {
		if ((y & 0xffff) != 0)
			return -1;		/* Don't know about this kind of table operation */
		h = (y >> 16) & 0xffff;
		vwk = (Virtual *)((long)vwk - 1);
		return -1;			/* Don't know about anything yet */
	}

	colours = c_get_colour(vwk, colour);
    foreground = colours;
    background = colours >> 16;

	wk = vwk->real_address;

	/* Try acceleration */
	{
		struct RenderInfo ri;
		struct Pattern pat;

		ri.Memory = (uae_u8*)wk->screen.mfdb.address;
		ri.BytesPerRow = wk->screen.wrap;
		ri.RGBFormat = my_RGBFormat;

		/* Special optimization */
		if (mode == 1)
		{
			if (is_solid_pattern(pattern, 0x0000))
			{
				if (uaelib_picasso_FillRect(&my_BoardInfo, &ri, x, y, w, h, background, 0xff, my_RGBFormat))
					return 1;
			}
			else if (is_solid_pattern(pattern, 0xffff))
			{
				if (uaelib_picasso_FillRect(&my_BoardInfo, &ri, x, y, w, h, foreground, 0xff, my_RGBFormat))
					return 1;
			}
		}

		/* Standard acceleration */
		pat.Memory = (uae_u8*)pattern;
		pat.XOffset = x % 16;
		pat.YOffset = y % 16;
		pat.FgPen = foreground;
		pat.BgPen = background;
		pat.Size = 4;
		pat.DrawMode = uae_drawmode[mode];

		if (uaelib_picasso_BlitPattern(&my_BoardInfo, &ri, &pat, x, y, w, h, 0xff, my_RGBFormat))
			return 1;
	}

	pos = (short)y * (long)wk->screen.wrap + x * 2;
	addr = wk->screen.mfdb.address;
	line_add = (wk->screen.wrap - w * 2) >> 1;

	addr += pos >> 1;
	switch (mode) {
	case 1:				/* Replace */
		replace(addr, addr_fast, line_add, pattern, x, y, w, h, foreground, background);
		break;
	case 2:				/* Transparent */
		transparent(addr, addr_fast, line_add, pattern, x, y, w, h, foreground, background);
		break;
	case 3:				/* XOR */
		xor(addr, addr_fast, line_add, pattern, x, y, w, h, foreground, background);
		break;
	case 4:				/* Reverse transparent */
		revtransp(addr, addr_fast, line_add, pattern, x, y, w, h, foreground, background);
		break;
	}

	return 1;		/* Return as completed */
}
