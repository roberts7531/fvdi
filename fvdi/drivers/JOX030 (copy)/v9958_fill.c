
#include "fvdi.h"
#include "relocate.h"
#include <stdint.h>

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
void putPix2(long x, long y, uint8_t col){
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
uint8_t getPix(long x, long y){
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
uint8_t getPatPix(long x0,long y0, long x, long y, short* pattern,long colour)
{
	uint16_t line = pattern[(y)%16];
	if(colour)
		return  line>>15-((x)%16)&1;
	else
		return  !(line>>15-((x)%16)&1);
}
uint16_t rightRotate(uint16_t n, uint8_t d)
{
    
    return (n >> d) | (n << (16 - d));
}
uint8_t getPatByte(long x0, long y0, long byteX, long y, short* pattern, long colour) {

    // Calculate the effective start position within the pattern
    long effectiveX0 = (byteX * 8);// - x0;
    long effectiveY0 = y - y0;
	uint16_t line = pattern[y % 16];
	uint8_t byte;
	line = rightRotate(line,effectiveX0);
	if(effectiveX0%2==0) byte= line >>8;
	else byte = line & 0xff;
    // Ensure effectiveX0 is within the range 0 to 15
   
   
    if (!colour)
        byte = ~byte;

    return byte;
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
	if(mode!=1) return 0;
	/*
	int xPos, yPos;
	for(yPos=y;yPos<y+h;yPos++){
		for(xPos=x;xPos<x+w;xPos++){
			uint8_t patPix = getPatPix(x,y,xPos,yPos,pattern,col);
			putPix2(xPos,yPos,patPix);
		}
	}
	return 1;*/


	uint8_t shouldUsePattern1 = shouldUsePattern(pattern);
		//if(!shouldUsePattern1) return 0;
/*
      char buffer[50];
	  access->funcs.puts("Mode:");
	access->funcs.ltoa(buffer,mode,10);
	access->funcs.puts(buffer);
	access->funcs.puts("x:");
	access->funcs.ltoa(buffer,x,10);
	access->funcs.puts(buffer);
	access->funcs.puts("y:");
	access->funcs.ltoa(buffer,y,10);
	access->funcs.puts(buffer);
	access->funcs.puts("w:");
	access->funcs.ltoa(buffer,w,10);
	access->funcs.puts(buffer);
	access->funcs.puts("h:");
	access->funcs.ltoa(buffer,h,10);
	access->funcs.puts(buffer);
	access->funcs.puts("\r\n");
*/
	volatile unsigned char* vramBase = (volatile unsigned char*) 0xc0800000;

	uint8_t startTrimmed = 0 ;
	uint8_t endTrimmed = 0;
	uint16_t startByte = x/8;
	unsigned char startPixel = 7-(x%8);
	if(startPixel!=0) {
		startByte++;
		startTrimmed = 1;
	}
	uint16_t endByte = (x+w)/8;
	uint16_t endPixel = 7-((x+w)%8);
	if(endPixel!=7){ 
		endTrimmed = 1;
		endByte--;
	}

	uint16_t xp,yp;
	switch(mode){
		
		case 1:
			for(yp=y;yp<y+h;yp++){
				if(startTrimmed){
					for(xp=x;xp<startByte*8;xp++){
						putPix2(xp,yp,getPatPix(x,y,xp,yp,pattern,col));
					}
				}
				for(xp=startByte;xp<endByte;xp++){
					*(vramBase + (yp*100)+xp) = getPatByte(x,y,xp,yp,pattern,col);

				}
				if(endTrimmed){
					for(xp=endByte*8;xp<x+w;xp++){
						putPix2(xp,yp,getPatPix(x,y,xp,yp,pattern,col));
					}
				}
			}	
		return 1;
		case 2:
			return 0;
		case 3: //xor
			for(yp=y;yp<y+h;yp++){
				if(startTrimmed){
					for(xp=x;xp<startByte*8;xp++){
						uint8_t oldPix = getPix(xp,yp);
						if(colour)
							putPix2(xp,yp,oldPix^ 0x00);
						else
							putPix2(xp,yp,oldPix ^ 0x01);
					}
				}
				for(xp=startByte;xp<endByte;xp++){
					uint8_t oldByte = *(vramBase + (yp*100)+xp);
					if(colour)
						*(vramBase + (yp*100)+xp) = oldByte ^ 0x00;
					else
						*(vramBase + (yp*100)+xp) = oldByte ^ 0xff;

				}
			}	
		return 1;
		case 4:
			return 0;
			for(yp=y;yp<y+h;yp++){
				for(xp=startByte;xp<endByte;xp++){
					uint8_t oldByte = *(vramBase + (yp*100)+xp);
					if(colour)
						*(vramBase + (yp*100)+xp) = oldByte & 0xff;
					else
						*(vramBase + (yp*100)+xp) = oldByte & 0x00;

				}
			}
		return 0;
		default:
			for(yp=y;yp<y+h;yp++){
				for(xp=startByte;xp<endByte;xp++){
					uint8_t oldByte = *(vramBase + (yp*100)+xp);
					if(colour)
						*(vramBase + (yp*100)+xp) = pattern[yp%16];
					else
						*(vramBase + (yp*100)+xp) = ~pattern[yp%16];

				}
			}	
		return 1;

	}

	return 0;

}