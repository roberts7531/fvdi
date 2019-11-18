/*
 * fVDI Bezier code
 *
 * $Id: bezier.c,v 1.4 2004-10-17 21:44:11 johan Exp $
 *
 * Copyright 1999-2001/2003, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 *
 * This is a modified version of code with an original
 * copyright as follows.
 */

/*   Description       : Fast Bezier approximation using
 *                       four control points.
 *
 *   Original Author   : William D. Herndon, CCP Software, 29.04.88
 *                       See also the best book so far on beziers:
 *                        Algorithms for graphics & image processing,
 *                        Theo Pavlidis, Bell Labs,
 *                        Computer Science Press ISBN 0-914894-65-X.
 *
 ********************************************************************
 *       Copyright 1999, Caldera Thin Clients, Inc.                 *
 *       This software is licenced under the GNU Public License.    *
 *       Please see LICENSE.TXT for further information.            *
 *                                                                  *
 *                  Historical Copyright                            *
 *                                                                  *
 *  Copyright (C) 1976-1992 Digital Research Inc. All rights        *
 *  reserved. The Software Code contained in this listing is        *
 *  proprietary to Digital Research Inc., Monterey,                 *
 *  California, and is covered by U.S. and other copyright          *
 *  protection. Unauthorized copying, adaption, distribution,       *
 *  use or display is prohibited and may be subject to civil        *
 *  and criminal penalties. Disclosure to others is                 *
 *  prohibited. For the terms and conditions of software use,       *
 *  refer to the appropriate Digital Research Licence               *
 *  Agreement.                                                      *
 ********************************************************************/

#include "fvdi.h"
#include "utility.h"
#include "function.h"
#include "line.h"

#define BIG_ENDIAN

#define	BEZIER_START	0x01
#define	POINT_MOVE	0x02

#define NULL 0
#define TRUE 1
#define FALSE 0

/* These should be fetched from wk-> */
#define	MIN_BEZIER_DEPTH	2	/*** CJLT  was 1 ***/
#define	MAX_BEZIER_DEPTH	7

#define	MIN_DEPTH_SCALE		9
#define	MAX_DEPTH_SCALE		0

#define MINVERTSIN		129
#define MININTIN		56

#define MULT(a,b)	((long)(a) * (b))
#define _max(x,y)		(((x) > (y)) ? (x) : (y))


extern short line_types[];


/* A normal ABS macro generated silly code */
/* The argument can't be a value! */
#define POSITIVE(v)		\
{				\
    if (v < 0)		\
    v = -v;		\
    }

/* Find min and max among four elements */
/* None of the arguments can be values! */
#define MINMAX(type,min,max,v3,v4)	\
{				\
    if (min > max) {	\
    type tmp = min;	\
    min = max;	\
    max = tmp;	\
    }			\
    if (v3 > v4) {		\
    type tmp = v3;	\
    v3 = v4;	\
    v4 = tmp;	\
    }			\
    if (v4 > max)		\
    max = v4;	\
    if (v3 < min)		\
    min = v3;	\
    }

/* Find the max among three values */
#define MAX3(type,max,v1,v2,v3)	\
{				\
    type tmp;		\
    max = v1;		\
    POSITIVE(max);		\
    tmp = v2;		\
    POSITIVE(tmp);		\
    if (tmp > max)		\
    max = tmp;	\
    tmp = v3;		\
    POSITIVE(tmp);		\
    if (tmp > max)		\
    max = tmp;	\
    }


short CDECL calc_bez(char *marks, short *points, long flags,
                     long maxpnt, long maxin, short **xmov, short **xpts,
                     short *pnt_mv_cnt, short *x_used);


/*================================================================
    Internal static variables
================================================================*/

#if 0
short	wk_depth_scale	= MAX_DEPTH_SCALE;
short	*appbuff	= NULL;
unsigned short	appsize	= 0;
short	*dosbuff	= NULL;
unsigned short	dossize	= 0;
short	bez_capable	= 0;
short	is_metafile	= 0;
#endif

