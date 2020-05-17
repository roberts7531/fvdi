/*
 * firebee.h - FireBee specific functions
 * This is part of the FireBee driver for fVDI
 * Glued by Vincent Riviere
 * Stirred, mixed and shaken by Markus Fröschle
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

#ifndef _FIREBEE_H_
#define _FIREBEE_H_

#include "fvdi.h"
#include <stdlib.h>
#include <stdbool.h>

/* from firebee.c */
void fbee_set_clock(int clock);
void fbee_set_screen(void *addr);
void fbee_set_panning(unsigned short *mem);

long c_write_pixel(Virtual *vwk, MFDB *mfdb, long x, long y, long colour);
long c_read_pixel(Virtual *vwk, MFDB *mfdb, long x, long y);
long c_line_draw(Virtual *vwk, long x1, long y1, long x2, long y2, long pattern, long colour, long mode);
long c_expand_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour);
long c_fill_area(Virtual *vwk, long x, long y, long w, long h, short *pattern, long colour, long mode, long interior_style);
long c_fill_polygon(Virtual *vwk, short points[], long n, short index[], long moves, short *pattern, long colour, long mode, long interior_style);
long c_blit_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation);
long c_text_area(Virtual *vwk, short *text, long length, long dst_x, long dst_y, short *offsets);
long c_mouse_draw(Workstation *wk, long x, long y, Mouse *mouse);

long c_get_colour(Virtual *vwk, long colour);
void c_set_colours(Virtual *vwk, long start, long entries, unsigned short *requested, Colour palette[]);

#endif /* _FIREBEE_H_ */
