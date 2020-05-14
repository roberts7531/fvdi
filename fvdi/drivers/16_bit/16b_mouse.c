/*
 * Bitplane mouse routine
 *
 * Copyright 2006, Johan Klockars
 * Copyright 2002 The EmuTOS development team
 * Copyright 1999 by Caldera, Inc. and Authors:
 * Copyright 1982 by Digital Research Inc.
 *
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

#include "fvdi.h"
#include "driver.h"
#include "../bitplane/bitplane.h"

#define PIXEL       short
#define PIXEL_SIZE  sizeof(PIXEL)

static unsigned long mouse_save_state = 0;
static long mouse_old_colours = 0;
static PIXEL mouse_foreground = 0xffff;
static PIXEL mouse_background = 0;

static unsigned short mouse_data[16 * 2] = {
    0xffff, 0x0000, 0x7ffe, 0x3ffc, 0x3ffc, 0x1ff8, 0x1ff8, 0x0ff0,
    0x0ff0, 0x07e0, 0x07e0, 0x03c0, 0x03c0, 0x0180, 0x0180, 0x0000
};
static PIXEL mouse_saved[16 * 16];



static void set_mouse_shape(Mouse *mouse, unsigned short *masks)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        *masks++ = mouse->mask[i];
        *masks++ = mouse->data[i];
    }
}


static void hide_mouse(Workstation *wk)
{
    unsigned long state = mouse_save_state;
    PIXEL *dst;
    PIXEL *save_w;
    short i, w, h;
    unsigned long wrap;

    dst = (PIXEL *) ((long) wk->screen.mfdb.address + (state & 0x00ffffffL));

    w = (state >> 28) & 0x0f;
    h = (state >> 24) & 0x0f;
    save_w = mouse_saved;
    wrap = wk->screen.wrap - (w + 1) * PIXEL_SIZE;
    wrap /= PIXEL_SIZE;     /* Change into pixel count */

    do
    {
        i = w;
        do
        {
            *dst++ = *save_w++;
        } while (--i >= 0);
        dst += wrap;
    } while (--h >= 0);

    mouse_save_state = 0;
}


static void draw_mouse(Workstation *wk, short x, short y)
{
    unsigned long state;
    PIXEL *dst;
    PIXEL *save_w;
    const unsigned short *mask_start;
    short w, h, shft;
    unsigned long wrap;

    x -= wk->mouse.hotspot.x;
    y -= wk->mouse.hotspot.y;
    w = 16;
    h = 16;

    mask_start = mouse_data;
    if (y < wk->screen.coordinates.min_y)
    {
        short ys;

        ys = wk->screen.coordinates.min_y - y;
        h -= ys;
        y = wk->screen.coordinates.min_y;
        mask_start += ys << 1;
    }
    if (y + h - 1 > wk->screen.coordinates.max_y)
    {
        h = wk->screen.coordinates.max_y - y + 1;
    }

    shft = 0;

    if (x < wk->screen.coordinates.min_x)
    {
        short xs;

        xs = wk->screen.coordinates.min_x - x;
        w -= xs;
        x = wk->screen.coordinates.min_x;
        shft = xs;
    }
    if (x + w - 1 > wk->screen.coordinates.max_x)
    {
        w = wk->screen.coordinates.max_x - x + 1;
    }

    if (w <= 0 || h <= 0)
    {
        mouse_save_state = 0;
        return;
    }

    wrap = wk->screen.wrap - w * PIXEL_SIZE;
    wrap /= PIXEL_SIZE;     /* Change into pixel count */
    dst = (PIXEL *) ((long) wk->screen.mfdb.address + y * (long) wk->screen.wrap + x * PIXEL_SIZE);

    w--;
    h--;
    state = 0;
    state |= (long) (w & 0x0f) << 28;
    state |= (long) (h & 0x0f) << 24;
    state |= (long) dst - (long) wk->screen.mfdb.address;
    mouse_save_state = state;

    save_w = mouse_saved;

    {
        unsigned short fg, bg;
        short i;

        do
        {
            bg = *mask_start++;
            bg <<= shft;
            fg = *mask_start++;
            fg <<= shft;
            i = w;
            do
            {
                *save_w++ = *dst;
                if (fg & 0x8000)
                    *dst = mouse_foreground;
                else if (bg & 0x8000)
                    *dst = mouse_background;
                dst++;
                bg <<= 1;
                fg <<= 1;
            } while (--i >= 0);
            dst += wrap;
        } while (--h >= 0);
    }
}


long CDECL c_mouse_draw(Workstation *wk, long x, long y, Mouse *mouse)
{
    unsigned long state;
    long mouseparm = (long) mouse;

    /* Need to mask x since it contains old operation in high bits (use that?) */
    x &= 0xffffL;

    state = mouse_save_state;

    if (mouseparm > 7)
    {                                   /* New mouse shape */
        long *pp = (long *)&wk->mouse.colour;

        pp = (long *)&wk->mouse.colour;
        if (*pp != mouse_old_colours)
        {
            Colour *global_palette;
            PIXEL *realp;

            mouse_old_colours = *pp;
            /* c_get_colours(wk, *pp, &mouse_foreground, &mouse_background); */
            global_palette = wk->screen.palette.colours;
            realp = (PIXEL *)&global_palette[wk->mouse.colour.foreground].real;
            mouse_foreground = *realp;
            realp = (PIXEL *)&global_palette[wk->mouse.colour.background].real;
            mouse_background = *realp;
        }

        if (!fix_shape)
            set_mouse_shape(mouse, mouse_data);
        return 0;
    }

    if (state && !no_restore && (mouseparm == 0 || mouseparm == 2 || mouseparm == 3 || mouseparm == 4))
    {                                   /* Move or Hide */
        hide_mouse(wk);
    }

    if (mouseparm == 0 || mouseparm == 3 || mouseparm == 4)
    {                                   /* Move or Show */
        draw_mouse(wk, (short)x, (short)y);
    }

    return 0;
}
