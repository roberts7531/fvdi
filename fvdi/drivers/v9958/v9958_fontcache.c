/* 
 * A 16 bit graphics mono-expand routine, by Johan Klockars.
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
#include <stdint.h>
#include "v99x8.h"
#include "relocate.h"

extern Access *access;

#define PIXEL		short
#define PIXEL_SIZE	sizeof(PIXEL)
void getCachePos(uint8_t ch, uint16_t* x, uint16_t* y){\
	*y = ((ch/16)*8)+210;
	*x = (ch%16*8);
}


uint8_t cached[256] = {{0}};
void parseChar(uint8_t* font, uint8_t height,uint8_t c,uint8_t charc,uint16_t x,uint16_t y,long maxX,long maxY){
    char buffer[50];
    uint16_t cX,cY;

	uint8_t* current;
	uint8_t pixel[8][4];
	int i,j;
	for(i=0;i<8;i++){
		current = font + (i*256)+c;
		
		for(j=0;j<4;j++){
			uint8_t highPixInd = (j*2)+1;//1
			uint8_t lowPixInd = (j*2); //0
			uint8_t isHighSet = (*current>>highPixInd)&1; //00011000 >> 1 = 00001100 & 1 = 0
			uint8_t isLowSet  = (*current>>lowPixInd)&1;  //00011000 >> 0 = 00011000 & 1 = 0
			pixel[i][3-j] =0;
			if(isHighSet){
				pixel[i][3-j] |= (0x1<<4);
			}
			if(isLowSet){
				pixel[i][3-j] |= 0x1;
			}
			
		}

		//fontPointer++;
	}
	//getCachePos(c,&cX,&cY);

	//cached[c]=1;
	//}

	
	if((x+8)<=maxX&&(y+8)<=maxY){
		v99x8_hmmc(x,y,8,8, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, pixel);
		
		return;
	}
	v99x8_hmmc(0,210,8,8, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, pixel);
	
	if(x<=maxX&&(y+8)<=maxY){
		v99x8_lmmm(0, 210, x, y, maxX-x,8, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, V99X8_LOGICAL_OPERATION_IMP);
	}
	if((x+8)<=maxX&&y<=maxY){
		v99x8_lmmm(0, 210, x, y, 8,maxY-y, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, V99X8_LOGICAL_OPERATION_IMP);
	}
	return;
	int8_t xLeft = maxX-x;
	int8_t yLeft = maxY-y;
	if(xLeft>8)xLeft=8;
	if(yLeft>8)yLeft=8;
	if(yLeft<0||xLeft<0) return;
	v99x8_lmmm(0, 210, x, y, xLeft,yLeft, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, V99X8_LOGICAL_OPERATION_IMP);
	
	
	/*
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
*/
}
static long *clipping(Virtual *vwk, long *rect)
{
    vwk = (Virtual *) ((long) vwk & ~1);

    rect[0] = vwk->clip.rectangle.x1;
    rect[1] = vwk->clip.rectangle.y1;
    rect[2] = vwk->clip.rectangle.x2;
    rect[3] = vwk->clip.rectangle.y2;

    return rect;
}


long CDECL c_text_area(Virtual *vwk, short *text, long length, long dst_x, long dst_y, short *offsets)
{
    long rect[4];
    unsigned long foreground;
    unsigned long background;
    long *font;
    long w, h, mode;
    long ret;
    long *fgbg;

    if (vwk->text.effects)
        return 0;

    if (offsets)
        return 0;

    font = (long *) vwk->text.current_font->extra.unpacked.data;
    if (!font)                          /* Must have unpacked data */
        return 0;

    w = vwk->text.current_font->widest.cell;    /* Used to be character, which was wrong */
    if (w != 8)                         /* Only that width allowed for now */
        return 0;
uint8_t* fontPointer = (uint8_t*)vwk->text.current_font->data;
uint16_t* current;
 uint8_t low = vwk->text.current_font->code.low;
 uint8_t high = vwk->text.current_font->code.high;
 uint8_t width = vwk->text.current_font->width;
 uint8_t height = vwk->text.current_font->height;
 if(height!=8) return 0;
 dst_y += (&vwk->text.current_font->extra.distance.base)[vwk->text.alignment.vertical];
 	char buffer[50];
 	access->funcs.ltoa(buffer,*text,16);
		access->funcs.puts(buffer);
	clipping(vwk, rect);
	
	int i1;
	for(i1=0;i1<length;i1++){
		current = text+i1;
		uint8_t toPrint = *current;
	
		parseChar(fontPointer,height,toPrint,high-low,dst_x+(8*i1),dst_y,rect[2],rect[3]);
	}

	
	//parseChar(fontPointer,height,'A',high-low);
	
    
    fgbg = (long *) &vwk->text.colour;
    //get_colours_r(vwk, *fgbg, &foreground, &background);

    

    h = vwk->text.current_font->height;

    mode = vwk->mode;
    

	return 1;
    if (ret == -32)                     /* Unknown NatFeat? */
        return 0;

    return ret;
}