/* Fetch from wk-> */
short	max_poly_points = 1024;


/**** New structures added by CLT *****/

struct coords {
    short x;
    short y;
}; /* All co-ordinate pairs are longs */

struct node {
    struct coords anchor, in, out;
    short onscreen, level;
};
/* "node" is a misnomer as each node is one section of Bezier.
 * anchor is its end point. Its start point is the anchor from the
 *   next node (where next=current+1)
 * in is the coordinates of the direction point in this section
 *   belonging to its anchor
 * out is the direction point in this section of the next anchor
 * onscreen is a flag. TRUE if Bezier entirely onscreen/page
 * level is the depth of division before we should stop and print
 */


struct v_bez_pars {
    short num_pts;
    short *points;
    char  *bezarr;
    short *extent;
    short *totpoints;
    short *totmoves;
};

void CDECL
lib_v_bez(Virtual *vwk, struct v_bez_pars *par)
{
    short x_used;
    short *xpts, *xmov;
    int depth_scale, depth_scale_min;
    int result;
    short *block;
    int num_points;
    short *points;
    short pattern;
    short minx, miny, maxx, maxy, k;
    int i;

    num_points = par->num_pts;
    points = par->points;

    result = -1;
    depth_scale_min = vwk->real_address->drawing.bezier.depth_scale.min;
    for(depth_scale = vwk->bezier.depth_scale;
        depth_scale <= depth_scale_min; depth_scale++) {
        xpts = &vwk->clip.rectangle.x1;
        result = calc_bez(par->bezarr, points, depth_scale,
                          num_points, num_points, &xmov, &xpts,
                          par->totmoves, &x_used);
        if (result >= 0)
            break;
        if (!x_used) {
            lib_v_pline(vwk, (void *)par);
            break;
        }
    }
    if (result >= 0) {
        num_points = result;
        points = xpts;
        if ((vwk->line.width > 1) && (block = (short *)allocate_block(0))) {
            wide_line(vwk, points, num_points, vwk->line.colour.l, block, vwk->mode);
            free_block(block);
        } else {
            pattern = vwk->line.user_mask;
            if (vwk->line.type != 7)
                pattern = line_types[vwk->line.type - 1];
            v_bez_accel((long)vwk + 1, points, ((long)num_points << 16) | 1,
                        (long)*par->totmoves, xmov, (long)pattern,
                        *(long *)&vwk->line.colour, (long)vwk->mode);
        }
    } else {
        x_used = 0;
        xpts = 0;
        xmov = 0;
    }

    minx = maxx = *points++;
    miny = maxy = *points++;
    for(i = num_points - 1; i >= 0; i--) {
        k = *points++;
        if (k < minx)
            minx = k;
        else if (k > maxx)
            maxx = k;
        k = *points++;
        if (k < miny)
            miny = k;
        else if (k > maxy)
            maxy = k;
    }

    par->extent[0] = minx;
    par->extent[1] = miny;
    par->extent[2] = maxx;
    par->extent[3] = maxy;
    *par->totpoints = num_points;
    *par->totmoves = x_used;
    if (xpts)
        free_block(xpts);
}


/*
 * bez_depth:  Find the bezier depth necessary for
 *             a reasonable looking curve.
 */
int
bez_depth(short *pts, short depth_scale)
{
    short base, xdiff, ydiff, depth;
    long bez_size;

    /* Estimate area. May be to high, but that's better than too low */

    base = pts[0];			/* Find max xdiff */
    MAX3(short, xdiff, base - pts[2], base - pts[4], base - pts[6]);

    base = pts[1];			/* Find max ydiff */
    MAX3(short, ydiff, base - pts[3], base - pts[5], base - pts[7]);

    bez_size = MULT(xdiff, ydiff);

    if (!(bez_size >>= 4 * MIN_BEZIER_DEPTH + depth_scale - 2))
        return MIN_BEZIER_DEPTH;

    for(depth = MIN_BEZIER_DEPTH + 1; depth < MAX_BEZIER_DEPTH; depth++)
        if (!(bez_size >>= 4))
            return depth; /*** CLT was 3 ***/

    return MAX_BEZIER_DEPTH;
}


