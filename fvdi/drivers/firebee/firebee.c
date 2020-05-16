/*
 * firebee.c - FireBee specific functions
 * This is part of the FireBee driver for fVDI
 *
 * https://github.com/ezrec/saga-drivers/tree/master/saga.card
 * Glued by Vincent Riviere
 * Reused for the FireBee by Markus Fröschle
 */

/*
 * Copyright (C) 2016, Jason S. McMullan <jason.mcmullan@gmail.com>
 * All rights reserved.
 *
 * Licensed under the MIT License:
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "fvdi.h"
#include "driver.h"
#include "firebee.h"
#include "video.h"

/*
 * Inspired from:
 * https://github.com/ezrec/saga-drivers/blob/master/saga.card/setclock.c
 */
void fbee_set_clock(const struct ModeInfo *mi)
{
}

/*
 * Inspired from:
 * https://github.com/ezrec/saga-drivers/blob/master/saga.card/initcard.c
 */
void fbee_fix_mode(struct ModeInfo *mi)
{
}

/*
 * Inspired from:
 * https://github.com/ezrec/saga-drivers/blob/master/saga.card/setgc.c
 */
void fbee_set_modeline(const struct ModeInfo *mi, UBYTE Format)
{
    UWORD width, hsstrt, hsstop, htotal;
    UWORD height, vsstrt, vsstop, vtotal;
    UBYTE doublescan = 0;

    /* Borders (mi->HorBlankSize and mi->VerBlankSize)
     * are not truely supported, since the border color
     * can not be set.
     */

    width = mi->Width;
    hsstrt = width + mi->HorBlankSize + mi->HorSyncStart;
    hsstop = hsstrt + mi->HorSyncSize;
    htotal = mi->HorTotal;

    height = mi->Height;
    vsstrt = height + mi->VerBlankSize + mi->VerSyncStart;
    vsstop = vsstrt + mi->VerSyncSize;
    vtotal = mi->VerTotal;

    if (IS_DOUBLEX(width)) {
        doublescan |= SAGA_VIDEO_DBLSCAN_X;
        hsstrt <<= 1;
        hsstop <<= 1;
        htotal <<= 1;
        width  <<= 1;
    }

    if (IS_DOUBLEY(height)) {
        doublescan |= SAGA_VIDEO_DBLSCAN_Y;
        vsstrt <<= 1;
        vsstop <<= 1;
        vtotal <<= 1;
        height <<= 1;
    }

    /* Monitor mode info */
    /*
    debug("ModeLine \"%ldx%ld\" %ld, %ld %ld %ld %ld, %ld %ld %ld %ld, %sHSync %sVSync%s%s",
            width, height, mi->PixelClock / 1000000,
            width, hsstrt, hsstop, htotal,
            height, vsstrt, vsstop, vtotal,
            (mi->Flags & GMF_HPOLARITY) ? "+" : "-",
            (mi->Flags & GMF_VPOLARITY) ? "+" : "-",
            (doublescan & SAGA_VIDEO_DBLSCAN_X) ? " DoubleScanX" : "",
            (doublescan & SAGA_VIDEO_DBLSCAN_Y) ? " DoubleScanY" : "");
*/
    Write16(SAGA_VIDEO_HPIXEL, width);
    Write16(SAGA_VIDEO_HSSTRT, hsstrt);
    Write16(SAGA_VIDEO_HSSTOP, hsstop);
    Write16(SAGA_VIDEO_HTOTAL, htotal);

    Write16(SAGA_VIDEO_VPIXEL, height);
    Write16(SAGA_VIDEO_VSSTRT, vsstrt);
    Write16(SAGA_VIDEO_VSSTOP, vsstop);
    Write16(SAGA_VIDEO_VTOTAL, vtotal);

    Write16(SAGA_VIDEO_HVSYNC, ((mi->Flags & GMF_HPOLARITY) ? (1 << 0) : 0) |
            ((mi->Flags & GMF_VPOLARITY) ? (1 << 1) : 0));

    Write16(SAGA_VIDEO_MODE, SAGA_VIDEO_MODE_FORMAT(Format) |
            SAGA_VIDEO_MODE_DBLSCN(doublescan));
}

/*
 * Inspired from:
 * https://github.com/ezrec/saga-drivers/blob/master/saga.card/setgc.c
 */
void fbee_set_panning(UBYTE *mem)
{
    Write32(SAGA_VIDEO_PLANEPTR, (IPTR)mem);
}
