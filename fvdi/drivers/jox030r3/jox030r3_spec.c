/* 
 * A 16 bit mode specification/initialization file, by Johan Klockars.
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

#include "fvdi.h"
#include "driver.h"
#include "relocate.h"
#include "../bitplane/bitplane.h"
#include "string/memset.h"
#include "jox030r3.h"
extern Access *access;

short default_vdi_colors[256][3] = {
	{ 1000,1000,1000 },
	{    0,   0,   0 },
	{ 1000,   1,   0 },
	{    0,1000,   0 },
	{    0,   0,1000 },
	{    0,1000,1000 },
	{ 1000,1000,   0 },
	{ 1000,   0,1000 },
	{  733, 733, 733 },
	{  533, 533, 533 },
	{  667,   0,   0 },
	{    0, 667,   0 },
	{    0,   0, 667 },
	{    0, 667, 667 },
	{  667, 667,   0 },
	{  667,   0, 667 },
	{ 1000,1000,1000 },
	{  933, 933, 933 },
	{  867, 867, 867 },
	{  800, 800, 800 },
	{  733, 733, 733 },
	{  667, 667, 667 },
	{  600, 600, 600 },
	{  533, 533, 533 },
	{  467, 467, 467 },
	{  400, 400, 400 },
	{  333, 333, 333 },
	{  267, 267, 267 },
	{  200, 200, 200 },
	{  133, 133, 133 },
	{   67,  67,  67 },
	{    0,   0,   0 },
	{ 1000,   0,   0 },
	{ 1000,   0,  67 },
	{ 1000,   0, 133 },
	{ 1000,   0, 200 },
	{ 1000,   0, 267 },
	{ 1000,   0, 333 },
	{ 1000,   0, 400 },
	{ 1000,   0, 467 },
	{ 1000,   0, 533 },
	{ 1000,   0, 600 },
	{ 1000,   0, 667 },
	{ 1000,   0, 733 },
	{ 1000,   0, 800 },
	{ 1000,   0, 867 },
	{ 1000,   0, 933 },
	{ 1000,   0,1000 },
	{  933,   0,1000 },
	{  867,   0,1000 },
	{  800,   0,1000 },
	{  733,   0,1000 },
	{  667,   0,1000 },
	{  600,   0,1000 },
	{  533,   0,1000 },
	{  467,   0,1000 },
	{  400,   0,1000 },
	{  333,   0,1000 },
	{  267,   0,1000 },
	{  200,   0,1000 },
	{  133,   0,1000 },
	{   67,   0,1000 },
	{    0,   0,1000 },
	{    0,  67,1000 },
	{    0, 133,1000 },
	{    0, 200,1000 },
	{    0, 267,1000 },
	{    0, 333,1000 },
	{    0, 400,1000 },
	{    0, 467,1000 },
	{    0, 533,1000 },
	{    0, 600,1000 },
	{    0, 667,1000 },
	{    0, 733,1000 },
	{    0, 800,1000 },
	{    0, 867,1000 },
	{    0, 933,1000 },
	{    0,1000,1000 },
	{    0,1000, 933 },
	{    0,1000, 867 },
	{    0,1000, 800 },
	{    0,1000, 733 },
	{    0,1000, 667 },
	{    0,1000, 600 },
	{    0,1000, 533 },
	{    0,1000, 467 },
	{    0,1000, 400 },
	{    0,1000, 333 },
	{    0,1000, 267 },
	{    0,1000, 200 },
	{    0,1000, 133 },
	{    0,1000,  67 },
	{    0,1000,   0 },
	{   67,1000,   0 },
	{  133,1000,   0 },
	{  200,1000,   0 },
	{  267,1000,   0 },
	{  333,1000,   0 },
	{  400,1000,   0 },
	{  467,1000,   0 },
	{  533,1000,   0 },
	{  600,1000,   0 },
	{  667,1000,   0 },
	{  733,1000,   0 },
	{  800,1000,   0 },
	{  867,1000,   0 },
	{  933,1000,   0 },
	{ 1000,1000,   0 },
	{ 1000, 933,   0 },
	{ 1000, 867,   0 },
	{ 1000, 800,   0 },
	{ 1000, 733,   0 },
	{ 1000, 667,   0 },
	{ 1000, 600,   0 },
	{ 1000, 533,   0 },
	{ 1000, 467,   0 },
	{ 1000, 400,   0 },
	{ 1000, 333,   0 },
	{ 1000, 267,   0 },
	{ 1000, 200,   0 },
	{ 1000, 133,   0 },
	{ 1000,  67,   0 },
	{  733,   0,   0 },
	{  733,   0,  67 },
	{  733,   0, 133 },
	{  733,   0, 200 },
	{  733,   0, 267 },
	{  733,   0, 333 },
	{  733,   0, 400 },
	{  733,   0, 467 },
	{  733,   0, 533 },
	{  733,   0, 600 },
	{  733,   0, 667 },
	{  733,   0, 733 },
	{  667,   0, 733 },
	{  600,   0, 733 },
	{  533,   0, 733 },
	{  467,   0, 733 },
	{  400,   0, 733 },
	{  333,   0, 733 },
	{  267,   0, 733 },
	{  200,   0, 733 },
	{  133,   0, 733 },
	{   67,   0, 733 },
	{    0,   0, 733 },
	{    0,  67, 733 },
	{    0, 133, 733 },
	{    0, 200, 733 },
	{    0, 267, 733 },
	{    0, 333, 733 },
	{    0, 400, 733 },
	{    0, 467, 733 },
	{    0, 533, 733 },
	{    0, 600, 733 },
	{    0, 667, 733 },
	{    0, 733, 733 },
	{    0, 733, 667 },
	{    0, 733, 600 },
	{    0, 733, 533 },
	{    0, 733, 467 },
	{    0, 733, 400 },
	{    0, 733, 333 },
	{    0, 733, 267 },
	{    0, 733, 200 },
	{    0, 733, 133 },
	{    0, 733,  67 },
	{    0, 733,   0 },
	{   67, 733,   0 },
	{  133, 733,   0 },
	{  200, 733,   0 },
	{  267, 733,   0 },
	{  333, 733,   0 },
	{  400, 733,   0 },
	{  467, 733,   0 },
	{  533, 733,   0 },
	{  600, 733,   0 },
	{  667, 733,   0 },
	{  733, 733,   0 },
	{  733, 667,   0 },
	{  733, 600,   0 },
	{  733, 533,   0 },
	{  733, 467,   0 },
	{  733, 400,   0 },
	{  733, 333,   0 },
	{  733, 267,   0 },
	{  733, 200,   0 },
	{  733, 133,   0 },
	{  733,  67,   0 },
	{  467,   0,   0 },
	{  467,   0,  67 },
	{  467,   0, 133 },
	{  467,   0, 200 },
	{  467,   0, 267 },
	{  467,   0, 333 },
	{  467,   0, 400 },
	{  467,   0, 467 },
	{  400,   0, 467 },
	{  333,   0, 467 },
	{  267,   0, 467 },
	{  200,   0, 467 },
	{  133,   0, 467 },
	{   67,   0, 467 },
	{    0,   0, 467 },
	{    0,  67, 467 },
	{    0, 133, 467 },
	{    0, 200, 467 },
	{    0, 267, 467 },
	{    0, 333, 467 },
	{    0, 400, 467 },
	{    0, 467, 467 },
	{    0, 467, 400 },
	{    0, 467, 333 },
	{    0, 467, 267 },
	{    0, 467, 200 },
	{    0, 467, 133 },
	{    0, 467,  67 },
	{    0, 467,   0 },
	{   67, 467,   0 },
	{  133, 467,   0 },
	{  200, 467,   0 },
	{  267, 467,   0 },
	{  333, 467,   0 },
	{  400, 467,   0 },
	{  467, 467,   0 },
	{  467, 400,   0 },
	{  467, 333,   0 },
	{  467, 267,   0 },
	{  467, 200,   0 },
	{  467, 133,   0 },
	{  467,  67,   0 },
	{  267,   0,   0 },
	{  267,   0,  67 },
	{  267,   0, 133 },
	{  267,   0, 200 },
	{  267,   0, 267 },
	{  200,   0, 267 },
	{  133,   0, 267 },
	{   67,   0, 267 },
	{    0,   0, 267 },
	{    0,  67, 267 },
	{    0, 133, 267 },
	{    0, 200, 267 },
	{    0, 267, 267 },
	{    0, 267, 200 },
	{    0, 267, 133 },
	{    0, 267,  67 },
	{    0, 267,   0 },
	{   67, 267,   0 },
	{  133, 267,   0 },
	{  200, 267,   0 },
	{  267, 267,   0 },
	{  267, 200,   0 },
	{  267, 133,   0 },
	{  267,  67,   0 },
	{ 1000,1000,1000 },
	{    0,   0,   0 }
};

char red[] = {5};
char green[] = {5};
char blue[] = {5};
char none[] = {0};
static Mode const mode[1] = {
    {8, CHUNKY|CHECK_PREVIOUS, {red, green, blue, none, none, none}, 0,  2, 2, 1}
};

char driver_name[] = "V9958 driver by roberts7531";

long CDECL (*write_pixel_r)(Virtual *vwk, MFDB *mfdb, long x, long y, long colour) = c_write_pixel;
long CDECL (*read_pixel_r)(Virtual *vwk, MFDB *mfdb, long x, long y) = c_read_pixel;
long CDECL (*line_draw_r)(Virtual *vwk, long x1, long y1, long x2, long y2, long pattern, long colour, long mode) = c_line_draw;
long CDECL (*expand_area_r)(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour) = c_expand_area;
long CDECL (*fill_area_r)(Virtual *vwk, long x, long y, long w, long h, short *pattern, long colour, long mode, long interior_style) = c_fill_area;
long CDECL (*fill_poly_r)(Virtual *vwk, short points[], long n, short index[], long moves, short *pattern, long colour, long mode, long interior_style) = 0;
long CDECL (*blit_area_r)(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation) = c_blit_area;
long CDECL (*text_area_r)(Virtual *vwk, short *text, long length, long dst_x, long dst_y, short *offsets) = 0;
long CDECL (*mouse_draw_r)(Workstation *wk, long x, long y, Mouse *mouse) = c_mouse_draw;

long CDECL (*get_colour_r)(Virtual *vwk, long colour) = c_get_colour;
void CDECL (*get_colours_r)(Virtual *vwk, long colour, unsigned long *foreground, unsigned long *background) = 0;
void CDECL (*set_colours_r)(Virtual *vwk, long start, long entries, unsigned short *requested, Colour palette[]) = c_set_colours;

long wk_extend = 0;
short accel_s = 0;
short accel_c = A_SET_PAL | A_GET_COL | A_SET_PIX | A_GET_PIX | A_MOUSE | A_FILL |A_BLIT | A_LINE | A_EXPAND;

const Mode *graphics_mode = &mode[0];

short shadow = 0;
short fix_shape = 0;
short no_restore = 0;



static Option const options[] = {
    {"debug",      { &debug }, 2 },              /* debug, turn on debugging aids */
};


