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
volatile uint16_t* vramBase = (uint16_t*)0xC0800000;

static long tos_colours[] = {0, 255, 1, 2, 4, 6, 3, 5, 7, 8, 9, 10, 12, 14, 11, 13};
extern Access *access;


uint8_t reds[256];
uint8_t greens[256];
uint8_t blues[256];

long CDECL
c_get_colour(Virtual *vwk, long colour)
{
	// There was much more code here before, no idea what it did, it works like this so it is alright I hope
	return colour&0xf;

}

short color_overrides[][7] = {
        { 10, 667,   0,   0, 1000,  562,  562 }, // light red
        { 11,   0, 667,   0,  562, 1000,  562 }, // light green
        { 12,   0,   0, 667,  562,  562, 1000 }, // light blue
        { 13,   0, 667, 667,  562, 1000, 1000 }, // light cyan
        { 14, 667, 667,   0, 1000, 1000,  562 }, // light yellow
        { 15, 667,   0, 667, 1000,  562, 1000 }, // light magenta
};
uint8_t indices[16];
void CDECL set_colour( long paletteIndex, long red, long green, long blue)
{

	//this code sets up a pallete index with the specified value
	uint8_t shortInd = (uint8_t) paletteIndex&0xf;
	if(indices[shortInd]==1){
		//but after setting it up, it reset all the colors to 0 for some reason, so i made it impossible for it to change a color after it has been set once
		//return;
	}
	red = (red << 3) | (red >> 2);
	green = (green << 3) | (green >> 2);
	blue = (blue << 3) | (blue >> 2);

	//uint8_t redBlue = blue&0x7;
	//redBlue += (red&0x7)<<4;
	//uint8_t greenR = green&0x7;
	//uint16_t coldat = redBlue + (greenR<<8); 
	//v99x8_palette_register_write(shortInd, coldat);
	setColor((uint8_t)red,(uint8_t)green,(uint8_t)blue,(uint8_t)paletteIndex);
	indices[shortInd]=1;
   
}
void CDECL c_get_colours(Virtual *vwk, long colour, unsigned long *foreground, unsigned long *background)
{
    Colour *local_palette, *global_palette;
    Colour *fore_pal, *back_pal;
    unsigned short *realp;

    local_palette = vwk->palette;
    if (local_palette && !((long)local_palette & 1))    /* Complete local palette? */
        fore_pal = back_pal = local_palette;
    else {                      /* Global or only negative local */
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
    *foreground = *realp;
    realp = (unsigned short *)&back_pal[colour >> 16].real;
    *background = *realp;
}

void CDECL
c_set_colours(Virtual *UNUSED(vwk), long start, long entries, unsigned short *requested, Colour palette[])
{
    unsigned long colour;
    unsigned short component;
    unsigned long tc_word;
    int i,idx;
    // I have no idea what all of this does, i just copied it from another driver, crashes without it
    if ((long) requested & 1) {                        /* New entries? */
        //access->funcs.puts("c_set_colours: low bit of 'requested' is set.\n");
        requested = (unsigned short *) ((long) requested & 0xfffffffeL);
        for (i = 0; i < entries; i++) {
            requested++;                                /* First word is reserved */
            component = *requested++;
            palette[start + i].vdi.red = component;
            palette[start + i].hw.red = component;        /* Not at all correct */
            colour = component >> (16 - red_bits);        /* (component + (1 << (14 - red_bits))) */
            tc_word = colour << green_bits;
            component = *requested++;
            palette[start + i].vdi.green = component;
            palette[start + i].hw.green = component;        /* Not at all correct */
            colour = component >> (16 - green_bits);        /* (component + (1 << (14 - green_bits))) */
            tc_word |= colour;
            tc_word <<= blue_bits;
            component = *requested++;
            palette[start + i].vdi.blue = component;
            palette[start + i].hw.blue = component;        /* Not at all correct */
            colour = component >> (16 - blue_bits);                /* (component + (1 << (14 - blue_bits))) */
            tc_word |= colour;
            set_colour(start + i,
                         palette[start + i].vdi.red,
                         palette[start + i].vdi.green,
                         palette[start + i].vdi.blue);

            *(PIXEL *) &palette[start + i].real = (PIXEL) tc_word;
        }
    } else {
        //access->funcs.puts("c_set_colours: low bit of 'requested' is not set.\n");
        // In this case, we are being asked to set the hardware palette.
        // For each entry, we have the requested red, green and blue VDI values. A VDI
        // color value ranges from 0 to 1000.

        // As mentioned above, use the TOS 2 light colors instead of the dark ones.
        for ( idx = 0; idx < 6; idx++) {
            short palette_index = color_overrides[idx][0];
            uint16_t match_r = color_overrides[idx][1], match_g = color_overrides[idx][2], match_b = color_overrides[idx][3];
            uint16_t current_r = requested[palette_index * 3];
            uint16_t current_g = requested[palette_index * 3 + 1];
            uint16_t current_b = requested[palette_index * 3 + 2];

            if ((match_r == current_r) && (match_g == current_g) && (match_b == current_b)) {
                requested[palette_index * 3] = color_overrides[idx][4];
                requested[palette_index * 3 + 1] = color_overrides[idx][5];
                requested[palette_index * 3 + 2] = color_overrides[idx][6];
            }
        }

        for (i = 0; i < entries; i++) {
            //PRINTF(("    entry %d: red = %d, green = %d, blue = %d\n", i, requested[0], requested[1], requested[2]));

            component = *requested++;
            palette[start + i].vdi.red = component;
            long temp = (component * ((1L << red_bits) - 1) + 500L) / 1000;
            palette[start + i].hw.red = (short) temp;
            tc_word = palette[start + i].hw.red << green_bits;

            component = *requested++;
            palette[start + i].vdi.green = component;
            temp = (component * ((1L << green_bits) - 1) + 500L) / 1000;
            palette[start + i].hw.green = (short) temp;

            tc_word |= temp;
            tc_word <<= blue_bits;

            component = *requested++;
            palette[start + i].vdi.blue = component;
            temp = (component * ((1L << blue_bits) - 1) + 500L) / 1000;
            palette[start + i].hw.blue = (short) temp;
            tc_word |= temp;

            //PRINTF(("        hw.red = %d, hw.blue = %d, hw.green = %d, truecolor = 0x%08lx\n",
            //    palette[start + i].hw.red, palette[start + i].hw.blue, palette[start + i].hw.green, tc_word));

            set_colour(start + i,
                         palette[start + i].hw.red,
                         palette[start + i].hw.green,
                         palette[start + i].hw.blue);

            *(PIXEL *) &palette[start + i].real = (PIXEL) tc_word;
        }
    }
}