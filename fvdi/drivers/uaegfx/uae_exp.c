/*
 * uae_exp.c - Monochrome expand functions
 * This is part of the WinUAE RTG driver for fVDI
 * Derived from Johan Klockars's example in ../16_bit/16b_exp.c
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
#include "uaelib.h"

#define PIXEL		short
#define PIXEL_SIZE	sizeof(PIXEL)

extern void CDECL c_get_colour(Virtual *vwk, long colour, short *foreground, short *background);

/*
 * Make it as easy as possible for the C compiler.
 * The current code is written to produce reasonable results with Lattice C.
 * (long integers, optimize: [x xx] time)
 * - One function for each operation -> more free registers
 * - 'int' is the default type
 * - some compilers aren't very smart when it comes to *, / and %
 * - some compilers can't deal well with *var++ constructs
 */

static void replace(short *src_addr, int src_line_add, PIXEL *dst_addr, int dst_line_add, int x, int w, int h, PIXEL foreground, PIXEL background)
{
	int i, j;
	unsigned int expand_word, mask;

	x = 1 << (15 - (x & 0x000f));

	for(i = h - 1; i >= 0; i--) {
		expand_word = *src_addr++;
		mask = x;
		for(j = w - 1; j >= 0; j--) {
			if (expand_word & mask) {
				*dst_addr++ = foreground;
			} else {
				*dst_addr++ = background;
			}
			if (!(mask >>= 1)) {
				mask = 0x8000;
				expand_word = *src_addr++;
			}
		}
		src_addr += src_line_add;
		dst_addr += dst_line_add;
	}
}

static void transparent(short *src_addr, int src_line_add, PIXEL *dst_addr, int dst_line_add, int x, int w, int h, PIXEL foreground, PIXEL background)
{
	int i, j;
	unsigned int expand_word, mask;

	x = 1 << (15 - (x & 0x000f));

	for(i = h - 1; i >= 0; i--) {
		expand_word = *src_addr++;
		mask = x;
		for(j = w - 1; j >= 0; j--) {
			if (expand_word & mask) {
				*dst_addr++ = foreground;
			} else {
				dst_addr++;
			}
			if (!(mask >>= 1)) {
				mask = 0x8000;
				expand_word = *src_addr++;
			}
		}
		src_addr += src_line_add;
		dst_addr += dst_line_add;
	}
}

static void xor(short *src_addr, int src_line_add, PIXEL *dst_addr, int dst_line_add, int x, int w, int h, PIXEL foreground, PIXEL background)
{
	int i, j, v;
	unsigned int expand_word, mask;

	x = 1 << (15 - (x & 0x000f));

	for(i = h - 1; i >= 0; i--) {
		expand_word = *src_addr++;
		mask = x;
		for(j = w - 1; j >= 0; j--) {
			if (expand_word & mask) {
				v = ~*dst_addr;
				*dst_addr++ = v;
			} else {
				dst_addr++;
			}
			if (!(mask >>= 1)) {
				mask = 0x8000;
				expand_word = *src_addr++;
			}
		}
		src_addr += src_line_add;
		dst_addr += dst_line_add;
	}
}

static void revtransp(short *src_addr, int src_line_add, PIXEL *dst_addr, int dst_line_add, int x, int w, int h, PIXEL foreground, PIXEL background)
{
	int i, j;
	unsigned int expand_word, mask;

	x = 1 << (15 - (x & 0x000f));

	for(i = h - 1; i >= 0; i--) {
		expand_word = *src_addr++;
		mask = x;
		for(j = w - 1; j >= 0; j--) {
			if (!(expand_word & mask)) {
				*dst_addr++ = foreground;
			} else {
				dst_addr++;
			}
			if (!(mask >>= 1)) {
				mask = 0x8000;
				expand_word = *src_addr++;
			}
		}
		src_addr += src_line_add;
		dst_addr += dst_line_add;
	}
}

long CDECL c_expand_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour)
{
	Workstation *wk;
	PIXEL *src_addr, *dst_addr;
	short foreground, background;
	int src_wrap, dst_wrap;
	int src_line_add, dst_line_add;
	unsigned long src_pos, dst_pos;

	KDEBUG(("c_expand_area: %ld,%ld from %ld,%ld to %ld,%ld\n", w, h, src_x, src_y, dst_x, dst_y));

	wk = vwk->real_address;

	c_get_colour(vwk, colour, &foreground, &background);

	src_wrap = (long)src->wdwidth * 2;		/* Always monochrome */
	src_addr = src->address;
	src_pos = (short)src_y * (long)src_wrap + (src_x >> 4) * 2;
	src_line_add = src_wrap - (((src_x + w) >> 4) - (src_x >> 4) + 1) * 2;

	if (!dst || !dst->address || (dst->address == wk->screen.mfdb.address)) {		/* To screen? */
		dst_wrap = wk->screen.wrap;
		dst_addr = wk->screen.mfdb.address;
	} else {
		dst_wrap = (long)dst->wdwidth * 2 * dst->bitplanes;
		dst_addr = dst->address;
	}
	dst_pos = (short)dst_y * (long)dst_wrap + dst_x * PIXEL_SIZE;
	dst_line_add = dst_wrap - w * PIXEL_SIZE;

	/* Try acceleration */
	{
		struct RenderInfo ri;
		struct Template template;

		ri.Memory = (uae_u8*)dst_addr;
		ri.BytesPerRow = dst_wrap;
		ri.RGBFormat = my_RGBFormat;

		template.Memory = (uae_u8*)src->address + src_wrap * src_y + src_x / 8;
		template.BytesPerRow = src_wrap;
		template.XOffset = src_x % 8;
		template.DrawMode = uae_drawmode[operation];
		template.FgPen = foreground;
		template.BgPen = background;

		if (uaelib_picasso_BlitTemplate(&my_BoardInfo, &ri, &template, dst_x, dst_y, w, h, 0xff, my_RGBFormat))
			return 1;
	}

	src_addr += src_pos / 2;
	dst_addr += dst_pos / PIXEL_SIZE;
	src_line_add /= 2;
	dst_line_add /= PIXEL_SIZE;			/* Change into pixel count */

	switch (operation) {
	case 1:				/* Replace */
		replace(src_addr, src_line_add, dst_addr, dst_line_add, src_x, w, h, foreground, background);
		break;
	case 2:				/* Transparent */
		transparent(src_addr, src_line_add, dst_addr, dst_line_add, src_x, w, h, foreground, background);
		break;
	case 3:				/* XOR */
		xor(src_addr, src_line_add, dst_addr, dst_line_add, src_x, w, h, foreground, background);
		break;
	case 4:				/* Reverse transparent */
		revtransp(src_addr, src_line_add, dst_addr, dst_line_add, src_x, w, h, foreground, background);
		break;
	}

	return 1;		/* Return as completed */
}
