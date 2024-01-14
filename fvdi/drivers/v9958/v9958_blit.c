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
#include "v9990.h"

#define PIXEL		short
#define PIXEL_SIZE	sizeof(PIXEL)


long CDECL
c_blit_area(Virtual *vwk, MFDB *src, long src_x, long src_y,
            MFDB *dst, long dst_x, long dst_y,
            long w, long h, long operation)
{
	Workstation *wk;

	int from_screen, to_screen;








	wk = vwk->real_address;

	from_screen = 0;
	if (!src || !src->address || (src->address == wk->screen.mfdb.address)) {		/* From screen? */

		
		from_screen = 1;
	} else {



	}

	//src_line_add = src_wrap - w * PIXEL_SIZE;


	to_screen = 0;
	if (!dst || !dst->address || (dst->address == wk->screen.mfdb.address)) {		/* To screen? */


		to_screen = 1;
	} else {



	}

	//dst_line_add = dst_wrap - w * PIXEL_SIZE;

        if(from_screen && to_screen) /* From screen to screen (VRAM to VRAM blits only) */
        {
                

                enum v99x8_logical_operation lo;

		switch(operation) {
	        case 1:				/* Replace */
                        //VDPWriteReg(VDP_LOP,VDP_LOP_WCANDSC);
                        lo=V99X8_LOGICAL_OPERATION_IMP;
	        	break;
	        case 2:				/* Transparent */
                        //VDPWriteReg(VDP_LOP,VDP_LOP_WCORSC);
                        lo=V99X8_LOGICAL_OPERATION_OR;
	        	break;
		case 3:
                        //VDPWriteReg(VDP_LOP,VDP_LOP_WCSC);
                        lo=V99X8_LOGICAL_OPERATION_IMP;
			break;
		case 6:
                        //VDPWriteReg(VDP_LOP,VDP_LOP_WCEORSC);
                        lo=V99X8_LOGICAL_OPERATION_XOR;
			break;
		case 7:
                        //VDPWriteReg(VDP_LOP,VDP_LOP_WCORSC);
			lo=V99X8_LOGICAL_OPERATION_OR;
			break;
		case 10:
                        //VDPWriteReg(VDP_LOP,VDP_LOP_WCNOTSC);
			lo=V99X8_LOGICAL_OPERATION_NOT;
			break;
		default:
                        //VDPWriteReg(VDP_LOP,VDP_LOP_WCSC);
			lo=V99X8_LOGICAL_OPERATION_IMP;
			break;
		}
		v99x8_lmmm(src_x, src_y, dst_x, dst_y, w, h, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, lo);

        } else if(!from_screen && to_screen) /* From memory to screen */
        {

		enum v99x8_logical_operation lo;
		switch(operation) {
	        case 1:				/* Replace */
                        lo=V99X8_LOGICAL_OPERATION_IMP;
	        	break;
	        case 2:				/* Transparent */
                        lo=V99X8_LOGICAL_OPERATION_OR;
	        	break;
		case 3:
                        lo=V99X8_LOGICAL_OPERATION_IMP;
			break;
		case 7:
                        lo=V99X8_LOGICAL_OPERATION_OR;
			break;
		default:
                        lo=V99X8_LOGICAL_OPERATION_IMP;
			break;
		}
		v99x8_lmmm(src_x, src_y+300, dst_x, dst_y, w, h, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, lo);
		//v99x8_lmmc(dst_x, dst_y, w, h,  V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, lo, srcaddr);

        } else if(from_screen && !to_screen) /* From screen to memory */
        {
        	enum v99x8_logical_operation lo;
		switch(operation) {
	        case 1:				/* Replace */
                        lo=V99X8_LOGICAL_OPERATION_IMP;
	        	break;
	        case 2:				/* Transparent */
                        lo=V99X8_LOGICAL_OPERATION_OR;
	        	break;
		case 3:
                        lo=V99X8_LOGICAL_OPERATION_IMP;
			break;
		case 7:
                        lo=V99X8_LOGICAL_OPERATION_OR;
			break;
		default:
                        lo=V99X8_LOGICAL_OPERATION_IMP;
			break;
		}
        	v99x8_lmmm(src_x, src_y, dst_x, dst_y+300, w, h, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, lo);
                //v99x8_lmcm(src_x, src_y, w, h, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, dstaddr);
                
           
        } else
        {
                return 0;
        }

	return 1;	/* Return as completed */
}

