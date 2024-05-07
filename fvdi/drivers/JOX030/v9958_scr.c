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

#include <stdint.h>
#include "utility.h"

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
int getPixelValueFromMFDB(MFDB* font, long x, long y) {
    int pixelValue =0;
    int plane = 0;
    for(plane = 0;plane < font->bitplanes;plane++){
    int realY;
    
    //int realY = y +(plane*font->height);
    // Calculate the index of the short containing the pixel
    long shortIndex = (y * (font->wdwidth)) + (x / 16)+(plane*font->wdwidth*font->height);

    // Extract the short containing the pixel
    unsigned short pixelShort = font->address[shortIndex];

    // Calculate the bit position within the short for the pixel
    int bitPosition = (x % 16);

    // Extract the pixel value from the short using a bitmask
    pixelValue |= ((pixelShort >> (15 - bitPosition)) & 0x1)<<plane;
	
    }
    return pixelValue;
}
/* destination MFDB (odd address marks table operation)
 * x or table address
 * y or table length (high) and type (0 - coordinates)
 */


long CDECL
c_write_pixel(Virtual *vwk, MFDB *dst, long x, long y, long colour)
{
	Workstation *wk;
	long offset;
        uint16_t col;
	
	if ((long)vwk & 1)
		return 0;
	
	
	volatile unsigned char* vramBase = (volatile unsigned char*) 0xc0800000;

	
	uint16_t whichByte = x/8;
	vramBase += y*100 + whichByte;
	unsigned char whichPixel = 7-(x%8);


	wk = vwk->real_address;
	if (!dst || !dst->address || (dst->address == wk->screen.mfdb.address)) {
	        //put pixel on the screen
		unsigned char oldPixels = *vramBase;

// Create a mask to isolate the specific pixel within the byte
		unsigned char mask = 1 << whichPixel;

// Clear the specific pixel in the old pixel value
	oldPixels &= ~mask;

// Set the specific pixel to the new color value
	oldPixels |= (colour << whichPixel) & mask;

// Write the modified pixel value back to VRAM
	*vramBase = oldPixels;
	        
	        //v99x8_pset(x,y,colour,V99X8_ARGUMENT_MXD_VRAM,V99X8_LOGICAL_OPERATION_IMP);
	       	
	} else {
		setPixelValueInMFDB(dst,x,y,colour);
		//supposed to put pixel in a buffer, but Im putting it in VRAM offscreen for performance;
		//v99x8_pset(x,y+300,colour,V99X8_ARGUMENT_MXD_VRAM,V99X8_LOGICAL_OPERATION_IMP);
	
		
	}

	return 1;
}


long CDECL
c_read_pixel(Virtual *vwk, MFDB *src, long x, long y)
{
	Workstation *wk;
	long offset;
	unsigned long colour;
	//v99x8_mode_r1_write(V99X8_MODE_R1_BL);
	volatile unsigned char* vramBase = (volatile unsigned char*) 0xc0800000;

	uint16_t whichByte = x/8;
	vramBase += y*100 + whichByte;
	unsigned char whichPixel = 7-(x%8);
	wk = vwk->real_address;
	if (!src || !src->address || (src->address == wk->screen.mfdb.address)) {
	       //colour = v99x8_point(x,y,V99X8_ARGUMENT_MXD_VRAM,V99X8_LOGICAL_OPERATION_IMP);
		unsigned char mask = 1;
		mask = (mask<<whichPixel);
		uint8_t pixel = *vramBase;
		pixel &= mask;
		colour = (pixel >> whichPixel);
	} else {
		//colour = v99x8_point(x,y+300,V99X8_ARGUMENT_MXD_VRAM,V99X8_LOGICAL_OPERATION_IMP);
		colour = getPixelValueFromMFDB(src, x, y);
	}
	//v99x8_mode_r1_write(V99X8_MODE_R1_BL);
	return colour;
}
