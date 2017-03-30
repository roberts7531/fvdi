/*
 * sag_spec.c - Specification/initialization file
 * This is part of the SAGA driver for fVDI
 * Derived from Johan Klockars's example in ../16_bit/16b_spec.c
 *
 * Copyright (C) 2017 Vincent Riviere
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "fvdi.h"
#include "relocate.h"
#include "radeon.h"
#include "video.h"
#include <os.h>

#include "driver.h"

char r_16[] = {5, 11, 12, 13, 14, 15};
char g_16[] = {6,  5,  6,  7,  8,  9, 10};
char b_16[] = {5,  0,  1,  2,  3,  4};
char none[] = {0};

Mode mode[1] =
{{16, CHUNKY | CHECK_PREVIOUS | TRUE_COLOUR, {r_16, g_16, b_16, none, none, none}, 0,  2, 2, 1}};

extern Device device;

char driver_name[] = "FBee";

struct
{
    short used; /* Whether the mode option was used or not. */
    short width;
    short height;
    short bpp;
    short freq;
} resolution = {0, 640, 480, 16, 60};

struct {
    short width;
    short height;
} pixel;

extern Driver *me;
extern Access *access;

extern short *loaded_palette;

extern short colours[][3];
extern void CDECL initialize_palette(Virtual *vwk, long start, long entries, short requested[][3], Colour palette[]);
extern void CDECL c_initialize_palette(Virtual *vwk, long start, long entries, short requested[][3], Colour palette[]);
extern void *c_set_colours;		/* Just to check if the routine is available */

extern long tokenize(const char *ptr);

extern void *c_write_pixel;
extern void *c_read_pixel;
extern void *c_line_draw;
extern void *c_expand_area;
extern void *c_fill_area;
extern void *c_blit_area;
extern void *c_mouse_draw;
extern void *c_set_colours;
extern void *c_get_colour;

void *write_pixel_r = &c_write_pixel;
void *read_pixel_r  = &c_read_pixel;
void *line_draw_r   = &c_line_draw;
void *expand_area_r = &c_expand_area;
void *fill_area_r   = &c_fill_area;
void *fill_poly_r   = 0;
void *blit_area_r   = &c_blit_area;
void *text_area_r   = 0;
void *mouse_draw_r  = &c_mouse_draw;
void *set_colours_r = &c_set_colours;
void *get_colours_r = 0;
void *get_colour_r  = &c_get_colour;

static void fbee_puts(const char* message)
{
    access->funcs.puts("FBee: ");
    access->funcs.puts(message);
    access->funcs.puts("\x0d\x0a");
}

static void panic(const char* message)
{
    /* Unfortunately, fVDI can't recover from driver initialization failure.
     * So we just wait forever. */
    fbee_puts(message);
    for(;;);
}

long wk_extend = 0;

short accel_s = 0;
short accel_c = A_SET_PAL | A_GET_COL | A_SET_PIX | A_GET_PIX | A_BLIT | A_FILL | A_EXPAND | A_LINE | A_MOUSE;

Mode *graphics_mode = &mode[0];

short debug = 0;

long set_mode(const char **ptr);

Option options[] = {
    {"debug",      &debug,             2},  /* debug, turn on debugging aids */
    {"mode",       set_mode,          -1},  /* mode WIDTHxHEIGHTxDEPTH@FREQ */
};

char *get_num(char *token, short *num)
{
    char buf[10], c;
    int i;

    *num = -1;
    if (!*token)
        return token;
    for(i = 0; i < 10; i ++) {
        c = buf[i] = *token++;
        if ((c < '0') || (c > '9'))
            break;
    }
    if (i > 5)
        return token;

    buf[i] = '\0';
    *num = access->funcs.atol(buf);
    return token;
}

int set_bpp(int bpp)
{
    switch (bpp) {
        case -1:
        case 16:
            graphics_mode = &mode[0];
            break;
        default:
            panic("Unsupported BPP.");
            break;
    }

    return bpp;
}

