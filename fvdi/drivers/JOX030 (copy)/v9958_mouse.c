/*
 * uae_mous.c - Mouse cursor functions
 * This is part of the WinUAE RTG driver for fVDI
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
#include <stdint.h>

#include "relocate.h"



uint8_t m_temp[128];

/* External data and functions */
extern Driver *me;
extern long CDECL c_xpand_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour);
extern long CDECL c_blit_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation);



extern Access *access;



void
putPix(long x, long y, long colour)
{
	
	volatile unsigned char* vramBase = (volatile unsigned char*) 0xc0800000;
	if(colour == 1) colour = 0;
	else if(colour ==0) colour =1;
	else return 1;
	

	
	uint16_t whichByte = x/8;
	vramBase += y*100 + whichByte;
	unsigned char whichPixel = 7-(x%8);


		        //put pixel on the screen
		unsigned char oldPixels = *vramBase;
		unsigned char mask = 1;
		mask = ~(mask<<whichPixel);
		oldPixels &= mask;
		oldPixels |= colour<<whichPixel;
		*vramBase = oldPixels;
	        
	        //v99x8_pset(x,y,colour,V99X8_ARGUMENT_MXD_VRAM,V99X8_LOGICAL_OPERATION_IMP);
	       	
	

	return ;
}
#define GPU_REG_DATA 0xc080ffff
#define GPU_REG_REGNO 0xc080fffe
#define GPU_REG_WPROT 0xc080fffd
void gpuWriteReg2(unsigned char reg, unsigned char data){
	*((volatile unsigned char*)GPU_REG_WPROT) = 0xaf;
	*((volatile unsigned char*)GPU_REG_REGNO) = reg;
	*((volatile unsigned char*)GPU_REG_DATA) = data;
}
void setCurSpritePos( uint16_t dx, uint16_t dy){
	gpuWriteReg2(9,dx>>8);
	gpuWriteReg2(8,dx&0xff);
	gpuWriteReg2(7,dy>>8);
	gpuWriteReg2(6,dy&0xff);
}

long CDECL
c_mouse_draw(Workstation *wk, long x, long y, Mouse *mouse)
{
	
	if ((long)mouse > 7) /* Set new mouse cursor shape */
	{
		//cursor should be updated now, but for now I have only a single hardcoded cursor

		return 0;
	}
	else
	{
				
		
		switch ((long)mouse)
		{
		case 0: /* Move visible */
		case 4: /* Move visible forced (wk_mouse_forced) */
			//show_mouse( (short)x, (short)y);
			setCurSpritePos(x,y);
			break;

		case 1: /* Move hidden */
		case 5: /* Move hidden forced (wk_mouse_forced) */
			//show_mouse(vwk, (short)x, (short)y);
			//hide_mouse();
			setCurSpritePos(x,y);

			break;

		case 2: /* Hide */
			//hide_mouse();
			setCurSpritePos(x,y);

			break;

		case 3: /* Show */
			
			setCurSpritePos(x,y);
//show_mouse( (short)x, (short)y);
			break;
		}

		return 0;
	}

	return 0;
}