/*
 * do_bez4:  Calculate a bezier curve for xyin using
 *           exactly four control points.
 *
 * Input:    x & y control point coords in array xyin.
 *           Max # of points of bezier curve to calculate m.
 * Output:   Bezier curve x & y coords in array xyout.
 *
 * The first point is already done and the last point
 * will be done for us in the drivers. -WDH
 */
int
do_bez4(short *xyin, short depth, short *xyout, short clip[])
{
    int count;
    struct coords mid;  /* Used to save a temporary value */
    struct node *current;
    struct node bez[MAX_BEZIER_DEPTH + 1];

    /* Initialize our local data structures */

    count = 0;
    current = bez;  /* Point to first node */
    current->onscreen = FALSE;  /* Assume it may be partly or all off */
    current->level = depth - 1; /* ... and initialize it. Depth-1 because the last
                step includes a hidden depth and a half */
    current->anchor.x = ((((long)xyin[6]) << 16) >> 1) + 0x4000;		/* It may look back-to-front, but that ensures */
    current->anchor.y = ((((long)xyin[7]) << 16) >> 1) + 0x4000;		/*   it prints in the right direction. */
    current->in.x =     ((((long)xyin[4]) << 16) >> 1) + 0x4000;		/* We have one bit to guard */
    current->in.y =     ((((long)xyin[5]) << 16) >> 1) + 0x4000;		/*   against overflow and 15 to */
    current->out.x =    ((((long)xyin[2]) << 16) >> 1) + 0x4000;		/*   guard against rounding error */
    current->out.y =    ((((long)xyin[3]) << 16) >> 1) + 0x4000;		/* Also, we add 4000H to bias to */
    (current + 1)->anchor.x = ((((long)xyin[0]) << 16) >> 1) + 0x4000;	/*   the centre of the pixel */
    (current + 1)->anchor.y = ((((long)xyin[1]) << 16) >> 1) + 0x4000;

    /* Now recursively divide the bez into small segments
     * that can be printed as straight lines
     */

    while (1) {					/* Until break, near the middle of the loop */
        if (current->onscreen == FALSE) {		/* Test if on-screen. */
            short xmin, xmax, ymin, ymax;
            short v3, v4;

            /* Find min and max of the coordinates */
#ifndef BIG_ENDIAN
            xmin = (short)(current->anchor.x >> 16);
            xmax = (short)(current->in.x >> 16);
            v3   = (short)(current->out.x >> 16);
            v4   = (short)((current + 1)->anchor.x >> 16);
#else
            xmin = *(short *)&current->anchor.x;
            xmax = *(short *)&current->in.x;
            v3   = *(short *)&current->out.x;
            v4   = *(short *)&(current + 1)->anchor.x;
#endif
            MINMAX(short, xmin, xmax, v3, v4);

#ifndef BIG_ENDIAN
            ymin = (short)(current->anchor.y >> 16);
            ymax = (short)(current->in.y >> 16);
            v3   = (short)(current->out.y >> 16);
            v4   = (short)((current + 1)->anchor.y >> 16);
#else
            ymin = *(short *)&current->anchor.y;
            ymax = *(short *)&current->in.y;
            v3   = *(short *)&current->out.y;
            v4   = *(short *)&(current + 1)->anchor.y;
#endif
            MINMAX(short, ymin, ymax, v3, v4);

            xmin = xmin * 2 - 16;		/* Take line thickness into account */
            ymin = ymin * 2 - 16;
            xmax = xmax * 2 + 1 + 16;
            ymax = ymax * 2 + 1 + 16;
            if ((xmax < clip[0]) || (xmin > clip[2]) || (ymax < clip[1]) || (ymin > clip[3]))
                current->level = 0;	/* Offscreen */
            else if ((xmin >= clip[0]) && (xmax <= clip[2]) && (ymin >= clip[1]) && (ymax <= clip[3]))
                current->onscreen = TRUE;
        }

        if (current->level <= 0) {
            /* We've reached the last stage. Send off the section as three
             * lines. No time to explain now. See CJLT's notes.
             */

            *xyout++ = (int)(((((current + 1)->anchor.x + current->in.x) >> 1) + current->out.x) >> 16);
            *xyout++ = (int)(((((current + 1)->anchor.y + current->in.y) >> 1) + current->out.y) >> 16);
            *xyout++ = (int)((((current->anchor.x + current->out.x) >> 1) + current->in.x) >> 16);
            *xyout++ = (int)((((current->anchor.y + current->out.y) >> 1) + current->in.y) >> 16);
            count += 3;
            if (current == bez)
                break;  /* ... from the infinite while loop, since we have done it.*/
            *xyout++ = (int)((current->anchor.x << 1) >> 16);
            *xyout++ = (int)((current->anchor.y << 1) >> 16);

            current--;
        } else {		/* Recursively divide the section into two pieces */
            mid.x = (current->in.x + current->out.x) >> 1;
            mid.y = (current->in.y + current->out.y) >> 1;
            (current + 2)->anchor.x = (current + 1)->anchor.x;
            (current + 2)->anchor.y = (current + 1)->anchor.y;
            (current + 1)->out.x = (current->out.x + (current + 1)->anchor.x) >> 1;
            (current + 1)->out.y = (current->out.y + (current + 1)->anchor.y) >> 1;
            (current + 1)->in.x = (mid.x + (current + 1)->out.x ) >> 1;
            (current + 1)->in.y = (mid.y + (current + 1)->out.y ) >> 1;
            current->in.x = (current->in.x + current->anchor.x) >> 1;
            current->in.y = (current->in.y + current->anchor.y) >> 1;
            current->out.x = (current->in.x + mid.x) >> 1;
            current->out.y = (current->in.y + mid.y) >> 1;
            (current + 1)->anchor.x = ((current + 1)->in.x + current->out.x) >> 1;
            (current + 1)->anchor.y = ((current + 1)->in.y + current->out.y) >> 1;

            /* and set the levels */

            current->level--;
            (current + 1)->level = current->level;
            (current + 1)->onscreen = current->onscreen;
            current++;		/* and point to the top of the stack of Bezier bits */
        }
    }

    return count;
}


