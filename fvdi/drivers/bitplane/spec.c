/*
 * fVDI device driver specific setup
 *
 * Copyright 1998-2002, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

#include "fvdi.h"
#include "relocate.h"

#include "os.h"
#include "driver.h"
#include "bitplane.h"
#include "string/memset.h"

#if 0
#define FAST		/* Write in FastRAM buffer */
#define BOTH		/* Write in both FastRAM and on screen */
#else
#undef FAST
#undef BOTH
#endif


static char const red[] = { 6 };
static char const green[] = { 6 };
static char const blue[] = { 6 };
static char const none[] = { 0 };

static Mode const mode[1] = {
    { 1, CHECK_PREVIOUS, { red, green, blue, none, none, none}, 0, 0, 1, 1 }
};

char driver_name[] = "Bitplane (shadow)";

long CDECL(*write_pixel_r) (Virtual *vwk, MFDB * mfdb, long x, long y, long colour) = c_write_pixel;
long CDECL(*read_pixel_r) (Virtual *vwk, MFDB * mfdb, long x, long y) = c_read_pixel;
long CDECL(*line_draw_r) (Virtual *vwk, long x1, long y1, long x2, long y2, long pattern, long colour, long mode) = c_line_draw;
long CDECL(*expand_area_r) (Virtual *vwk, MFDB * src, long src_x, long src_y, MFDB * dst, long dst_x, long dst_y, long w, long h, long operation, long colour) = c_expand_area;
long CDECL(*fill_area_r) (Virtual *vwk, long x, long y, long w, long h, short *pattern, long colour, long mode, long interior_style) = c_fill_area;
long CDECL(*fill_poly_r) (Virtual *vwk, short points[], long n, short index[], long moves, short *pattern, long colour, long mode, long interior_style) = 0;
long CDECL(*blit_area_r) (Virtual *vwk, MFDB * src, long src_x, long src_y, MFDB * dst, long dst_x, long dst_y, long w, long h, long operation) = c_blit_area;
long CDECL(*text_area_r) (Virtual *vwk, short *text, long length, long dst_x, long dst_y, short *offsets) = 0;
long CDECL(*mouse_draw_r) (Workstation *wk, long x, long y, Mouse * mouse) = c_mouse_draw;

long CDECL(*get_colour_r) (Virtual *vwk, long colour) = c_get_colour;
void CDECL(*get_colours_r) (Virtual *vwk, long colour, unsigned long *foreground, unsigned long *background) = 0;
void CDECL(*set_colours_r) (Virtual *vwk, long start, long entries, unsigned short *requested, Colour palette[]) = c_set_colours;

long wk_extend = 0;
short accel_s = 0;
short accel_c = A_SET_PAL | A_GET_COL | A_SET_PIX | A_GET_PIX | A_BLIT | A_FILL | A_EXPAND | A_LINE | A_MOUSE;

const Mode *graphics_mode = &mode[0];

short shadow = 0;
short fix_shape = 0;
short no_restore = 0;
short depth = 0;


/*
 * Handle any driver specific parameters
 */
long check_token(char *token, const char **ptr)
{
	(void) ptr;
    if (access->funcs.equal(token, "shadow"))
        shadow = 1;
    else if (access->funcs.equal(token, "fixshape"))
        fix_shape = 1;
    else if (access->funcs.equal(token, "norestore"))
        no_restore = 1;
    else
        return 0;
    return 1;
}


/*
 * Do whatever setup work might be necessary on boot up
 * and which couldn't be done directly while loading.
 * Supplied is the default fVDI virtual workstation.
 */
