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

extern void CDECL c_get_colours(Virtual *vwk, long colour, short *foreground, short *background);

extern long CDECL clip_line(Virtual *vwk, long *x1, long *y1, long *x2, long *y2);



void draw_line(long x_0, long y_0, long x_1, long y_1, uint8_t colour,enum v99x8_logical_operation op) {
    enum v99x8_argument arg = V99X8_ARGUMENT_MXD_VRAM;

    long maj = x_1 - x_0;
    long min = y_1 - y_0;

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
	}
	/* Oh right, the line should be patterned, well nevermind, it is going to be solid for now
        pat = (pattern & 0x08 << 13) -1;
        pat |= (pattern & 0x04 << 10) -1;
        pat |= (pattern & 0x02 << 7) -1;
        pat |= (pattern & 0x01 << 4) -1;
        */
        draw_line(x1, y1, x2, y2, colour&0xf, op) ;
        
	return 1;	
}