/*
 * bezier4:     Calculate a bezier curve for xyin using
 *              exactly four control points.
 *
 * Input:       x & y control point coords in array xyin.
 *              Power of two of # of points of bezier curve to
 *              calculate. Must be 4, 5 or 6.
 * Output:      Bezier curve x & y coords in array xyout.
 */
void
bezier4(short *xyin, short **xyout, short depth_scale, short clip[])
{
    int depth, cnt;

    depth = bez_depth(xyin, depth_scale);
    cnt = do_bez4(xyin, depth, *xyout, clip);  /* CJLT changed to new parameters and returned value */
    *xyout += 2 * (cnt - 1);
}


/*
 * calc_bez:	Calculate Bezier curves and moves (when necessary).
 *
 * Input:  points  - The points (actual or reference). (was PTSIN)
 *          marks  - Marks beziers & move points. (was INTIN)
 *     flags & 0xff  depth_scale
 *     flags & 0x100 close_loop
 *                 - If first point should be copied
 *                   to last point (also done in point moves).
 *                   For polygons.
 *           xpts  - Clip coordinates
 *
 * Output:   xpts  - The resulting points.
 *           xmov  - An ordered list of the
 *                   indices of move points in XPTS.
 *         return  - The number of points in XPTS.
 *     pnt_mv_cnt  - The size of XMOV.
 *         x_used  - If Beziers or moves occured.
 */
