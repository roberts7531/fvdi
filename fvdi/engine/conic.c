/*
 * fVDI circle/ellipse/pie/arc code
 *
 * Copyright 1999/2001-2003, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 *
 * This is extracted and modified from code with an
 * original copyright as follows.
 */

/*************************************************************************
**       Copyright 1999, Caldera Thin Clients, Inc.                     **
**       This software is licenced under the GNU Public License.        **
**       Please see LICENSE.TXT for further information.                **
**                                                                      **
**                  Historical Copyright                                **
**                                                                      **
**  Copyright (c) 1987, Digital Research, Inc. All Rights Reserved.     **
**  The Software Code contained in this listing is proprietary to       **
**  Digital Research, Inc., Monterey, California and is covered by U.S. **
**  and other copyright protection.  Unauthorized copying, adaptation,  **
**  distribution, use or display is prohibited and may be subject to    **
**  civil and criminal penalties.  Disclosure to others is prohibited.  **
**  For the terms and conditions of software code use refer to the      **
**  appropriate Digital Research License Agreement.                     **
**                                                                      **
**************************************************************************/


#include "fvdi.h"
#include "relocate.h"
#include "utility.h"
#include "function.h"
#include "globals.h"

#define MAX_ARC_CT 256

#ifdef __GNUC__
#define SMUL_DIV(x,y,z) ((short)(((short)(x)*(long)((short)(y)))/(short)(z)))
#else
#ifdef __PUREC__
#define SMUL_DIV(x,y,z) ((short)(((x)*(long)(y))/(z)))
#else
int SMUL_DIV(int, int, int);   /* d0d1d0d2 */
#pragma inline d0 = SMUL_DIV(d0, d1, d2) { "c1c181c2"; }
#endif
#endif


void ellipsearc(Virtual * vwk, long gdp_code, long xc, long yc, long xrad, long yrad, long beg_ang, long end_ang);
void rounded_box(Virtual * vwk, long gdp_code, short *coords);


static void clc_arc(Virtual *vwk, long gdp_code, long xc, long yc, long xrad, long yrad,
             long beg_ang, long end_ang, long del_ang, long n_steps,
             Fgbg fill_colour, Fgbg border_colour,
             short *pattern, short *points, long mode, long interior_style)
{
    short i, j, start, angle;

    /* if (vwk->clip.on) */
    {
        if (((xc + xrad) < vwk->clip.rectangle.x1) ||
            ((xc - xrad) > vwk->clip.rectangle.x2) ||
            ((yc + yrad) < vwk->clip.rectangle.y1) ||
            ((yc - yrad) > vwk->clip.rectangle.y2))
            return;
    }
    start = angle = beg_ang;

    *points++ = SMUL_DIV(Icos(angle), xrad, 32767) + xc;
    *points++ = yc - SMUL_DIV(Isin(angle), yrad, 32767);

    for (i = 1, j = 2; i < n_steps; i++, j += 2)
    {
        angle = SMUL_DIV(del_ang, i, n_steps) + start;
        *points++ = SMUL_DIV(Icos(angle), xrad, 32767) + xc;
        *points++ = yc - SMUL_DIV(Isin(angle), yrad, 32767);
    }
    angle = end_ang;

    *points++ = SMUL_DIV(Icos(angle), xrad, 32767) + xc;
    *points++ = yc - SMUL_DIV(Isin(angle), yrad, 32767);

    /*
     * If pie wedge, draw to center and then close.
     * If arc or circle, do nothing because loop should close circle.
     */

    if ((gdp_code == 3) || (gdp_code == 7))
    {
        /* Pie wedge */
        n_steps++;
        *points++ = xc;
        *points++ = yc;
    }

    if ((gdp_code == 2) || (gdp_code == 6)) /* Open arc */
    {
        c_pline(vwk, n_steps + 1, border_colour, points - (n_steps + 1) * 2);
    }
    else
    {
        fill_poly(vwk, points - (n_steps + 1) * 2, n_steps + 1,
                  fill_colour, pattern, points, mode, interior_style);
        if (gdp_code != 4 && gdp_code != 5) /* TOS VDI doesn't draw the perimeter for v_circle() and v_ellipse() */
            if (vwk->fill.perimeter)
                c_pline(vwk, n_steps + 1, border_colour, points - (n_steps + 1) * 2);
    }
}


static void col_pat(Virtual *vwk, Fgbg *fill_colour, Fgbg *border_colour, short **pattern)
{
    short interior;

    interior = vwk->fill.interior;

    *border_colour = vwk->fill.colour;
    if (interior)
    {
        *fill_colour = vwk->fill.colour;
    } else
    {
        fill_colour->background = vwk->fill.colour.foreground;
        fill_colour->foreground = vwk->fill.colour.background;
    }

    if (interior == 4)
    {
        *pattern = vwk->fill.user.pattern.in_use;
    } else
    {
        *pattern = pattern_ptrs[interior];
        if (interior & 2)               /* interior 2 or 3 */
            *pattern += (vwk->fill.style - 1) * 16;
    }
}


