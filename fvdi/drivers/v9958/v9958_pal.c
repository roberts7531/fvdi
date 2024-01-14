/* 
 * 16 bit palette handling routines, by Johan Klockars.
 *
 * $Id: 16b_pal.c,v 1.4 2005-08-04 10:17:10 johan Exp $
 *
 * This file is an example of how to write an
 * fVDI device driver routine in C.
 *
 * You are encouraged to use this file as a starting point
 * for other accelerated features, or even for supporting
 * other graphics modes. This file is therefore put in the
 * public domain. It's not copyrighted or under any sort
 * of license.
 */

#include "v99x8.h"
//#include "fvdi.h"
#include "relocate.h"
#include <stdint.h>

#define NOVA 0		/* 1 - byte swap 16 bit colour value (NOVA etc) */

#define red_bits   5	/* 5 for all normal 16 bit hardware */
#define green_bits 5	/* 6 for Falcon TC and NOVA 16 bit, 5 for NOVA 15 bit */
			/* (I think 15 bit Falcon TC disregards the green LSB) */
#define blue_bits  5	/* 5 for all normal 16 bit hardware */

#define PIXEL		unsigned char


static long tos_colours[] = {0, 255, 1, 2, 4, 6, 3, 5, 7, 8, 9, 10, 12, 14, 11, 13};
extern Access *access;


long CDECL
c_get_colour(Virtual *vwk, long colour)
{
	// There was much more code here before, no idea what it did, it works like this so it is alright I hope
	return colour&0xf;

}


uint8_t indices[16];
void CDECL c_set_colour(Virtual *vwk, long paletteIndex, long red, long green, long blue)
{
	//this code sets up a pallete index with the specified value
	uint8_t shortInd = (uint8_t) paletteIndex&0xf;
	if(indices[shortInd]==1){
		//but after setting it up, it reset all the colors to 0 for some reason, so i made it impossible for it to change a color after it has been set once
		return;
	}
	uint8_t redBlue = blue&0x7;
	redBlue += (red&0x7)<<4;
	uint8_t greenR = green&0x7;
	uint16_t coldat = redBlue + (greenR<<8); 
	v99x8_palette_register_write(shortInd, coldat);
	indices[shortInd]=1;
   
}

void CDECL
c_set_colours(Virtual *vwk, long start, long entries, unsigned short *requested, Colour palette[])
{
	unsigned long colour;
	unsigned short component;
	unsigned long tc_word;
	int i;
	// I have no idea what all of this does, i just copied it from another driver, crashes without it
	if ((long)requested & 1) {			/* New entries? */
		requested = (short *)((long)requested & 0xfffffffeL);
		for(i = 0; i < entries; i++) {
			requested++;				/* First word is reserved */
			component = *requested++;
			palette[start + i].vdi.red = component;
			palette[start + i].hw.red = component;	/* Not at all correct */
			colour = component >> (16 - red_bits);	/* (component + (1 << (14 - red_bits))) */
			tc_word = colour << green_bits;
			component = *requested++;
			palette[start + i].vdi.green = component;
			palette[start + i].hw.green = component;	/* Not at all correct */
			colour = component >> (16 - green_bits);	/* (component + (1 << (14 - green_bits))) */
			tc_word |= colour;
			tc_word <<= blue_bits;
			component = *requested++;
			palette[start + i].vdi.blue = component;
			palette[start + i].hw.blue = component;	/* Not at all correct */
			colour = component >> (16 - blue_bits);		/* (component + (1 << (14 - blue_bits))) */
			tc_word |= colour;
#if NOVA
			switch (sizeof(PIXEL)) {
			case 2:
				tc_word = ((tc_word & 0x000000ffL) << 8) | ((tc_word & 0x0000ff00L) >>  8);
				break;
			default:
				tc_word = ((tc_word & 0x000000ffL) << 24) | ((tc_word & 0x0000ff00L) <<  8) |
				          ((tc_word & 0x00ff0000L) >>  8) | ((tc_word & 0xff000000L) >> 24);
				break;
			}
#endif
			c_set_colour(	vwk,
					start + i,
					palette[start + i].vdi.red,
					palette[start + i].vdi.green,
		       			palette[start + i].vdi.blue);

			*(PIXEL *)&palette[start + i].real = (PIXEL)tc_word;
		}
	} else {
		for(i = 0; i < entries; i++) {
			component = *requested++;
			palette[start + i].vdi.red = component;
#if 0
			palette[start + i].hw.red = component;	/* Not at all correct */
#endif
			colour = (component * ((1L << red_bits) - 1) + 500L) / 1000;
			palette[start + i].hw.red = (colour + (1L << (red_bits - 1))) / ((1L << red_bits) - 0);
			palette[start + i].hw.red = colour;
			tc_word = colour << green_bits;
			component = *requested++;
			palette[start + i].vdi.green = component;
#if 0
			palette[start + i].hw.green = component;	/* Not at all correct */
#endif
			colour = (component * ((1L << green_bits) - 1) + 500L) / 1000;
			palette[start + i].hw.green = (colour + (1L << (green_bits - 1))) / ((1L << green_bits) - 0);
			palette[start + i].hw.green = colour;
			tc_word |= colour;			/* Was (colour + colour) */
			tc_word <<= blue_bits;
			component = *requested++;
			palette[start + i].vdi.blue = component;
#if 0
			palette[start + i].hw.blue = component;	/* Not at all correct */
#endif
			colour = (component * ((1L << blue_bits) - 1) + 500L) / 1000;
			palette[start + i].hw.blue = (colour + (1L << (blue_bits - 1))) / ((1L << blue_bits) - 0);
			palette[start + i].hw.blue = colour;
			tc_word |= colour;
#if NOVA
			switch (sizeof(PIXEL)) {
			case 2:
				tc_word = ((tc_word & 0x000000ffL) << 8) | ((tc_word & 0x0000ff00L) >>  8);
				break;
			default:
				tc_word = ((tc_word & 0x000000ffL) << 24) | ((tc_word & 0x0000ff00L) <<  8) |
				          ((tc_word & 0x00ff0000L) >>  8) | ((tc_word & 0xff000000L) >> 24);
				break;
			}
#endif
			c_set_colour(	vwk,
					start + i,
					palette[start + i].hw.red,
					palette[start + i].hw.green,
		       			palette[start + i].hw.blue);

			*(PIXEL *)&palette[start + i].real = (PIXEL)tc_word;
		}
	}
}
