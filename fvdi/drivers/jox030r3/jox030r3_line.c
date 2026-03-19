/* 
 * A 16 bit graphics line routine, by Johan Klockars.
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
#include "relocate.h"
#include <stdint.h>
extern Access *access;
extern void CDECL c_get_colours(Virtual *vwk, long colour, short *foreground, short *background);

extern long CDECL clip_line(Virtual *vwk, long *x1, long *y1, long *x2, long *y2);

extern void fillPattern(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t* pattern,uint8_t color,uint8_t bgcol, uint8_t isXor);
void draw_str_line(long x_0, long y_0, long x_1, long y_1, uint16_t color,uint8_t operation) {
    uint16_t pattern[16] = {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff};
	
	uint16_t x,y,width,height;
	if(x_0==x_1) {
		x = x_0;
		y = (y_0 < y_1) ? y_0 : y_1;
		width = 1;
		height = (y_0 < y_1) ? y_1 - y_0 : y_0 - y_1 ;
	} else {
		y = y_0;
		x = (x_0 < x_1) ? x_0 : x_1;
		height = 1;
		width = (x_0 < x_1) ? x_1 - x_0 : x_0 - x_1;
	}
	
	uint8_t fgcol = (operation == 1) ? color >>8 : color & 0xff;
	uint8_t bgcol = (operation == 1) ? color & 0xff : color >> 8;
	fillPattern(x,y,width,height,pattern,fgcol,bgcol,operation ==1);

}
long CDECL c_line_draw(Virtual *vwk, long x1, long y1, long x2, long y2,
                       long pattern, long colour, long mode)
{
	Workstation *wk;
	short *addr;
	
	long pos;
	

	if ((long)vwk & 1) {
		return -1;			/* Don't know about anything yet */
	}

	if (!clip_line(vwk, &x1, &y1, &x2, &y2))
		return 1;
	
	
	wk = vwk->real_address;

	pos = (short)y1 * (long)wk->screen.wrap + x1 * 2;
	addr = wk->screen.mfdb.address;
	addr += pos >> 1;
	uint8_t op;
	switch (mode) {
	case 1:				/* Replace */
                op = 0;
		break;
	case 2:				/* Transparent */
                return 0; //op = V99X8_LOGICAL_OPERATION_OR;
		break;
	case 3:				/* XOR */
                op = 1;
		break;
	case 4:				/* Reverse transparent */
                return 0;
		break;
	default:
		op = 0;
	}
	/* Oh right, the line should be patterned, well nevermind, it is going to be solid for now
        pat = (pattern & 0x08 << 13) -1;
        pat |= (pattern & 0x04 << 10) -1;
        pat |= (pattern & 0x02 << 7) -1;
        pat |= (pattern & 0x01 << 4) -1;
        */
       if(x1==x2||y1==y2){
       		//draw straight line
       		draw_str_line(x1, y1, x2, y2, colour, op) ;
       
       }else{
       		//This routine is bugged, good luck!
        	//draw_line(x1, y1, x2, y2, colour&0xf, op) ;
			return 0;
       }
	return 1;	
}
