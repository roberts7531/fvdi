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
#include "v99x8.h"
#include "relocate.h"
extern Access *access;
extern void CDECL c_get_colours(Virtual *vwk, long colour, short *foreground, short *background);

extern long CDECL clip_line(Virtual *vwk, long *x1, long *y1, long *x2, long *y2);


void draw_str_line(long x0, long y0, long x1, long y1, uint8_t colour,enum v99x8_logical_operation op) {
   //TODO patterns
   if(x0==y0) return;//whats the point of drawing nothing 
   if(x0==x1){
   // y dir
   	int dy=0;
   	int ny=0;
   	if(y1<y0){
   		dy = y1;
   		ny = y0-y1;
   	}else{
   		dy = y0;
   		ny = y1-y0;
   	}
   
   //void v99x8_lmmv(uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo);
   	if(ny>0)
   	v99x8_lmmv(x0, dy, 1, ny, colour, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, op);
   }else{
   // x dir
   	int dx=0;
   	int nx=0;
   	if(x1<x0){
   		dx = x1;
   		nx = x0-x1;
   	}else{
   		dx = x0;
   		nx = x1-x0;
   	}
   
   //void v99x8_lmmv(uint16_t dx, uint16_t dy, uint16_t nx, uint16_t ny, uint8_t color, enum v99x8_argument arg, enum v99x8_logical_operation lo);
   	if(nx>0)
   	v99x8_lmmv(dx, y0, nx, 1, colour, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, op);
   }
   
}
void draw_line(long x_0, long y_0, long x_1, long y_1, uint8_t colour,enum v99x8_logical_operation op) {
    enum v99x8_argument arg = V99X8_ARGUMENT_MXD_VRAM;
	//wtf Drawing line x1:353 y1:34 x2:353 y2:90 mode:3

    long maj = x_1 - x_0; //0
    long min = y_1 - y_0; //56
    int log =0;
    if (min>50 || min<-50){
    	//this is the problem draw
    	log =1;
    }	
    if (maj < 0) {
        maj = -maj;
        arg |= V99X8_ARGUMENT_DIX_LEFT;
    } else {
        arg |= V99X8_ARGUMENT_DIX_RIGHT;
    }

    if (min < 0) {
        min = -min;
        arg |= V99X8_ARGUMENT_DIY_UP;
    } else {
        arg |= V99X8_ARGUMENT_DIY_DOWN;
    }

    if (maj < min) {
        long tmp = maj;
        maj = min;
        min = tmp;
        arg |= V99X8_ARGUMENT_MAJ_LONG_Y;
    } else {
        arg |= V99X8_ARGUMENT_MAJ_LONG_X;
    }
    if(log){
     access->funcs.puts("Drawing line x1:");
	char buffer[50];
	access->funcs.ltoa(buffer,x_0,10);
	access->funcs.puts(buffer);
	access->funcs.puts(" y1:");

	access->funcs.ltoa(buffer,y_0,10);
	access->funcs.puts(buffer);
	access->funcs.puts(" x2:");

	access->funcs.ltoa(buffer,x_1,10);
	access->funcs.puts(buffer);
	access->funcs.puts(" y2:");

	access->funcs.ltoa(buffer,y_1,10);
	access->funcs.puts(buffer);
	access->funcs.puts(" maj:");

	access->funcs.ltoa(buffer,maj,10);
	access->funcs.puts(buffer);
	access->funcs.puts(" min:");

	access->funcs.ltoa(buffer,min,10);
	access->funcs.puts(buffer);
	access->funcs.puts("\r\n");
    }
    v99x8_line(x_0, y_0, maj, min, colour, arg, op);
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
	enum v99x8_logical_operation op;
	switch (mode) {
	case 1:				/* Replace */
                op = V99X8_LOGICAL_OPERATION_IMP;
		break;
	case 2:				/* Transparent */
                op = V99X8_LOGICAL_OPERATION_OR;
		break;
	case 3:				/* XOR */
                op = V99X8_LOGICAL_OPERATION_XOR;
		break;
	case 4:				/* Reverse transparent */
                op = V99X8_LOGICAL_OPERATION_AND;
		break;
	default:
		op = V99X8_LOGICAL_OPERATION_IMP;
	}
	/* Oh right, the line should be patterned, well nevermind, it is going to be solid for now
        pat = (pattern & 0x08 << 13) -1;
        pat |= (pattern & 0x04 << 10) -1;
        pat |= (pattern & 0x02 << 7) -1;
        pat |= (pattern & 0x01 << 4) -1;
        */
       if(x1==x2||y1==y2){
       		//draw straight line
       		draw_str_line(x1, y1, x2, y2, colour&0xf, op) ;
       
       }else{
       		//This routine is bugged, good luck!
        	draw_line(x1, y1, x2, y2, colour&0xf, op) ;
       }
	return 1;	
}
