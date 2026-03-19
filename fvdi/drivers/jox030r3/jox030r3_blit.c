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
#include "driver.h"
#include "jox030r3.h"
//#include "v9990.h"
static inline int is_screen(struct wk_ *wk, MFDB *mfdb)
{
    return (mfdb == NULL || mfdb->address == NULL || mfdb->address == wk->screen.mfdb.address) ? 1 : 0;
}
#define PIXEL		short
#define PIXEL_SIZE	sizeof(PIXEL)

short* mfdb_address = 0;


void printVar(long var) {
    char buffer[50];
	access->funcs.ltoa(buffer,var,10);
	access->funcs.puts(buffer);
}

void printBlit(long srcx,long srcy,long dstx,long dsty,long w,long h,long operation){
    access->funcs.puts("srcx:");
    printVar(srcx);
    access->funcs.puts(" srcy:");
    printVar(srcy);
    access->funcs.puts(" dstx:");
    printVar(dstx);
    access->funcs.puts(" dsty:");
    printVar(dsty);
    access->funcs.puts(" width:");
    printVar(w);
    access->funcs.puts(" height:");
    printVar(h);
    access->funcs.puts(" operation:");
    printVar(operation);
    access->funcs.puts("\n");
}

long CDECL
c_expand_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h,
              long operation, long colour)
{
    int src_is_screen = is_screen(vwk->real_address, src);
    int dst_is_screen = is_screen(vwk->real_address, dst);
   /* access->funcs.puts("src_x:");
    printVar(src_x);
    access->funcs.puts(" src_y:");
    printVar(src_y);
    access->funcs.puts(" src_w:");
    printVar(src->width);
    access->funcs.puts(" src_h:");
    printVar(src->height);
    access->funcs.puts(" dst_x:");
    printVar(dst_x);
    access->funcs.puts(" dst_y:");
    printVar(dst_y);
    access->funcs.puts("\r\nw:");
    printVar(w);
    access->funcs.puts(" h:");
    printVar(h);
    access->funcs.puts(" oper:");
    printVar(operation);
    access->funcs.puts(" col:");
    printVar(colour);
    access->funcs.puts("\n");
*/
#if 0
    static uint16_t exp_seq = 0;
    long fg = colour & 0xFFFF;
    long bg = (colour >> 16) & 0xFFFF;
    kprintf("c_expand_area()[%d]: src: is_screen? %s x: %ld, y: %ld\n", exp_seq++, src_is_screen ? "yes" : "no", src_x, src_y);
    kprintf("               dst: is_screen? %s x: %ld, y: %ld\n", dst_is_screen ? "yes" : "no", dst_x, dst_y);
    kprintf("               w: %ld, h: %ld, operation: %ld (%s), colour = 0x%08lx, fg = %ld, bg = %ld\n", w, h, operation,
            operation2string(operation), colour, fg, bg);
    kprintf("               dst_x: %ld, dst_t: %ld\n", dst_x, dst_y);
#endif

   // const uint16_t vram_save_address = 0xD000; // FIXME: Do proper VRAM management here.
    if (!src_is_screen && dst_is_screen) {
        if (src_x != 0 || src_y != 0) return 0;
        uint16_t pattern[16];
        uint16_t widthBlocks = w/16; 
        uint16_t blockRemainder = w%16;
        uint16_t heightBlocks = h/16;
        uint16_t heightRemainder = h%16;
        uint8_t fillerMode = 2;
        switch (operation){
            case 1: 
                fillerMode = 2;
                break;
            case 2:
                fillerMode = 6;
                break;
            case 3:
                fillerMode = 3;
                break;
            default:
                return 0;
        }


        int i;
        for(i=0;i<16;i++) pattern[i] = 0;
        int x,y,yb;
        int yRunning = src_y;
        for (yb = 0; yb < heightBlocks;yb++) {
            int xRunning = dst_x;

            for (x = 0; x<widthBlocks; x++) {
                for (y = 0; y<16 && (y+yRunning) < h; y++) {
                    pattern[y] = *(src->address + src->wdwidth * (y+yRunning) + x);
                };
                fillPattern(xRunning,dst_y + (16*yb),16,16,pattern,colour&0xff,colour>>8,fillerMode,false);
                xRunning += 16;
            }
            if (blockRemainder != 0) {
                //for(x=xRunning;x<xRunning+blockRemainder;x++){
                    for (y = 0; y<16 && (y+yRunning) < h; y++) {
                        pattern[y] = *(src->address + src->wdwidth * (y+yRunning) + widthBlocks);
                    };
                    fillPattern(xRunning,dst_y + (16*yb),blockRemainder,16,pattern,colour&0xff,colour>>8,fillerMode,false);
                //}
            }
            yRunning += 16;
        }
        if (heightRemainder != 0) {
            int xRunning = dst_x;

            for (x = 0; x<widthBlocks; x++) {
                for (y = 0; y<16 && (y+yRunning) < h; y++) {
                    pattern[y] = *(src->address + src->wdwidth * (y+yRunning) + x);
                };
                fillPattern(xRunning,dst_y + (16*heightBlocks),16,heightRemainder,pattern,colour&0xff,colour>>8,fillerMode,false);
                xRunning += 16;
            }
            if (blockRemainder != 0) {
                //for(x=xRunning;x<xRunning+blockRemainder;x++){
                    for (y = 0; y<16 && (y+yRunning) < h; y++) {
                        pattern[y] = *(src->address + src->wdwidth * (y+yRunning) + widthBlocks);
                    };
                    fillPattern(xRunning,dst_y + (16*heightBlocks),blockRemainder,heightRemainder,pattern,colour&0xff,colour>>8,fillerMode,false);
                //}
            }
        }
        
        return 1;
        //volatile xmreg_t *const xosera_ptr = xv_prep_dyn(me->device);
        //uint16_t buffer[4];

        //if (src_x != 0 || src_y != 0) return 0; /* Expect src pos to be 0,0; fallback if not. */
        //xm_setw(WR_INCR, 1);
        //xm_setw(WR_ADDR, vram_save_address);
        //short *p = src->address;

        // Expand the incoming monochrome bitmap to 4 bits per pixel.
        //for (short y = 0; y < src->height; y++) {
        //    for (short word = 0; word < src->wdwidth; word++) {
        //        uint16_t word_val = *p++;
        //        expand_word(buffer, word_val);
        //        for (int i = 0; i < 4; i++) vram_setw_next_wait(buffer[i]);
        //    }
        //}
        // Blit the newly created off-screen pixmap to the screen
        //long hw_color = c_get_colour(vwk, colour & 0xFFFF) & 0xF;
        //expand_blit(xosera_ptr, vram_save_address, dst_x, dst_y, w, h, expanded_color[hw_color],
        //            src->wdwidth * 4, operation, vwk->real_address->screen.mfdb.wdwidth);
        //return 1;
    }
    return 0;
}

