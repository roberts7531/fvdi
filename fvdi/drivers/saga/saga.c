/*
 * saga.c - SAGA specific functions
 * This is part of the SAGA driver for fVDI
 * Most of the code here come from the SAGA Picasso96 driver.
 * https://github.com/ezrec/saga-drivers/tree/master/saga.card
 * Glued by Vincent Riviere
 * Additions by Thorsten Otto
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
#include "saga.h"
#include "video.h"
#include "board.h"

/* The delta allows negative positioning */
#define SAGA_MOUSE_DELTAX  16
#define SAGA_MOUSE_DELTAY  8


/*
 * Inspired from:
 * https://github.com/ezrec/saga-drivers/blob/master/saga.card/setclock.c
 */
void saga_set_clock(const struct ModeInfo *mi)
{
    int clock_id = ((ULONG)mi->Numerator << 8) | mi->Denomerator;

    saga_pll_clock_program(clock_id);
}

/*
 * Inspired from:
 * https://github.com/ezrec/saga-drivers/blob/master/saga.card/initcard.c
 */
void saga_fix_mode(struct ModeInfo *mi)
{
    BOOL is_NTSC = FALSE;
    int refresh;
    int clock_id;

    refresh = mi->Numerator;
    if (!refresh)
        refresh = 60;

    mi->PixelClock = (ULONG)mi->HorTotal * mi->VerTotal * refresh;
    if (mi->Flags & GMF_DOUBLESCAN)
        mi->PixelClock *= 2;
    if (mi->Flags & GMF_DOUBLECLOCK)
        mi->PixelClock *= 2;

    /* Fix up PixelClock to a 'sane' value */
    clock_id = saga_pll_clock_lookup(is_NTSC, &mi->PixelClock);
    mi->Numerator = (clock_id >> 8) & 0xff;
    mi->Denomerator = (clock_id >> 0) & 0xff;
}

/*
 * Inspired from:
 * https://github.com/ezrec/saga-drivers/blob/master/saga.card/setgc.c
 */
void saga_set_modeline(const struct ModeInfo *mi, UBYTE Format)
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

    /* turn on video out enable/video RTG */
    Write16(0xDFF100, 0x0280);
}

/*
 * Inspired from:
 * https://github.com/ezrec/saga-drivers/blob/master/saga.card/setgc.c
 */
void saga_set_panning(UBYTE *mem)
{
    Write32(SAGA_VIDEO_PLANEPTR, (IPTR)mem);
}


void saga_set_mouse_position(short x, short y)
{
	if (x < -16)
	{
		/* put position outside window */
		Write16(SAGA_VIDEO_SPRITEX, SAGA_VIDEO_MAXHV - 1);
		Write16(SAGA_VIDEO_SPRITEY, SAGA_VIDEO_MAXVV - 1);
	} else
	{
		UBYTE boardid = ( *(volatile UWORD*)VREG_BOARD ) >> 8;
		
		if (boardid == VREG_BOARD_V4SA)
		{
			x += SAGA_MOUSE_DELTAX;
			y += SAGA_MOUSE_DELTAY;
		}
		Write16(SAGA_VIDEO_SPRITEX, x);
		Write16(SAGA_VIDEO_SPRITEY, y);
	}
}


static unsigned short rgb16to4(unsigned short color)
{
	/*
	 * Input : 0xRRRRRGGGGGGBBBBB
	 * Output: 0x0000RRRRGGGGBBBB
	 */
	return ((color >> 4) & 0xf00) |
	       ((color >> 3) & 0x0f0) |
	       ((color >> 1) & 0x00f);
}


/*
 * In amiga sprites, each pixel can be one of three colors
 * or transparent. Figure below shows how the color of each pixel in a sprite
 *  is determined.
 * 
 *                                                  high-order word of
 *                                                   sprite data line
 *    _______________________________
 *   |            _|#|_              |    _ _0 0 0 0 0 1 1 1 0 1 1 1 0 0 0 0
 *   |          _|o|#|o|_            |   |
 *   |_ _ _ _ _|o|o|#|o|o|_ _ _ _ _ _|       |
 *   |_|_|_|_|#|#|#|_|#|#|#|_|_|_|_|_|- -|   |
 *   |    |    |o|o|#|o|o|           |       |
 *   |    |      |o|#|o|             |   |_ _|_0 0 0 0 0 1 1 1 0 1 1 1 0 0 0 0
 *   |____|________|#|_______________|       |
 *        |                                  | |      low-order word of
 *        |                                  | |      sprite data line
 *                                           | |
 *   transparent                             | |
 *                  Forms a binary code,
 *                    used as the color  --> 0 0
 *                  choice from a group
 *                   of color registers.
 *
 * For our purposes, we set index 1 to the mask color,
 * and index 2&3 to the data color of the mouse definition
 */

void saga_set_mouse_sprite(Workstation *wk, Mouse *mouse)
{
	unsigned short fg, bg;
	Colour *global_palette;
	unsigned short *realp;
	unsigned short mousedata[2 * 16];
	unsigned long *lp;
	int i;
	
	/* c_get_colour(wk, *pp, &foreground, &background); */
	global_palette = wk->screen.palette.colours;
	realp = (unsigned short *)&global_palette[wk->mouse.colour.foreground].real;
	fg = *realp;
	realp = (unsigned short *)&global_palette[wk->mouse.colour.background].real;
	bg = *realp;

	fg = rgb16to4(fg);
	bg = rgb16to4(bg);

	/* SAGA HW MOUSE => COLORS */
	Write16(SAGA_VIDEO_SPRITECLUT + 0, bg);  /* COLOR1 */
	Write16(SAGA_VIDEO_SPRITECLUT + 2, fg);  /* COLOR2 */
	Write16(SAGA_VIDEO_SPRITECLUT + 4, fg);  /* COLOR3 */
	
	/* SAGA HW MOUSE => BITPLANES DATA */
	for (i = 0; i < 16; i++)
	{
		mousedata[i * 2 + 0] = mouse->mask[i];
		mousedata[i * 2 + 1] = mouse->data[i];
	}
	lp = (unsigned long *)mousedata;
	for (i = 0; i < 16; i++)
		Write32(SAGA_VIDEO_SPRITEBPL + i * 4, lp[i]);
}
