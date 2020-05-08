/*
 * uae_pal.c - Palette functions
 * This is part of the WinUAE RTG driver for fVDI
 * Derived from Johan Klockars's example in ../16_bit/16b_pal.c
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
#include "../bitplane/bitplane.h"
#include "relocate.h"
#include "uaegfx.h"

#define NOVA 0		/* 1 - byte swap 16 bit colour value (NOVA etc) */

#define red_bits   5	/* 5 for all normal 16 bit hardware */
#define green_bits 6	/* 6 for Falcon TC and NOVA 16 bit, 5 for NOVA 15 bit */
			/* (I think 15 bit Falcon TC disregards the green LSB) */
#define blue_bits  5	/* 5 for all normal 16 bit hardware */


long CDECL c_get_colour(Virtual *vwk, long colour)
{
	Colour *local_palette, *global_palette;
	Colour *fore_pal, *back_pal;
    unsigned short foreground, background;
    unsigned short *realp;

	KDEBUG(("c_get_colour colour=%ld\n", colour));

	local_palette = vwk->palette;
	if (local_palette && !((long)local_palette & 1))	/* Complete local palette? */
		fore_pal = back_pal = local_palette;
	else {						/* Global or only negative local */
		local_palette = (Colour *)((long)local_palette & 0xfffffffeL);
		global_palette = vwk->real_address->screen.palette.colours;
		if (local_palette && ((short)colour < 0))
			fore_pal = local_palette;
		else
			fore_pal = global_palette;
		if (local_palette && ((colour >> 16) < 0))
			back_pal = local_palette;
		else
			back_pal = global_palette;
	}

    realp = (unsigned short *)&fore_pal[(short)colour].real;
	foreground = *realp;
	realp = (unsigned short *)&back_pal[colour >> 16].real;
	background = *realp;
	return ((unsigned long)background << 16) | (unsigned long)foreground;
}


void CDECL
c_set_colours(Virtual *vwk, long start, long entries, unsigned short *requested, Colour palette[])
{
	unsigned short colour;
	unsigned short component;
	unsigned long tc_word;
	int i;
    short *realp;
	
	KDEBUG(("c_set_colours start=%ld entries=%ld\n", start, entries));

	if ((long)requested & 1) {			/* New entries? */
		requested = (unsigned short *)((long)requested & 0xfffffffeL);
		for(i = 0; i < entries; i++) {
			requested++;				/* First word is reserved */
			component = *requested++ >> 8;
			palette[start + i].vdi.red = (component * 1000L) / 255;
			palette[start + i].hw.red = component;	/* Not at all correct */
			colour = component >> (16 - red_bits);	/* (component + (1 << (14 - red_bits))) */
			tc_word = colour << green_bits;
			component = *requested++ >> 8;
			palette[start + i].vdi.green = (component * 1000L) / 255;
			palette[start + i].hw.green = component;	/* Not at all correct */
			colour = component >> (16 - green_bits);	/* (component + (1 << (14 - green_bits))) */
			tc_word |= colour;
			tc_word <<= blue_bits;
			component = *requested++ >> 8;
			palette[start + i].vdi.blue = (component * 1000L) / 255;
			palette[start + i].hw.blue = component;	/* Not at all correct */
			colour = component >> (16 - blue_bits);		/* (component + (1 << (14 - blue_bits))) */
			tc_word |= colour;
#if NOVA
			tc_word = ((tc_word & 0x000000ff) << 24) | ((tc_word & 0x0000ff00) <<  8) |
			          ((tc_word & 0x00ff0000) >>  8) | ((tc_word & 0xff000000) >> 24);
#endif
			realp = (short *)&palette[start + i].real;
			*realp = tc_word;
		}
	} else {
		for(i = 0; i < entries; i++) {
			component = *requested++;
			palette[start + i].vdi.red = component;
			palette[start + i].hw.red = component;	/* Not at all correct */
			colour = (component * ((1L << red_bits) - 1) + 500L) / 1000;
			tc_word = colour << green_bits;
			component = *requested++;
			palette[start + i].vdi.green = component;
			palette[start + i].hw.green = component;	/* Not at all correct */
			colour = (component * ((1L << green_bits) - 1) + 500L) / 1000;
			tc_word |= colour;			/* Was (colour + colour) */
			tc_word <<= blue_bits;
			component = *requested++;
			palette[start + i].vdi.blue = component;
			palette[start + i].hw.blue = component;	/* Not at all correct */
			colour = (component * ((1L << blue_bits) - 1) + 500L) / 1000;
			tc_word |= colour;
#if NOVA
			tc_word = (tc_word << 8) | (tc_word >> 8);
#endif
			realp = (short *)&palette[start + i].real;
			*realp = tc_word;
		}
	}
}