/*
 * Handle any driver specific parameters
 */
long check_token(char *token, const char **ptr)
{
    int i;
    int normal;
    char *xtoken;

    xtoken = token;
    switch (token[0])
    {
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
    for (i = 0; i < (int)(sizeof(options) / sizeof(Option)); i++)
    {
        if (access->funcs.equal(xtoken, options[i].name))
        {
            switch (options[i].type)
            {
            case -1:                /* Function call */
                return (options[i].var.func) (ptr);
            case 0:                 /* Default 1, set to 0 */
                *options[i].var.s = 1 - normal;
                return 1;
            case 1:                 /* Default 0, set to 1 */
                *options[i].var.s = normal;
                return 1;
            case 2:                 /* Increase */
                *options[i].var.s += -1 + 2 * normal;
                return 1;
            case 3:
                if ((*ptr = access->funcs.skip_space(*ptr)) == NULL)
                {
                    ;               /* *********** Error, somehow */
                }
                *ptr = access->funcs.get_token(*ptr, token, 80);
                *options[i].var.s = token[0];
                return 1;
            }
        }
    }

    return 0;
}




/*
 * Do whatever setup work might be necessary on boot up
 * and which couldn't be done directly while loading.
 * Supplied is the default fVDI virtual workstation.
 */
 
 /* I have no idea what most of this does, just leave it as it is*/
long CDECL initialize(Virtual *vwk)
{
	access->funcs.puts("Init\r\n");
    Workstation *wk;
	char *buf;
	int old_palette_size;
	Colour *old_palette_colours;
	int fast_w_bytes;
	
	vwk = me->default_vwk;	/* This is what we're interested in */	
	wk = vwk->real_address;

      

	/* update the settings */
	wk->screen.mfdb.width = 1024;
	wk->screen.mfdb.height = 768;
	wk->screen.mfdb.bitplanes = 8;

	
	wk->screen.mfdb.wdwidth = (wk->screen.mfdb.width + 15) / 16;
	wk->screen.wrap = wk->screen.mfdb.width * (wk->screen.mfdb.bitplanes / 8);

	wk->screen.coordinates.max_x = wk->screen.mfdb.width - 1;
	wk->screen.coordinates.max_y = wk->screen.mfdb.height - 1;

	wk->screen.look_up_table = 0;			/* Was 1 (???)	Shouldn't be needed (graphics_mode) */
	wk->screen.mfdb.standard = 0;

	if (wk->screen.pixel.width > 0)        /* Starts out as screen width */
		wk->screen.pixel.width =  (wk->screen.pixel.width * 1000L) / wk->screen.mfdb.width;
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
        access->funcs.copymem(loaded_palette, default_vdi_colors, 256 * 3 * sizeof(short));

    wk->screen.palette.colours = access->funcs.malloc(256L * sizeof(Colour), 3); 
    wk->screen.palette.size = 256;
   
	device.byte_width = wk->screen.wrap;
	device.address = wk->screen.mfdb.address;

	
    return 1;
}

/*
 *
 */
long CDECL setup(long type, long value)
{
    long ret;

    ret = -1;
    switch (type)
    {
    case Q_NAME:
        ret = (long) driver_name;
        break;
    case S_DRVOPTION:
        ret = tokenize((char *) value);
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
	access->funcs.puts("Openwk\r\n");
    Workstation *wk;

	vwk = me->default_vwk;  /* This is what we're interested in */
	wk = vwk->real_address;



	wk->screen.mfdb.width = 1024;
	wk->screen.mfdb.height = 768;


	wk->screen.mfdb.bitplanes = 8;


	device.address = wk->screen.mfdb.address;
	wk->screen.mfdb.wdwidth = (wk->screen.mfdb.width + 15) / 16;
	wk->screen.wrap = wk->screen.mfdb.width * (wk->screen.mfdb.bitplanes / 8);

	wk->screen.coordinates.max_x = wk->screen.mfdb.width - 1;
	wk->screen.coordinates.max_y = (wk->screen.mfdb.height & 0xfff0) - 1;

	wk->screen.look_up_table = 0;			/* Was 1 (???)	Shouldn't be needed (graphics_mode) */
	wk->screen.mfdb.standard = 0;

	if (wk->screen.pixel.width > 0)			/* Starts out as screen width */
		wk->screen.pixel.width = (wk->screen.pixel.width * 1000L) / wk->screen.mfdb.width;
	else								   /*	or fixed DPI (negative) */
		wk->screen.pixel.width = 25400 / -wk->screen.pixel.width;

	if (wk->screen.pixel.height > 0)		/* Starts out as screen height */
		wk->screen.pixel.height = (wk->screen.pixel.height * 1000L) / wk->screen.mfdb.height;
	else									/*	 or fixed DPI (negative) */
		wk->screen.pixel.height = 25400 / -wk->screen.pixel.height;
	setTextModeMode(TEXTMODE_MODE_OFF);
	setVideoMode(0,0);
    	c_initialize_palette(vwk, 0, wk->screen.palette.size, default_vdi_colors, wk->screen.palette.colours);

	
    return 0;
}

/*
 * 'Deinitialize'
 */
void CDECL clswk(Virtual *vwk)
{
    (void) vwk;
}
