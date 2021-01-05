/*
 * firebee.c - FireBee specific functions
 * This is part of the FireBee driver for fVDI
 *
 * https://github.com/ezrec/saga-drivers/tree/master/saga.card
 * Glued by Vincent Riviere
 * Reused for the FireBee by Markus Fröschle
 */

#include "fvdi.h"
#include "driver.h"
#include "firebee.h"
#include "fb_video.h"
#include <stdlib.h>
#include "modeline.h"

static void fbee_set_clockmode(enum fb_clockmode mode)
{
    if (!(mode <= FB_CLOCK_PLL))
    {
        access->funcs.puts("error: illegal clock mode\r\n");
        for (;;);
    }

    fb_vd_cntrl = (fb_vd_cntrl & ~(FB_CLOCK_MASK)) | mode << 8;
    fb_vd_cntrl = (fb_vd_cntrl & ~(FB_CLOCK_MASK)) | mode << 8;
}

/*
 * wait for the Firebee clock generator to be idle
 */
static void wait_pll(void)
{
    do {} while (fb_vd_pll_reconfig < 0);   /* wait until video PLL not busy */
}

/*
 * set the Firebee video (pixel) clock to <clock> MHz.
 */
void fbee_set_clock(unsigned short clock)
{
    fbee_set_clockmode(FB_CLOCK_PLL);

    wait_pll();
    fb_vd_frq = clock - 1;
    wait_pll();
    fb_vd_pll_reconfig = 0;
}

void fbee_set_screen(volatile struct videl_registers *regs, void *adr)
{
    regs->vbasx = ((unsigned long) adr >> 16) & 0x3ff;
    regs->vbasm = ((unsigned long) adr >> 8) & 0xff;
    regs->vbasl = ((unsigned long) adr);
}

void fbee_set_video(short *screen_address)
{
    fbee_set_screen(&videl_regs, screen_address);

    fb_vd_cntrl &= ~(FALCON_SHIFT_MODE | ST_SHIFT_MODE | FB_VIDEO_ON | VIDEO_DAC_ON);

    /* it appears we can only enable FireBee video if we write 0 to ST shift mode
     * and Falcon shift mode in exactly this sequence
     */
    videl_regs.stsft = 0;
    videl_regs.spshift = 0;

    set_videl_regs_from_modeline(&modeline, &videl_regs);

    fb_vd_cntrl |= COLOR16 | NEG_SYNC_ALLOWED;
    fb_vd_cntrl &= ~(FALCON_SHIFT_MODE | ST_SHIFT_MODE | COLOR24 | COLOR8 | COLOR1);
    fb_vd_cntrl |= FB_VIDEO_ON | VIDEO_DAC_ON;
}

void set_videl_regs_from_modeline(struct modeline *ml, volatile struct videl_registers *vr)
{
    unsigned short left_margin = (ml->h_total - ml->h_display) / 2;
    unsigned short upper_margin = (ml->v_total - ml->v_display) / 2;

    fbee_set_clockmode(FB_CLOCK_PLL);
    fbee_set_clock(ml->pixel_clock);

    vr->hht = ml->h_total;
    vr->hde = left_margin - 1 + ml->h_display;
    vr->hbe = left_margin - 1;
    vr->hdb = left_margin;
    vr->hbb = left_margin + ml->h_display;
    vr->hss = ml->h_total - (ml->h_sync_end - ml->h_sync_start);

    vr->vft = ml->v_total;
    vr->vde = upper_margin + ml->v_display - 1;
    vr->vbe = upper_margin - 1;
    vr->vdb = upper_margin;
    vr->vbb = upper_margin + ml->v_display;

    vr->vss = ml->v_total - (ml->v_sync_end - ml->v_sync_start);
}