long set_mode(const char **ptr)
{
    char token[80], *tokenptr;

    if (!(*ptr = access->funcs.skip_space(*ptr)))
        ;		/* *********** Error, somehow */
    *ptr = access->funcs.get_token(*ptr, token, 80);

    tokenptr = token;
    tokenptr = get_num(tokenptr, &resolution.width);
    tokenptr = get_num(tokenptr, &resolution.height);
    tokenptr = get_num(tokenptr, &resolution.bpp);
    tokenptr = get_num(tokenptr, &resolution.freq);

    resolution.used = 1;

    resolution.bpp = set_bpp(resolution.bpp);

    return 1;
}

/*
 * Handle any driver specific parameters
 */
long check_token(char *token, const char **ptr)
{
    int i;
    int normal;
    char *xtoken;

    xtoken = token;
    switch (token[0]) {
        case '+':
            xtoken++;
            normal = 1;
            break;
        case '-':
            xtoken++;
            normal = 0;
            break;
        default:
            normal = 1;
            break;
    }
    for(i = 0; i < sizeof(options) / sizeof(Option); i++) {
        if (access->funcs.equal(xtoken, options[i].name)) {
            switch (options[i].type) {
                case -1:     /* Function call */
                    return ((long (*)(const char **))options[i].varfunc)(ptr);
                case 0:      /* Default 1, set to 0 */
                    *(short *)options[i].varfunc = 1 - normal;
                    return 1;
                case 1:     /* Default 0, set to 1 */
                    *(short *)options[i].varfunc = normal;
                    return 1;
                case 2:     /* Increase */
                    *(short *)options[i].varfunc += -1 + 2 * normal;
                    return 1;
                case 3:
                    if (!(*ptr = access->funcs.skip_space(*ptr)))
                        ;  /* *********** Error, somehow */
                    *ptr = access->funcs.get_token(*ptr, token, 80);
                    *(short *)options[i].varfunc = token[0];
                    return 1;
            }
        }
    }

    return 0;
}

static struct ModeInfo *mi;
static UBYTE *screen_address;

/* Fix all ModeInfo with PLL data */
static void fix_all_mode_info(void)
{
    int i;

    for (i = 0; i < modeline_vesa_entries; i++) {
        struct ModeInfo *mi = &modeline_vesa_entry[i];
        fbee_fix_mode(mi);
    }
}

/* Find ModeInfo according to requested video mode */
static struct ModeInfo *find_mode_info(void)
{
    int i;

    for (i = 0; i < modeline_vesa_entries; i++) {
        struct ModeInfo *p = &modeline_vesa_entry[i];
        if (p->Width == resolution.width && p->Height == resolution.height)
            return p;
    }

    panic("Requested video mode mode not available.");
    return NULL;
}

/* Allocate screen buffer */
UBYTE *fbee_alloc_vram(UWORD width, UWORD height)
{
    ULONG buffer;
    ULONG vram_size = (ULONG)width * height * sizeof(short);
    const int alignment = 32;

    /* FireBee screen buffers live in ST RAM */
    buffer = (ULONG) Mxalloc(vram_size + alignment - 1, 0);
    if (!buffer)
        panic("Mxalloc() failed to allocate screen buffer.");

    buffer = (buffer + alignment - 1) & -alignment;

    return (UBYTE *) buffer + FIREBEE_VRAM_PHYS_OFFSET;
}

/*
 * Do whatever setup work might be necessary on boot up
 * and which couldn't be done directly while loading.
 * Supplied is the default fVDI virtual workstation.
 */
