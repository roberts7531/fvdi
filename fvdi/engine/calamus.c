/*
 * fVDI Calamus functions
 *
 * Copyright 2004, Standa Opichal
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

#include "fvdi.h"
#include "stdio.h"
#include "relocate.h"
#include "utility.h"
#include "function.h"
#include "globals.h"
#include "calamus.h"


/* From the dcsdstub.s file */
long CDECL dcsd_stub_active(void);
void *CDECL dcsd_stub_getbase(void);
void CDECL dcsd_stub_gettlt(unsigned char tlt[256]);
void CDECL dcsd_stub_blit_from_screen(struct DCSD_BLITARGS *args);
void CDECL dcsd_stub_blit_to_screen(struct DCSD_BLITARGS *args);


/* Driver's static variables */
MFDB dcsd_offscreen_mfdb;
long is_active = 0;
static Virtual *dcsd_vwk = 0L;


/*
 * Initialize the driver.
 *
 * This function is called by Calamus during its startup.
 */
static void CDECL dcsd_init(void)
{
    long size;
    is_active = 1;

    dcsd_vwk = screen_vwk;

    dcsd_offscreen_mfdb.width = screen_wk->screen.mfdb.width;
    dcsd_offscreen_mfdb.height = screen_wk->screen.mfdb.height;
    dcsd_offscreen_mfdb.wdwidth = screen_wk->screen.mfdb.wdwidth;
    dcsd_offscreen_mfdb.bitplanes = screen_wk->screen.mfdb.bitplanes;
    dcsd_offscreen_mfdb.standard = 0;

    /* Allocate offscreen buffer for the whole screen to be
     * directly used by Calamus
     */

    size = dcsd_offscreen_mfdb.wdwidth * 2L * dcsd_offscreen_mfdb.height * dcsd_offscreen_mfdb.bitplanes;

    dcsd_offscreen_mfdb.address = (void *)malloc(size);

    PRINTF(("dcsd_init: $%08lx [%d,%d,%d,%d -> %ld]\n",
        (long) dcsd_offscreen_mfdb.address,
        dcsd_offscreen_mfdb.width,
        dcsd_offscreen_mfdb.height,
        dcsd_offscreen_mfdb.wdwidth,
        dcsd_offscreen_mfdb.bitplanes,
        size));
}


static void CDECL dcsd_exit(void)
{
    PUTS("dcsd_exit\n");

    /* Do nothing if inactive:
     * - faulty state (Calamus crashed badly or something) */
    if (!is_active)
        return;

    /* Mark inactive */
    is_active = 0;

    PUTS("dcsd_exit: destroy\n");

    /* Delete the allocated offscreen buffer */
    free(dcsd_offscreen_mfdb.address);
}


/*
 * Returns the driver initialization status
 * (whether the init() function was called already)
 */
long CDECL dcsd_active(void)
{
    PRINTF(("dcsd_active: %s\n", is_active ? "yes" : "no"));

    return is_active;
}


/*
 * Returns the driver allocated offscreen buffer address to be used.
 */
void *CDECL dcsd_getbase(void)
{
    PRINTF(("dcsd_getbase: $%08lx\n", (long) dcsd_offscreen_mfdb.address));

    return dcsd_offscreen_mfdb.address;
}


/*
 * Fills in an index color lookup table if needed.
 */
void CDECL dcsd_gettlt(unsigned char tlt[256])
{
    int i;

    PUTS("dcsd_gettlt\n");

    /* FIXME: TODO give the driver translation table if necessary */
    for (i = 0; i < 256; i++)
        tlt[i] = i;
}


void CDECL dcsd_blit_from_screen(struct DCSD_BLITARGS *args)
{
    short coords[8];
    struct clip_ clipping;

    clipping = dcsd_vwk->clip;

    coords[0] = args->x;
    coords[1] = args->y;
    coords[2] = coords[0] + args->w - 1;
    coords[3] = coords[1] + args->h - 1;
    /* Dest and source coords are the same */
    coords[4] = coords[0];
    coords[5] = coords[1];
    coords[6] = coords[2];
    coords[7] = coords[3];

    PRINTF(("dcsd_blit_S2M: %d,%d,%d,%d\n", coords[0], coords[1], coords[2] - coords[0], coords[3] - coords[1]));

    /* FIXME: BPP transformations are necessary (now BPP 32bit only) */

    lib_vs_clip(dcsd_vwk, 0, NULL);

    /* Note: From screen direction should always use the 'replace' mode */
    lib_vdi_spppp(&lib_vro_cpyfm, dcsd_vwk, 3, coords, 0L, &dcsd_offscreen_mfdb, 0L);

    lib_vs_clip(dcsd_vwk, clipping.on, &clipping.rectangle.x1);
}


void CDECL dcsd_blit_to_screen(struct DCSD_BLITARGS *args)
{
    short coords[8];
    struct clip_ clipping;

    clipping = dcsd_vwk->clip;

    coords[0] = args->x;
    coords[1] = args->y;
    coords[2] = coords[0] + args->w - 1;
    coords[3] = coords[1] + args->h - 1;
    /* Dest and source coords are the same */
    coords[4] = coords[0];
    coords[5] = coords[1];
    coords[6] = coords[2];
    coords[7] = coords[3];

    PRINTF(("dcsd_blit_M2S: m=%d %d,%d,%d,%d\n", args->mode, coords[0], coords[1], coords[2] - coords[0], coords[3] - coords[1]));

    /* FIXME: BPP transformations are necessary (now BPP 32bit only)
     *
     * Calamus uses 1, 8 or 32bit offscreen buffer according to
     * the following table:
     *
     * VDI BPP    Calamus
     * 1          1
     * 2          8
     * 4          8
     * 8          8
     * 16        32
     * 24        32
     * 32        32
     */

    lib_vs_clip(dcsd_vwk, 0, NULL);

    lib_vdi_spppp(&lib_vro_cpyfm, dcsd_vwk, args->mode, coords, &dcsd_offscreen_mfdb, 0L, 0L);

    lib_vs_clip(dcsd_vwk, clipping.on, &clipping.rectangle.x1);
}


void calamus_initialize_cookie(struct DCSD_cookie *cookie, short version)
{
    cookie->version = version;   /* 0x0100 */
    cookie->init = dcsd_init;
    cookie->exit = dcsd_exit;
    cookie->active = dcsd_stub_active;
    cookie->getbase = dcsd_stub_getbase;
    cookie->gettlt = dcsd_stub_gettlt;
    cookie->blit_from_screen = dcsd_stub_blit_from_screen;
    cookie->blit_to_screen = dcsd_stub_blit_to_screen;
    cookie->custom = 0;
}
