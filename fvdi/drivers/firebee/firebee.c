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


static void set_fb_clockmode(enum fb_clockmode mode)
{
    if (mode > FB_CLOCK_PLL)
    {
        exit(1);
    }

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
void fbee_set_clock(int clock)
{
    set_fb_clockmode(FB_CLOCK_PLL);

    wait_pll();
    fb_vd_frq = clock - 1;
    wait_pll();
    fb_vd_pll_reconfig = 0;
}
