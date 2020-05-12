/*
 * uae_scr.c - Pixel set/get functions
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
#include "relocate.h"
#include "uaegfx.h"
	

/* destination MFDB (odd address marks table operation)
 * x or table address
 * y or table length (high) and type (0 - coordinates)
 */
long CDECL c_write_pixel(Virtual *vwk, MFDB *dst, long x, long y, long colour)
{
	Workstation *wk;
	long offset;
	
	KDEBUG(("c_write_pixel %ld,%ld\n", x, y));

	if ((long)vwk & 1)
		return 0;

	wk = vwk->real_address;
	if (!dst || !dst->address || (dst->address == wk->screen.mfdb.address)) {
		offset = wk->screen.wrap * y + x * sizeof(short);
		*(short *)((long)wk->screen.mfdb.address + offset) = colour;
	} else {
		offset = (dst->wdwidth * 2 * dst->bitplanes) * y + x * sizeof(short);
		*(short *)((long)dst->address + offset) = colour;
	}
	
	return 1;
}


long CDECL c_read_pixel(Virtual *vwk, MFDB *src, long x, long y)
{
	Workstation *wk;
	long offset;
	unsigned long colour;
	
	wk = vwk->real_address;
	if (!src || !src->address || (src->address == wk->screen.mfdb.address)) {
		offset = wk->screen.wrap * y + x * sizeof(short);
			colour = *(unsigned short *)((long)wk->screen.mfdb.address + offset);
	} else {
		offset = (src->wdwidth * 2 * src->bitplanes) * y + x * sizeof(short);
		colour = *(unsigned short *)((long)src->address + offset);
	}
	
	return colour;
}