static int clc_nsteps(long xrad, long yrad)
{
    long n_steps;

    if (xrad > yrad)
        n_steps = xrad;
    else
        n_steps = yrad;

    n_steps = (n_steps * arc_split) >> 16;

    if (n_steps < arc_min)
        n_steps = arc_min;
    else if (n_steps > arc_max)
        n_steps = arc_max;

    return (int)n_steps;
}


void ellipsearc(Virtual *vwk, long gdp_code,
                long xc, long yc, long xrad, long yrad, long beg_ang, long end_ang)
{
    int del_ang, n_steps;
    short *points, *pattern;
    Fgbg fill_colour, border_colour;
    long interior_style;

    del_ang = (int)(end_ang - beg_ang);
    if (del_ang <= 0)
        del_ang += 3600;

    n_steps = clc_nsteps(xrad, yrad);
    n_steps = SMUL_DIV(del_ang, n_steps, 3600);
    if (n_steps == 0)
        return;

    if ((points = (short *) allocate_block(0)) == NULL)
        return;

    pattern = 0;
    interior_style = 0;
    border_colour = vwk->line.colour;
    if (gdp_code == 7 || gdp_code == 5)
    {
        col_pat(vwk, &fill_colour, &border_colour, &pattern);
        interior_style = ((long) vwk->fill.interior << 16) | (vwk->fill.style & 0xffffL);
    }

    /* Dummy fill colour, pattern and interior style since not filled */
    clc_arc(vwk, gdp_code, xc, yc, xrad, yrad, beg_ang, end_ang, del_ang,
            n_steps, fill_colour, border_colour, pattern, points, vwk->mode, interior_style);

    free_block(points);
}


void rounded_box(Virtual *vwk, long gdp_code, short *coords)
/* long x1, long y1, long x2, long y2) */
{
    short i, j;
    short rdeltax, rdeltay;
    short xc, yc, xrad, yrad;
    short x1, y1, x2, y2;
    Workstation *wk = vwk->real_address;
    short *points, *pattern;
    Fgbg fill_colour, border_colour;
    long interior_style;

    if ((points = (short *) allocate_block(0)) == NULL)
        return;

    pattern = 0;
    interior_style = 0;
    border_colour = vwk->line.colour;
    if (gdp_code == 9)
    {
        col_pat(vwk, &fill_colour, &border_colour, &pattern);
        interior_style = ((long) vwk->fill.interior << 16) | (vwk->fill.style & 0xffffL);
    }

    x1 = coords[0];
    y1 = coords[1];
    if (x1 <= coords[2])
    {
        x2 = coords[2];
    } else
    {
        x2 = x1;
        x1 = coords[2];
    }
    if (y1 <= coords[3])
    {
        y2 = coords[3];
    } else
    {
        y2 = y1;
        y1 = coords[3];
    }

    rdeltax = (x2 - x1) / 2;
    rdeltay = (y2 - y1) / 2;

    xrad = wk->screen.mfdb.width >> 6;
    if (xrad > rdeltax)
        xrad = rdeltax;

    yrad = SMUL_DIV(xrad, wk->screen.pixel.width, wk->screen.pixel.height);
    if (yrad > rdeltay)
    {
        yrad = rdeltay;
        xrad = SMUL_DIV(yrad, wk->screen.pixel.height, wk->screen.pixel.width);
    }
    yrad = -yrad;

    /* n_steps = clc_nsteps(xrad, yrad); */

    for (i = 0; i < 5; i++)
    {
        points[i * 2] = SMUL_DIV(Icos(900 - 225 * i), xrad, 32767);
        points[i * 2 + 1] = SMUL_DIV(Isin(900 - 225 * i), yrad, 32767);
    }

    xc = x2 - xrad;
    yc = y1 - yrad;
    j = 10;
    for (i = 9; i >= 0; i--)
    {
        points[j + 1] = yc + points[i--];
        points[j] = xc + points[i];
        j += 2;
    }
    xc = x1 + xrad;
    j = 20;
    for (i = 0; i < 10; i++)
    {
        points[j++] = xc - points[i++];
        points[j++] = yc + points[i];
    }
    yc = y2 + yrad;
    j = 30;
    for (i = 9; i >= 0; i--)
    {
        points[j + 1] = yc - points[i--];
        points[j] = xc - points[i];
        j += 2;
    }
    xc = x2 - xrad;
    j = 0;
    for (i = 0; i < 10; i++)
    {
        points[j++] = xc + points[i++];
        points[j++] = yc - points[i];
    }
    points[40] = points[0];
    points[41] = points[1];

    if (gdp_code == 8)
    {
        c_pline(vwk, 21, border_colour, points);
    } else
    {
        fill_poly(vwk, points, 21, fill_colour, pattern, &points[42], vwk->mode, interior_style);
        if (vwk->fill.perimeter)
            c_pline(vwk, 21, border_colour, points);
    }

    free_block(points);
}