long CDECL
c_blit_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h,
            long operation)
{
    int vram_save_x  = 1; /* FIXME: Don't hardcode this. */
    int vram_save_y = 768;
	int src_is_screen = is_screen(vwk->real_address, src);
    int dst_is_screen = is_screen(vwk->real_address, dst);

    if (src_is_screen && !dst_is_screen) {
         //   access->funcs.puts("saving screen\n");

        uint16_t width = w > dst->width ? w : dst->width;
		doBlit(src_x,src_y,vram_save_x,vram_save_y,width,h,operation);
        mfdb_address = dst->address;
        //
        //save_to_vram(xosera_ptr, vram_save_address, src_x, src_y, width, h, pf_words_per_line);
    }
    if (src_is_screen && dst_is_screen) {
                   // access->funcs.puts("blit\n");

		doBlit(src_x,src_y,dst_x,dst_y,w,h,operation);
        //uint16_t src_addr = src_y * pf_words_per_line + (src_x >> 2);
        //uint16_t dst_addr = dst_y * pf_words_per_line + (dst_x >> 2);
        //if ((dst_addr < src_addr) || operation == OPER_NOT_D) {
        //    screen_to_screen(xosera_ptr, src_addr, dst_addr, src_x, w, h, operation, pf_words_per_line);
        //} else if (src_y != dst_y) {
        //    screen_to_screen_reverse(xosera_ptr, dst_x, dst_y, src_x, src_y, w, h, operation, pf_words_per_line);
        //} else {
        //    save_to_vram(xosera_ptr, vram_save_address, src_x, src_y, w, h, pf_words_per_line);
        //    restore_to_screen(xosera_ptr, vram_save_address, dst_x, dst_y, w, h,
        //                      compute_word_width(dst_x, w), pf_words_per_line);
        //}
    }
    if (!src_is_screen && dst_is_screen) {
             //       access->funcs.puts("restoring screen\n");
        //access->funcs.puts("MFDB addr:");
        //printVar(src->address);

        if (mfdb_address != src->address) {
            //if (src_x != 0 || src_y != 0) return 0;
            /*access->funcs.puts("MFDB wdwidth:");
            printVar(src->wdwidth);
            access->funcs.puts("\r\n");

            access->funcs.puts("MFDB bitplanes:");
            printVar(src->bitplanes);
            access->funcs.puts("\r\n");
            */
            //access->funcs.puts("operation:");
            //printVar(operation);
            //access->funcs.puts("\r\n");
            /*MFDB wdwidth:12
            MFDB bitplanes:8
            MFDB standard:0
            */
            int x,y;
            for ( y = 0; y < h; y++)
                {
                    int dx = x%2;
                    for ( x = 0; x < (w/2); x++)
                    {
                        long src_offset = (src->wdwidth  * 2 * src->bitplanes) * (src_y + y)
                                        + (src_x + x) * PIXEL_SIZE;

                        //long offset = (src->wdwidth  * src->bitplanes) * (src_y+y) + (src_x+x) * PIXEL_SIZE;
        	            //unsigned long colour = *(unsigned PIXEL *)((long)src->address + offset);
                        if (operation == 7) {
                            *(vram + (dst_y+y)*1024+(dst_x+dx)) |= colorLUT[*(unsigned PIXEL *)((long)src->address + src_offset) >>8];
                        } else {
                            *(vram + (dst_y+y)*1024+(dst_x+dx)) = colorLUT[*(unsigned PIXEL *)((long)src->address + src_offset) >>8];
                        }
                        dx++;
                        if (operation == 7) {
                            *(vram + (dst_y+y)*1024+(dst_x+dx)) |= colorLUT[*(unsigned PIXEL *)((long)src->address + src_offset) &0xff];
                        } else {
                            *(vram + (dst_y+y)*1024+(dst_x+dx)) = colorLUT[*(unsigned PIXEL *)((long)src->address + src_offset) &0xff];
                        }                        dx++;
                        //*(unsigned PIXEL *)((long)dst->fd_addr + dst_offset) =
                        //*(unsigned PIXEL *)((long)src->fd_addr + src_offset);
                    }
                }
            return 1;
              
            
            
            return 0;
        }
        access->funcs.puts("\n");
        uint16_t width = w > src->width ? w : src->width;
		doBlit(vram_save_x,vram_save_y,dst_x & 0xfff0,dst_y,width,h,operation);
        //
        //PRINTF(("restoring screen: w = %ld, src->width = %d\n", w, src->width));

       // uint16_t src_wdwidth = compute_word_width(dst_x, src->width - src_x);
        //restore_to_screen(xosera_ptr, vram_save_address + (src_x >> 2), dst_x, dst_y, w, h,
        //                  src_wdwidth, pf_words_per_line);
    }
    if (!src_is_screen && !dst_is_screen) {
        return 0; /* fallback for now */
    }

    return 1;
}
/*
long CDECL
c_blit_area(Virtual *vwk, MFDB *src, long src_x, long src_y,
            MFDB *dst, long dst_x, long dst_y,
            long w, long h, long operation)
{
	Workstation *wk;
	PIXEL *src_addr, *dst_addr, *dst_addr_fast;
        uint8_t *addrcounter;
    int src_is_screen = is_screen(vwk->real_address, src);

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
	if (!src || !src->address || (src->address == wk->screen.mfdb.address)) {		/* From screen? 
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
	if (!dst || !dst->address || (dst->address == wk->screen.mfdb.address)) {		/* To screen? 
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
        if(from_screen && to_screen) /* From screen to screen (VRAM to VRAM blits only) 
        {
             doBlit(src_x,src_y,dst_x,dst_y,w,h);   
         
    

        } else if(!from_screen && to_screen) /* From memory to screen 
        {

		return 0;

        } else if(from_screen && !to_screen) /* From screen to memory 
        {
        
		return 0;
        	//v99x8_lmmm(src_x, src_y, dst_x, dst_y+300, w, h, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, lo);
                //v99x8_lmcm(src_x, src_y, w, h, V99X8_ARGUMENT_MXD_VRAM | V99X8_ARGUMENT_DIY_DOWN | V99X8_ARGUMENT_DIX_RIGHT, dstaddr+dst_pos);
                
           
        } else
        {
                return 0;
        }

	return 1;	/* Return as completed 
}
*/
