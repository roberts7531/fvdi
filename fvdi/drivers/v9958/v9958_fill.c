/* 
 * A 16 bit graphics fill routine, by Johan Klockars.
 *
 * $Id: 16b_fill.c,v 1.2 2002-07-10 22:13:39 johan Exp $
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



#include "fvdi.h"
#include "v99x8.h"
#include "relocate.h"

extern void CDECL c_get_colour(Virtual *vwk, long colour, short *foreground, short *background);
extern long CDECL fallback_fill(Virtual *vwk, long x, long y, long w, long h, short *pattern, long colour, long mode, long interior_style);
extern Access *access;
uint8_t shouldUsePattern(short* pattern){
	int patline;
	for(patline=0;patline<16;patline++)
        {
            if((pattern[patline] & 0xffff) != 0xffff)
            {
                return 1;
            }
        }
        return 0;
}
enum v99x8_logical_operation getModeEnum(long mode){
	switch (mode) {
	case 1:				/* Replace */
                return V99X8_LOGICAL_OPERATION_IMP;
		break;
	case 2:				/* Transparent */
                return V99X8_LOGICAL_OPERATION_OR;
		break;
	case 3:				/* XOR */
                return V99X8_LOGICAL_OPERATION_XOR;
		break;
	case 4:				/* Reverse transparent */
                return V99X8_LOGICAL_OPERATION_AND;
		break;
	}
	return V99X8_LOGICAL_OPERATION_IMP;
}
void loadPatVram(short*pattern,uint8_t color){
	color &=0xf;
	uint8_t i,j;
	uint8_t pixel[16][8];
	for(i=0;i<16;i++){
		uint16_t patLine = (uint16_t) pattern[i];
		for(j=0;j<8;j++){
			uint8_t highPixInd = (j*2)+1;//1
			uint8_t lowPixInd = (j*2); //0
			uint8_t isHighSet = (patLine>>highPixInd)&1; //0101 >> 1 = 0010 & 1 = 0
			uint8_t isLowSet  = (patLine>>lowPixInd)&1;  //0101 >> 0 = 0101 & 1 = 1
			pixel[i][j] =0;
			if(isHighSet){
				pixel[i][j] |= (color<<4);
			}
			if(isLowSet){
				pixel[i][j] |= color;
			}
		}
	}


	v99x8_hmmc(0,256,16,16, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, pixel);
	v99x8_hmmc(16,256,16,16, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, pixel);
	v99x8_hmmc(0,256+16,16,16, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, pixel);
	v99x8_hmmc(16,256+16,16,16, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, pixel);

}
void fillPat(long x,long y,long w,long h,enum v99x8_logical_operation lo){
    uint8_t isOddX=0;
    uint8_t isOddY=0;
    if(x%2!=0){
    	isOddX=1;
    }
    if(y%2!=0){
    	isOddY=1;
    }
    
    int fullBlocksX = w/16;
    int fullBlocksY = h/16;
    int remainderX = w%16;
    int remainderY = h%16;
    uint8_t dH,dW;
    int i,j;
    if(fullBlocksX==0&&fullBlocksY==0){
    	v99x8_lmmm(0+isOddX, 256+isOddY, x,y , w, h, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT,lo);
    
    
    }
    if(fullBlocksX==0){
    	for(j=0;j<fullBlocksY;j++){
    		dH=0;
    		if(j==(fullBlocksY-1)){
    		dH = remainderY;
    		}
    		v99x8_lmmm(0+isOddX, 256+isOddY, x,y+(16*j) , remainderX, 16+dH, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT,lo);
    	}
    
    
    }
    if(fullBlocksY==0){
    	for(i=0;i<fullBlocksX;i++){
    		dW=0;
    		if(i==(fullBlocksX-1)){
    		dW = remainderX;
    		}
    		v99x8_lmmm(0+isOddX, 256+isOddY, x+(16*i),y , 16+dW, remainderY, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT,lo);
    	}
    
    
    }
    
    
    int rX,rY;
    
    
    for(i=0;i<fullBlocksX;i++){
    	for(j=0;j<fullBlocksY;j++){
    		rX = x+(16*i);
    		rY = y+(16*j);
    		dH=0;
    		dW=0;
    		if(i==(fullBlocksX-1)){
    		dW = remainderX;
    		}
    		if(j==(fullBlocksY-1)){
    		dH = remainderY;
    		}
    		v99x8_lmmm(0+isOddX, 256+isOddY, rX,rY , 16+dW, 16+dH, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT,lo);
    		
    	}
    }
    
    
    
}

long CDECL c_fill_area(Virtual *vwk, long x, long y, long w, long h,
                       short *pattern, long colour, long mode, long interior_style)
{


	short  col;










        col = (colour&0x0f)+((colour&0x0f)<<4);
        col += (col << 8);


	if ((long)vwk & 1) {
		if ((y & 0xffff) != 0)
			return -1;		/* Don't know about this kind of table operation */

		h = (y >> 16) & 0xffff;
		vwk = (Virtual *)((long)vwk - 1);
		return -1;			/* Don't know about anything yet */
	}

      

	uint8_t usePattern = shouldUsePattern(pattern);
	if(!usePattern){
		v99x8_lmmv(x, y, w, h, col, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, getModeEnum(mode));

		return 1;
	}
	// first we load the pattern into VRAM
	//then we copy to the fill area
	loadPatVram(pattern,col);
	fillPat(x,y,w,h,getModeEnum(mode));

	return 1;
}
