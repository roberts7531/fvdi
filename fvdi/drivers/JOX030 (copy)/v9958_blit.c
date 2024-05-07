/* 
 * A 16 bit graphics blit routine, by Johan Klockars.
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

#if 1
#define FAST		/* Write in FastRAM buffer */
#define BOTH		/* Write in both FastRAM and on screen */
#else
#undef FAST
#undef BOTH
#endif

#include "fvdi.h"
#include <stdint.h>
#include "v99x8.h"
//#include "v9990.h"

#define PIXEL		short
#define PIXEL_SIZE	sizeof(PIXEL)
void putPx(long x, long y, uint8_t col){
	volatile unsigned char* vramBase = (volatile unsigned char*) 0xc0800000;
	uint16_t whichByte = x/8;
	vramBase += y*100 + whichByte;
	unsigned char whichPixel = 7-(x%8);
	unsigned char oldPixels = *vramBase;
	unsigned char mask = 1 << whichPixel;
	oldPixels &= ~mask;
	oldPixels |= (col << whichPixel) & mask;
	*vramBase = oldPixels;

}
uint8_t getPx(long x, long y){
		volatile unsigned char* vramBase = (volatile unsigned char*) 0xc0800000;
		uint16_t whichByte = x/8;
		vramBase += y*100 + whichByte;
		unsigned char whichPixel = 7-(x%8);
		unsigned char mask = 1;
		mask = (mask<<whichPixel);
		uint8_t pixel = *vramBase;
		pixel &= mask;
			return (pixel >> whichPixel);
		
}

long CDECL
c_blit_area(Virtual *vwk, MFDB *src, long src_x, long src_y,
            MFDB *dst, long dst_x, long dst_y,
            long w, long h, long operation)
{
	Workstation *wk;
	PIXEL *src_addr, *dst_addr, *dst_addr_fast;
        uint8_t *addrcounter;

	int src_wrap, dst_wrap;
	int src_line_add, dst_line_add;
	unsigned long src_pos, dst_pos;
	int from_screen, to_screen;

        
        uint8_t direction;
        uint16_t pixelcount,count;
        uint8_t pixl;
        uint8_t *srcaddr, *dstaddr;

	wk = vwk->real_address;

	from_screen = 0;
	if (!src || !src->address || (src->address == wk->screen.mfdb.address)) {		/* From screen? */
		src_wrap = wk->screen.wrap;
		if (!(src_addr = wk->screen.shadow.address))
			src_addr = wk->screen.mfdb.address;
		from_screen = 1;
	} else {
		src_wrap = (long)src->wdwidth * 2 * src->bitplanes;
		src_addr = src->address;
		srcaddr = (uint8_t *)src->address;
	}
	src_pos = (short)src_y * (long)src_wrap + src_x * PIXEL_SIZE;
	//src_line_add = src_wrap - w * PIXEL_SIZE;
	src_line_add = (src_wrap - w) >> 1;// * PIXEL_SIZE;

	to_screen = 0;
	if (!dst || !dst->address || (dst->address == wk->screen.mfdb.address)) {		/* To screen? */
		dst_wrap = wk->screen.wrap;
		dst_addr = wk->screen.mfdb.address;
		to_screen = 1;
	} else {
		dst_wrap = (long)dst->wdwidth * 2 * dst->bitplanes;
		dst_addr = dst->address;
		dstaddr = (uint8_t *)dst->address;
	}
	dst_pos = (short)dst_y * (long)dst_wrap + dst_x * PIXEL_SIZE;
	//dst_line_add = dst_wrap - w * PIXEL_SIZE;
	dst_line_add = (dst_wrap - w) >> 1; // * PIXEL_SIZE;
        if(from_screen && to_screen) /* From screen to screen (VRAM to VRAM blits only) */
        {
                
                
		 int xStep = (dst_x < src_x) ? -1 : 1;
    	int yStep = (dst_y < src_y) ? -1 : 1;

    // Copy framebuffer data
	int x,y;
    for (y = 0; y < h; y++, src_y += yStep, dst_y += yStep) {
        for (x = 0; x < w; x++, src_x += xStep, dst_x += xStep) {
            // Read pixel color from source
            uint8_t color = getPx(src_x, src_y);
            // Write pixel color to destination
            putPx(dst_x, dst_y, color);
        }
        // Reset source and destination x coordinates after each row
        src_x -= xStep * w;
        dst_x -= xStep * w;
    }

        } else if(!from_screen && to_screen) /* From memory to screen */
        {

		return 0;

        } else if(from_screen && !to_screen) /* From screen to memory */
        {
        
		return 0;
        	//v99x8_lmmm(src_x, src_y, dst_x, dst_y+300, w, h, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, lo);
                //v99x8_lmcm(src_x, src_y, w, h, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, dstaddr+dst_pos);
                
           
        } else
        {
                return 0;
        }

	return 1;	/* Return as completed */
}