short CDECL
calc_bez(char *marks, short *points, long flags, long maxpnt, long maxin, short **xmov, short **xpts, short *pnt_mv_cnt, short *x_used)
{
    short maxchk, movptr, i;
    short *pts_ptr, *last_pnt, *pts_out;
    char *chk_ptr;
    unsigned short memneeded;
    short *XMOV, *XPTS;

    *pnt_mv_cnt = 0;

    /* Calculate the number of points we will actually need
     * need with all the Bezier curves and point moves.
     */

    maxchk = maxpnt;
    if (maxin > (maxpnt - 1))
        maxin = maxpnt - 1;
    *x_used = FALSE;

    if (flags & 0x100)
        maxpnt++;

    for(chk_ptr = marks, pts_ptr = points, i = 0; i < maxin; chk_ptr++, (pts_ptr += 2), i++) {
        if (*(char *)((long)chk_ptr ^ 1) & POINT_MOVE) {		/* Byte swapped! */
            (*pnt_mv_cnt)++;
            *x_used = TRUE;
        }
        if (*(char *)((long)chk_ptr ^ 1) & BEZIER_START) {		/* Byte swapped! */
            if (i >= maxin - 1)
                break; /* disallow 2nd to last pnt */
            *x_used = TRUE;
            maxpnt += (3 << (bez_depth(pts_ptr, flags & 0xff) - 1)) - 3;
            chk_ptr += 2;
            pts_ptr += 4;
            i += 2;
        }
    }

    if (flags & 0x100)
        maxpnt += *pnt_mv_cnt;
    if (maxpnt > max_poly_points || !*x_used) {
        *xpts = 0;		/* Nothing to deallocate */
        return -1;		/* No Beziers or point moves, or simply too many points */
    }

    memneeded = _max(maxpnt, MINVERTSIN) * 2 * sizeof(short) +
            _max(*pnt_mv_cnt, MININTIN) * sizeof(short);
    memneeded = (memneeded + 15) >> 4;

    if (!(XPTS = (short *)allocate_block(0)) || (*(long *)XPTS < memneeded)) {
        if (XPTS)
            free_block(XPTS);
        *xpts = 0;		/* Nothing to deallocate */
        return -1;
    }
    XMOV = XPTS + _max(maxpnt, MINVERTSIN) * 2;

    pts_ptr = points;
    last_pnt = pts_ptr;
    pts_out = XPTS;
    movptr = *pnt_mv_cnt;
    for(i = 0, chk_ptr = marks; i < maxchk; i++, chk_ptr++) {
        if (i < maxin) {
            if (*(char *)((long)chk_ptr ^ 1) & POINT_MOVE) {	/* Byte swapped! */
                if (flags & 0x100) {
                    *pts_out++ = *last_pnt++;
                    *pts_out++ = *last_pnt++;
                    last_pnt = pts_ptr;
                }
                XMOV[--movptr] = (short)(pts_out - XPTS - 4);
            }
            *pts_out++ = *pts_ptr++;
            *pts_out++ = *pts_ptr++;
            if (*(char *)((long)chk_ptr ^ 1) & BEZIER_START) {	/* Byte swapped! */
                if (i >= maxin - 1)
                    break; /* Disallow 2nd to last pnt */
                pts_ptr -= 2;
                bezier4(pts_ptr, &pts_out, flags & 0xff, *xpts);	/* Send along clip coordinates */
                pts_ptr += 6;
                chk_ptr += 2;
                i += 2;
            }
        } else {
            *pts_out++ = *pts_ptr++;
            *pts_out++ = *pts_ptr++;
        }
    }
    if (flags & 0x100) {
        *pts_out++ = *last_pnt++;
        *pts_out++ = *last_pnt++;
    }

    *xmov = XMOV;
    *xpts = XPTS;

    return (short)(pts_out - XPTS) >> 1;
}