long CDECL initialize(Virtual *vwk)
{
    Workstation *wk;
    int old_palette_size;
    Colour *old_palette_colours;

    /* Display startup banner */
    access->funcs.puts("\r\n");
    access->funcs.puts("\ep FireBee driver for fVDI \eq\r\n");
    access->funcs.puts("\xbd 2017 Markus Fr\x94schle\r\n");
    access->funcs.puts("Free Software distributed under GPLv2\r\n");
    access->funcs.puts("\r\n");

    /* Initialize the RTG card with the requested video mode */
    fix_all_mode_info();
    mi = find_mode_info();
    screen_address = fbee_alloc_vram(resolution.width, resolution.height);

    vwk = me->default_vwk;	/* This is what we're interested in */
    wk = vwk->real_address;

    wk->screen.look_up_table = 0;			/* Was 1 (???)  Shouldn't be needed (graphics_mode) */
    wk->screen.mfdb.standard = 0;
    if (wk->screen.pixel.width > 0)        /* Starts out as screen width */
        wk->screen.pixel.width = (wk->screen.pixel.width * 1000L) / wk->screen.mfdb.width;
    else                                   /*   or fixed DPI (negative) */
        wk->screen.pixel.width = 25400 / -wk->screen.pixel.width;
    if (wk->screen.pixel.height > 0)        /* Starts out as screen height */
        wk->screen.pixel.height = (wk->screen.pixel.height * 1000L) / wk->screen.mfdb.height;
    else                                    /*   or fixed DPI (negative) */
        wk->screen.pixel.height = 25400 / -wk->screen.pixel.height;


    /*
     * This code needs more work.
     * Especially if there was no VDI started since before.
     */

    if (loaded_palette)
        access->funcs.copymem(loaded_palette, colours, 256 * 3 * sizeof(short));
    if ((old_palette_size = wk->screen.palette.size) != 256) {	/* Started from different graphics mode? */
        old_palette_colours = wk->screen.palette.colours;
        wk->screen.palette.colours = (Colour *)access->funcs.malloc(256L * sizeof(Colour), 3);	/* Assume malloc won't fail. */
        if (wk->screen.palette.colours) {
            wk->screen.palette.size = 256;
            if (old_palette_colours)
                access->funcs.free(old_palette_colours);	/* Release old (small) palette (a workaround) */
        } else
            wk->screen.palette.colours = old_palette_colours;
    }
    c_initialize_palette(vwk, 0, wk->screen.palette.size, colours, wk->screen.palette.colours);

    device.byte_width = wk->screen.wrap;
    device.address = wk->screen.mfdb.address;

    pixel.width = wk->screen.pixel.width;
    pixel.height = wk->screen.pixel.height;

    return 1;
}

/*
 *
 */
long CDECL setup(long type, long value)
{
    long ret;

    ret = -1;
    switch(type) {
        case Q_NAME:
            ret = (long)driver_name;
            break;
        case S_DRVOPTION:
            ret = tokenize((char *)value);
            break;
    }

    return ret;
}

/*
 * Initialize according to parameters (boot and sent).
 * Create new (or use old) Workstation and default Virtual.
 * Supplied is the default fVDI virtual workstation.
 */
Virtual* CDECL opnwk(Virtual *vwk)
{
    Workstation *wk;
    vwk = me->default_vwk;  /* This is what we're interested in */
    wk = vwk->real_address;

    /* Switch to SAGA screen */
    fbee_set_clock(mi);
    fbee_set_modeline(mi, SAGA_VIDEO_FORMAT_RGB16);
    fbee_set_panning(screen_address);

    /* update the settings */
    wk->screen.mfdb.width = mi->Width;
    wk->screen.mfdb.height = mi->Height;
    wk->screen.mfdb.bitplanes = resolution.bpp;

    /*
     * Some things need to be changed from the
     * default workstation settings.
     */
    wk->screen.mfdb.address = (short *)screen_address;
    wk->screen.mfdb.wdwidth = (wk->screen.mfdb.width + 15) / 16;
    wk->screen.wrap = wk->screen.mfdb.width * (wk->screen.mfdb.bitplanes / 8);

    wk->screen.coordinates.max_x = wk->screen.mfdb.width - 1;
    wk->screen.coordinates.max_y = wk->screen.mfdb.height - 1;

    wk->screen.look_up_table = 0;			/* Was 1 (???)	Shouldn't be needed (graphics_mode) */
    wk->screen.mfdb.standard = 0;

    if (pixel.width > 0)			/* Starts out as screen width */
        wk->screen.pixel.width = (pixel.width * 1000L) / wk->screen.mfdb.width;
    else								   /*	or fixed DPI (negative) */
        wk->screen.pixel.width = 25400 / -pixel.width;

    if (pixel.height > 0)		/* Starts out as screen height */
        wk->screen.pixel.height = (pixel.height * 1000L) / wk->screen.mfdb.height;
    else									/*	 or fixed DPI (negative) */
        wk->screen.pixel.height = 25400 / -pixel.height;

    return 0;
}

/*
 * 'Deinitialize'
 */
void CDECL clswk(Virtual *vwk)
{
}
