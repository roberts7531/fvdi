/*
 * uae_blit.c - Blit functions
 * This is part of the WinUAE RTG driver for fVDI
 * Derived from Johan Klockars's example in ../16_bit/16b_blit.c
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

#define PIXEL		short
#define PIXEL_SIZE	sizeof(PIXEL)

/*
 * Make it as easy as possible for the C compiler.
 * The current code is written to produce reasonable results with Lattice C.
 * (long integers, optimize: [x xx] time)
 * - One function for each operation -> more free registers
 * - 'int' is the default type
 * - some compilers aren't very smart when it comes to *, / and %
 * - some compilers can't deal well with *var++ constructs
 */

static void
blit_copy(PIXEL *src_addr, int src_line_add,
          PIXEL *dst_addr, int dst_line_add,
          int w, int h)
{
	int i, j;
	PIXEL v;

	for(i = h - 1; i >= 0; i--) {
		for(j = w - 1; j >= 0; j--) {
			v = *src_addr++;
			*dst_addr++ = v;
		}
		src_addr += src_line_add;
		dst_addr += dst_line_add;
	}
}


static void
blit_or(PIXEL *src_addr, int src_line_add,
        PIXEL *dst_addr, int dst_line_add,
        int w, int h)
{
	int i, j;
	PIXEL v;

	for(i = h - 1; i >= 0; i--) {
		for(j = w - 1; j >= 0; j--) {
			v = *src_addr++;
			*dst_addr++ |= v;
		}
		src_addr += src_line_add;
		dst_addr += dst_line_add;
	}
}


static void uae_blit(PIXEL *src_addr, int src_line_add,
     PIXEL *dst_addr, int dst_line_add,
     int w, int h, int operation)
{
	int i, j;
	PIXEL v, vs, vd;

	for(i = h - 1; i >= 0; i--) {
		for(j = w - 1; j >= 0; j--) {
			vs = *src_addr++;
			vd = *dst_addr;
			switch(operation) {
			case 0:
				v = 0;
				break;
			case 1:
				v = vs & vd;
				break;
			case 2:
				v = vs & ~vd;
				break;
			case 3:
				v = vs;
				break;
			case 4:
				v = ~vs & vd;
				break;
			case 5:
				v = vd;
				break;
			case 6:
				v = vs ^ vd;
				break;
			case 7:
				v = vs | vd;
				break;
			case 8:
				v = ~(vs | vd);
				break;
			case 9:
				v = ~(vs ^ vd);
				break;
			case 10:
				v = ~vd;
				break;
			case 11:
				v = vs | ~vd;
				break;
			case 12:
				v = ~vs;
				break;
			case 13:
				v = ~vs | vd;
				break;
			case 14:
				v = ~(vs & vd);
				break;
			case 15:
				v = 0xffff;
				break;
			default:
				/* Should not happen */
				v = 0;
				break;
			}
			*dst_addr++ = v;
		}
		src_addr += src_line_add;
		dst_addr += dst_line_add;
	}
}


static void
pan_backwards_copy(PIXEL *src_addr, int src_line_add,
                   PIXEL *dst_addr, int dst_line_add,
                   int w, int h)
{
	int i, j;
	PIXEL v;

	for(i = h - 1; i >= 0; i--) {
		for(j = w - 1; j >= 0; j--) {
			v = *--src_addr;
			*--dst_addr = v;
		}
		src_addr += src_line_add;
		dst_addr += dst_line_add;
	}
}


static void
pan_backwards_or(PIXEL *src_addr, int src_line_add,
                 PIXEL *dst_addr, int dst_line_add,
                 int w, int h)
{
	int i, j;
	PIXEL v;

	for(i = h - 1; i >= 0; i--) {
		for(j = w - 1; j >= 0; j--) {
			v = *--src_addr;
			*--dst_addr |= v;
		}
		src_addr += src_line_add;
		dst_addr += dst_line_add;
	}
}