long initialize(Virtual *vwk)
{
    Workstation *wk;
#ifdef FAST
    char *buf;
    int fast_w_bytes;
#endif

    debug = access->funcs.misc(0, 1, 0);

    vwk = me->default_vwk;              /* This is what we're interested in */
    wk = vwk->real_address;

#ifdef FAST
    if (shadow)
    {
#if 0                                   /* It's not clear that this is a good idea */
        fast_w_bytes = (wk->screen.wrap + 15) & 0xfffffff0;
#else
        fast_w_bytes = wk->screen.wrap;
#endif
        buf = (char *) access->funcs.malloc(fast_w_bytes * wk->screen.mfdb.height + 255, 1);
        if (buf)
        {
            wk->screen.shadow.buffer = buf;
            wk->screen.shadow.address = (void *) (((long) buf + 255) & 0xffffff00);
            wk->screen.shadow.wrap = fast_w_bytes;
        } else
        {
            access->funcs.error("Can't allocate FastRAM!", 0);
            wk->screen.shadow.buffer = 0;
            wk->screen.shadow.address = 0;
        }
#ifndef BOTH
        wk->screen.mfdb.address = wk->screen.shadow.address;
#endif
    }
#endif
    if (!wk->screen.shadow.address)
        driver_name[10] = 0;

    if (wk->screen.pixel.width > 0)     /* Starts out as screen width */
        wk->screen.pixel.width = (wk->screen.pixel.width * 1000L) / wk->screen.mfdb.width;
    else                                /*   or fixed DPI (negative) */
        wk->screen.pixel.width = 25400 / -wk->screen.pixel.width;
    if (wk->screen.pixel.height > 0)    /* Starts out as screen height */
        wk->screen.pixel.height = (wk->screen.pixel.height * 1000L) / wk->screen.mfdb.height;
    else                                /*   or fixed DPI (negative) */
        wk->screen.pixel.height = 25400 / -wk->screen.pixel.height;

#if 0
    device.format = mode[depth].format;
    device.clut = mode[depth].clut;
    device.bit_depth = mode[depth].bpp;
    device.dummy1 = (1L << mode[depth].bpp) >> 16;
    device.colours = (1L << mode[depth].bpp) & 0xffff;
#endif
    device.byte_width = wk->screen.wrap;
    device.address = wk->screen.mfdb.address;
#if 0
    device.bits.red = mode[depth].bits.red[0];
    device.bits.green = mode[depth].bits.green[0];
    device.bits.blue = mode[depth].bits.blue[0];
    device.bits.alpha = mode[depth].bits.alpha[0];
    device.bits.genlock = mode[depth].bits.genlock[0];
    device.bits.unused = mode[depth].bits.unused[0];
    device.bits.organization = mode[depth].org;
    device.dummy2 = 0;
    if (device.clut == 2)
    {
        for(i = 0; i < mode[depth].bits.red[0]; i++)
            device.bitnumber.red[i] = mode[depth].bits.red[i + 1];
        for(i = mode[depth].bits.red[0]; i < 16; i++)
            device.bitnumber.red[i] = -1;       /* Not used */
        for(i = 0; i < mode[depth].bits.green[0]; i++)
            device.bitnumber.green[i] = mode[depth].bits.green[i + 1];
        for(i = mode[depth].bits.green[0]; i < 16; i++)
            device.bitnumber.green[i] = -1;     /* Not used */
        for(i = 0; i < mode[depth].bits.blue[0]; i++)
            device.bitnumber.blue[i] = mode[depth].bits.blue[i + 1];
        for(i = mode[depth].bits.blue[0]; i < 16; i++)
            device.bitnumber.blue[i] = -1;      /* Not used */
        for(i = 0; i < mode[depth].bits.alpha[0]; i++)
            device.bitnumber.alpha[i] = mode[depth].bits.alpha[i + 1];
        for(i = mode[depth].bits.alpha[0]; i < 16; i++)
            device.bitnumber.alpha[i] = -1;     /* Not used */
        for(i = 0; i < mode[depth].bits.genlock[0]; i++)
            device.bitnumber.genlock[i] = mode[depth].bits.genlock[i + 1];
        for(i = mode[depth].bits.genlock[0]; i < 16; i++)
            device.bitnumber.genlock[i] = -1;   /* Not used */
        for(i = 0; i < mode[depth].bits.unused[0]; i++)
            device.bitnumber.unused[i] = mode[depth].bits.unused[i + 1];
        for(i = mode[depth].bits.unused[0]; i < 32; i++)
            device.bitnumber.unused[i] = -1;    /* Not used */
        for(i = 0; i < 144; i++)
            device.reserved[i] = 0;
    } else
    {
        for(i = 0; i < sizeof(tos_colours); i++)
            device.vdi2pix[i] = tos_colours[i];
        if (mode[depth].bpp == 8)
        {
            for (; i < 255; i++)
                device.vdi2pix[i] = i;
            device.vdi2pix[255] = 15;
        } else
        {
            for(; i < 256; i++)
                device.vdi2pix[i] = 0;
        }
    }
#endif

    PRINTF(("%dx%dx%d screen at $%08lx\n", wk->screen.mfdb.width, wk->screen.mfdb.height, wk->screen.mfdb.bitplanes,
            (long) wk->screen.mfdb.address));

    return 1;
}

/*
 *
 */
long setup(long type, long value)
{
    long ret;

    (void) value;
    ret = -1;
    switch (type)
    {
#if 0
    case S_SCREEN:
        if (value != -1)
            old_screen = value;
        ret = old_screen;
        break;
    case S_AESBUF:
        if (value != -1)
            aes_buffer = value;
        ret = aes_buffer;
        break;
    case S_DOBLIT:
        if (value != -1)
            blit_fall_back = value;
        ret = blit_fall_back;
        break;
    case S_CACHEIMG:
        if (value != -1)
            cache_img = value;
        ret = cache_img;
        break;
#endif
    case Q_NAME:
        ret = (long) driver_name;
        break;
    }

    return ret;
}

/*
 * Initialize according to parameters (boot and sent).
 * Create new (or use old) Workstation and default Virtual.
 * Supplied is the default fVDI virtual workstation.
 */
Virtual *CDECL opnwk(Virtual *vwk)
{
    (void) vwk;
    me->default_vwk->real_address->screen.mfdb.address = (void *) Physbase();

    return 0;
}

/*
 * 'Deinitialize'
 */
void CDECL clswk(Virtual *vwk)
{
    (void) vwk;
}
