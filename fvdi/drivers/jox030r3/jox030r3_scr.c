/* 
 * 16 bit pixel set/get routines, by Johan Klockars.
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

#define BOTH	/* Write in both FastRAM and on screen */

#include "fvdi.h"
#include "relocate.h"
#include "jox030r3.h"
#include <stdint.h>
#include "utility.h"
#define PIXEL       char
#define PIXEL_SIZE  sizeof(PIXEL)
extern Access *access;

 void setPixelValueInMFDB(MFDB* font, long x, long y, long pixelValue) {
    // Iterate through each bitplane
    int plane;
    for (plane = 0; plane < font->bitplanes; plane++) {
        // Calculate the index of the short containing the pixel data for the current bitplane
        long shortIndex = (y * font->wdwidth) + (x / 16) + (plane*font->wdwidth*font->height);

        // Calculate the bit position within the short for the pixel
        int bitPosition = 15 - (x % 16);

        // Calculate the mask to set or clear the pixel bit
        unsigned short mask = 0x1 << bitPosition;

        // Set or clear the pixel bit based on the pixel value
        if (pixelValue & (1 << plane)) {
            // Set the pixel bit
            font->address[shortIndex] |= mask;
        } else {
            // Clear the pixel bit
            font->address[shortIndex] &= ~mask;
        }
    }
}

uint8_t colorLUT[16] = {0,2,3,6,4,7,5,8,9,10,11,14,12,15,13,1};






/* destination MFDB (odd address marks table operation)
 * x or table address
 * y or table length (high) and type (0 - coordinates)
 */

//volatile uint16_t* vramBase = (uint16_t*)0xC0800000;

long CDECL
c_write_pixel(Virtual *vwk, MFDB *dst, long x, long y, long colour)
{
	
	Workstation *wk;

	if ((long)vwk & 1)
		return 0;

	wk = vwk->real_address;
	if (!dst || !dst->address || (dst->address == wk->screen.mfdb.address)) {
		*(vram + y*1024+x) = (uint8_t)colour;

		
	} else {
		
		long offset = (dst->wdwidth * 2 * dst->bitplanes) * y + x * PIXEL_SIZE;
        *(PIXEL *)((long)dst->address + offset) = colour;
	}

	return 1;
}


long CDECL
c_read_pixel(Virtual *vwk, MFDB *src, long x, long y)
{
	Workstation *wk;
	unsigned long colour;

	
	wk = vwk->real_address;
	if (!src || !src->address || (src->address == wk->screen.mfdb.address)) {

		colour = *(vram + y*1024+x);
	} else {
		//colour = getPixelValueFromMFDB(src,x,y);
		long offset = (src->wdwidth * 2 * src->bitplanes) * y + x * PIXEL_SIZE;
        	colour = *(unsigned PIXEL *)((long)src->address + offset);
		colour = colorLUT[colour];
	}
	return colour;
}
