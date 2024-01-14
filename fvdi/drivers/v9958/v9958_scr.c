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
#include "v99x8.h"
#include <stdint.h>
#include "utility.h"

extern Access *access;

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

	v99x8_mode_r1_write(V99X8_MODE_R1_BL);
	wk = vwk->real_address;
	if (!dst || !dst->address || (dst->address == wk->screen.mfdb.address)) {
	        //put pixel on the screen
	        v99x8_pset(x,y,colour,V99X8_ARGUMENT_MXD_VRAM,V99X8_LOGICAL_OPERATION_IMP);
	       	
	} else {
		//supposed to put pixel in a buffer, but Im putting it in VRAM offscreen for performance;
		v99x8_pset(x,y+300,colour,V99X8_ARGUMENT_MXD_VRAM,V99X8_LOGICAL_OPERATION_IMP);
	
		
	}
	v99x8_mode_r1_write(V99X8_MODE_R1_BL);
	return 1;
}


long CDECL
c_read_pixel(Virtual *vwk, MFDB *src, long x, long y)
{
	Workstation *wk;
	long offset;
	unsigned long colour;
	v99x8_mode_r1_write(V99X8_MODE_R1_BL);
	wk = vwk->real_address;
	if (!src || !src->address || (src->address == wk->screen.mfdb.address)) {
	       colour = v99x8_point(x,y,V99X8_ARGUMENT_MXD_VRAM,V99X8_LOGICAL_OPERATION_IMP);
		
	} else {
		colour = v99x8_point(x,y+300,V99X8_ARGUMENT_MXD_VRAM,V99X8_LOGICAL_OPERATION_IMP);
		
	}
	v99x8_mode_r1_write(V99X8_MODE_R1_BL);
	return colour;
}