static void
pan_backwards(PIXEL *src_addr, int src_line_add,
              PIXEL *dst_addr, int dst_line_add,
              int w, int h, int operation)
{
	int i, j;
	PIXEL v, vs, vd;

	for(i = h - 1; i >= 0; i--) {
		for(j = w - 1; j >= 0; j--) {
			vs = *--src_addr;
			vd = *--dst_addr;
			switch(operation) {
			case 0:
				v = 0;
				break;
			case 1:
				v = vs & vd;
				break;
			case 2:
				v = vs & ~vd;
				break;
			case 3:
				v = vs;
				break;
			case 4:
				v = ~vs & vd;
				break;
			case 5:
				v = vd;
				break;
			case 6:
				v = vs ^ vd;
				break;
			case 7:
				v = vs | vd;
				break;
			case 8:
				v = ~(vs | vd);
				break;
			case 9:
				v = ~(vs ^ vd);
				break;
			case 10:
				v = ~vd;
				break;
			case 11:
				v = vs | ~vd;
				break;
			case 12:
				v = ~vs;
				break;
			case 13:
				v = ~vs | vd;
				break;
			case 14:
				v = ~(vs & vd);
				break;
			case 15:
				v = 0xffff;
				break;
			default:
				/* Should not happen */
				v = 0;
				break;
			}
			*dst_addr = v;
		}
		src_addr += src_line_add;
		dst_addr += dst_line_add;
	}
}


long CDECL
c_blit_area(Virtual *vwk, MFDB *src, long src_x, long src_y,
            MFDB *dst, long dst_x, long dst_y,
            long w, long h, long operation)
{
	Workstation *wk;
	PIXEL *src_addr, *dst_addr;
	int src_wrap, dst_wrap;
	int src_line_add, dst_line_add;
	unsigned long src_pos, dst_pos;

	KDEBUG(("c_blit_area %ld,%ld from %ld,%ld to %ld,%ld\n", w, h, src_x, src_y, dst_x, dst_y));

	wk = vwk->real_address;

	if (!src || !src->address || (src->address == wk->screen.mfdb.address)) {		/* From screen? */
		src_wrap = wk->screen.wrap;
		src_addr = wk->screen.mfdb.address;
	} else {
		src_wrap = (long)src->wdwidth * 2 * src->bitplanes;
		src_addr = src->address;
	}
	src_pos = (short)src_y * (long)src_wrap + src_x * PIXEL_SIZE;
	src_line_add = src_wrap - w * PIXEL_SIZE;

	if (!dst || !dst->address || (dst->address == wk->screen.mfdb.address)) {		/* To screen? */
		dst_wrap = wk->screen.wrap;
		dst_addr = wk->screen.mfdb.address;
	} else {
		dst_wrap = (long)dst->wdwidth * 2 * dst->bitplanes;
		dst_addr = dst->address;
	}
	dst_pos = (short)dst_y * (long)dst_wrap + dst_x * PIXEL_SIZE;
	dst_line_add = dst_wrap - w * PIXEL_SIZE;

	if (src_y < dst_y) {
		src_pos += (short)(h - 1) * (long)src_wrap;
		src_line_add -= src_wrap * 2;
		dst_pos += (short)(h - 1) * (long)dst_wrap;
		dst_line_add -= dst_wrap * 2;
	}

	/* Try acceleration */
	{
		struct RenderInfo srcri;
		struct RenderInfo dstri;

		srcri.Memory = (uae_u8*)src_addr;
		srcri.BytesPerRow = src_wrap;
		srcri.RGBFormat = my_RGBFormat;

		dstri.Memory = (uae_u8*)dst_addr;
		dstri.BytesPerRow = dst_wrap;
		dstri.RGBFormat = my_RGBFormat;

		if (uaelib_picasso_BlitRectNoMaskComplete(&my_BoardInfo, &srcri, &dstri, src_x, src_y, dst_x, dst_y, w, h, uae_blit_opcode[operation], my_RGBFormat))
			return 1;
	}

	src_addr += src_pos / PIXEL_SIZE;
	dst_addr += dst_pos / PIXEL_SIZE;
	src_line_add /= PIXEL_SIZE;		/* Change into pixel count */
	dst_line_add /= PIXEL_SIZE;

	if ((src_y == dst_y) && (src_x < dst_x)) {
		src_addr += w;		/* To take backward copy into account */
		dst_addr += w;
		src_line_add += 2 * w;
		dst_line_add += 2 * w;
		switch(operation) {
		case 3:
			pan_backwards_copy(src_addr, src_line_add, dst_addr, dst_line_add, w, h);
			break;
		case 7:
			pan_backwards_or(src_addr, src_line_add, dst_addr, dst_line_add, w, h);
			break;
		default:
			pan_backwards(src_addr, src_line_add, dst_addr, dst_line_add, w, h, operation);
			break;
		}
	} else {
		switch(operation) {
		case 3:
			blit_copy(src_addr, src_line_add, dst_addr, dst_line_add, w, h);
			break;
		case 7:
			blit_or(src_addr, src_line_add, dst_addr, dst_line_add, w, h);
			break;
		default:
			uae_blit(src_addr, src_line_add, dst_addr, dst_line_add, w, h, operation);
			break;
		}
	}

	return 1;	/* Return as completed */
}
