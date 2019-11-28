/*
 * fVDI default drawing function code
 *
 * $Id: default.c,v 1.7 2006-02-19 01:13:58 johan Exp $
 *
 * Copyright 2003, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 *
 * The Bresenham line drawing variation with perfect clipping is
 * based on code from the Graphics Gems series of books.
 */

#include "fvdi.h"
#include "function.h"


extern call_draw_line(Virtual *vwk, DrvLine *line);

long retry_line(Virtual *vwk, DrvLine *pars)
{
    short *table, length, *index, moves;
    short n;
    short init_x, init_y, x2, y2;
    short movepnt;
    DrvLine line;

    table = index = 0;
    length = moves = 0;
    if ((long)vwk & 1)
    {
        if ((unsigned long)(pars->y1 & 0xffff) > 1)
            return;		/* Don't know about this kind of table operation */
        table = (short *)pars->x1;
        length = (pars->y1 >> 16) & 0xffff;
        if ((pars->y1 & 0xffff) == 1)
        {
            index = (short *)pars->y2;
            moves = pars->x2 & 0xffff;
        }
        vwk = (Virtual *)((long)vwk - 1);
    }
    else
        return;

    /* Clear high words of coordinate unions? */
    line.pattern = pars->pattern;
    line.colour = pars->colour;
    line.mode = pars->mode;

    init_x = line.x1 = *table++;
    init_y = line.y1 = *table++;

    movepnt = -1;
    if (index)
    {
        moves--;
        if (index[moves] == -4)
            moves--;
        if (index[moves] == -2)
            moves--;
        if (moves >= 0)
            movepnt = (index[moves] + 4) / 2;
    }

    for(n = 1; n < length; n++)
    {
        x2 = *table++;
        y2 = *table++;
        if (n == movepnt)
        {
            if (--moves >= 0)
                movepnt = (index[moves] + 4) / 2;
            else
                movepnt = -1;		/* Never again equal to n */
            init_x = line.x1 = x2;
            init_y = line.y1 = y2;
            continue;
        }

        if (((n == length - 1) || (n == movepnt - 1)) &&
                ((x2 != init_x) || (y2 != init_y)))
            line.draw_last = 1;	/* Draw last pixel in the line */
        else
            line.draw_last = 0;	/* Do not draw last pixel in the line */

#if 0
        c_draw_line(vwk, x1, y1, x2, y2, pattern, colour, mode, draw_last);
#else
        line.x2 = x2;
        line.y2 = y2;
        call_draw_line(vwk, &line);
#endif

        line.x1 = x2;
        line.y1 = y2;
    }
}


/* This is in a rather adhoc state at the moment, unfortunately.
 * But it does seem to work, for what it does, and is not complicated.
 */
void vr_transfer_bits(Virtual *vwk, GCBITMAP *src_bm, GCBITMAP *dst_bm,
                      RECT16 *src_rect, RECT16 *dst_rect, long mode)
{
    char buf[10];
    char *error;

    error = 0;

    do
    {
        if ((src_rect->x2 - src_rect->x1 != dst_rect->x2 - dst_rect->x1) ||
                (src_rect->y2 - src_rect->y1 != dst_rect->y2 - dst_rect->y1))
        {
            error = "No support yet for scaling";
            break;
        }

        if (!src_bm)
        {
            error = "No support yet for screen->memory/screen";
            break;
        }

        if (dst_bm) {
            if (src_bm->px_format != dst_bm->px_format)
            {
                if ((src_bm->px_format == 0x01020101) &&
                        (dst_bm->px_format == 0x01020808))
                {
                    int x, y;

                    for (y = src_rect->y1; y <= src_rect->y2; y++)
                    {
                        long *src;
                        char *dst;
                        unsigned long v, mask;

                        src = (long *)(src_bm->addr + src_bm->width * y + (src_rect->x1 / 32) * 4);
                        mask = 1 << (31 - src_rect->x1 % 32);
                        dst = dst_bm->addr +
                                dst_bm->width * (dst_rect->y1 - src_rect->y1 + y) +
                                dst_rect->x1;
                        v = *src++;
                        if (mode == 33)
                        {
                            for (x = src_rect->x2 - src_rect->x1; x >= 0; x--)
                            {
                                if (v & mask)
                                    *dst = ~0;
                                dst++;
                                mask = (mask >> 1) | (mask << 31);
                                if ((long)mask < 0)
                                    v = *src++;
                            }
                        }
                        else
                        {
                            for (x = src_rect->x2 - src_rect->x1; x >= 0; x--)
                            {
                                if (v & mask)
                                    *dst++ = ~0;
                                else
                                    *dst++ = 0;
                                mask = (mask >> 1) | (mask << 31);
                                if ((long)mask < 0)
                                    v = *src++;
                            }
                        }
                    }
                    mode = 0;  /* Just to skip error printout at the end */
                }
                else if ((src_bm->px_format == 0x01020101) &&
                           (dst_bm->px_format == 0x03421820))
                {
                    int x, y;

                    for (y = src_rect->y1; y <= src_rect->y2; y++)
                    {
                        long *src, *dst;
                        unsigned long v, mask;

                        src = (long *)(src_bm->addr + src_bm->width * y + (src_rect->x1 / 32) * 4);
                        mask = 1 << (31 - src_rect->x1 % 32);
                        dst = (long *)(dst_bm->addr +
                                       dst_bm->width * (dst_rect->y1 - src_rect->y1 + y)) +
                                dst_rect->x1;
                        v = *src++;
                        if (mode == 33)
                        {
                            for (x = src_rect->x2 - src_rect->x1; x >= 0; x--)
                            {
                                if (v & mask)
                                    *dst = ~0;
                                dst++;
                                mask = (mask >> 1) | (mask << 31);
                                if ((long)mask < 0)
                                    v = *src++;
                            }
                        }
                        else
                        {
                            for (x = src_rect->x2 - src_rect->x1; x >= 0; x--)
                            {
                                if (v & mask)
                                    *dst++ = ~0;
                                else
                                    *dst++ = 0;
                                mask = (mask >> 1) | (mask << 31);
                                if ((long) mask < 0)
                                    v = *src++;
                            }
                        }
                    }
                    mode = 0;  /* Just to skip error printout at the end */
                }
                else
                {
                    error = "No support yet for memory->memory between these different pixmap formats";
                    break;
                }
            }
            else
            {
                if (src_bm->px_format == 0x01020101)
                {
                    /* PX_PREF1 */
                    error = "No support yet for 1 bit memory->memory";
                    break;
                }
                else if (src_bm->px_format == 0x01020808)
                {
                    /* PX_PREF8 */
                    int x, y;

                    for (y = src_rect->y1; y <= src_rect->y2; y++)
                    {
                        char *src, *dst;
                        src = src_bm->addr + src_bm->width * y + src_rect->x1;
                        dst = dst_bm->addr +
                                dst_bm->width * (dst_rect->y1 - src_rect->y1 + y) +
                                dst_rect->x1;
                        for (x = src_rect->x2 - src_rect->x1; x >= 0; x--)
                            *dst++ = *src++;
                    }
                }
                else if (src_bm->px_format == 0x03421820)
                {
                    /* PX_PREF32 */
                    int x, y;

                    for (y = src_rect->y1; y <= src_rect->y2; y++)
                    {
                        long *src, *dst;
                        src = (long *)(src_bm->addr + src_bm->width * y) + src_rect->x1;
                        dst = (long *)(dst_bm->addr +
                                       dst_bm->width * (dst_rect->y1 - src_rect->y1 + y)) +
                                dst_rect->x1;
                        for (x = src_rect->x2 - src_rect->x1; x >= 0; x--)
                            *dst++ = *src++;
                    }
                }
                else
                {
                    puts("Unsupported pixel format ($");
                    ltoa(buf, src_bm->px_format, 16);
                    puts(buf);
                    error = ") for memory->memory";
                    break;
                }
            }
        }
        else
        {
            if (src_bm->px_format == 0x01020101)
            {
                /* PX_PREF1 */
                int x, y, i;
                char *block;
                long *palette;
                MFDB mfdb;
                short coords[8];

#if 0
                if (!src_bm->ctab) {
                    error = "Need a colour table for 1 bit->TC";
                    break;
                }
                if (src_bm->ctab->color_space != 1) {
                    error = "Need an RGB colour table for 1 bit->TC";
                    break;
                }
#endif

                if (!(block = (char *)allocate_block(0)))
                {
                    error = "Could not allocate memory block";
                    break;
                }

#if 0
                palette = (long *)block;
                for(i = 0; i < src_bm->ctab->no_colors; i++) {
                    palette[i] = ((long)(src_bm->ctab->colors[i].rgb.red & 0xff) << 16) |
                            ((long)(src_bm->ctab->colors[i].rgb.green & 0xff) << 8) |
                            ((long)(src_bm->ctab->colors[i].rgb.blue & 0xff));
                }
#endif

                mfdb.address   = (short *)&block[src_bm->ctab->no_colors * sizeof(*palette)];
                mfdb.width     = src_rect->x2 - src_rect->x1 + 1;
                mfdb.height    = 1;
                mfdb.wdwidth   = (mfdb.width + 15) / 16;
                mfdb.standard  = 0;
                mfdb.bitplanes = 32;

                coords[0] = 0;
                coords[1] = 0;
                coords[2] = src_rect->x2 - src_rect->x1 + 1;
                coords[3] = 0;

                coords[4] = dst_rect->x1;
                coords[5] = dst_rect->y1;
                coords[6] = dst_rect->x2;
                coords[7] = dst_rect->y1;

                for (y = src_rect->y1; y <= src_rect->y2; y++)
                {
                    long *src, *dst;
                    unsigned long v, mask;

                    src = (long *)(src_bm->addr + src_bm->width * y) + (src_rect->x1 / 32);
                    mask = 1 << (31 - src_rect->x1 % 32);
                    dst = (long *)&block[src_bm->ctab->no_colors * sizeof(*palette)];
                    v = *src++;
                    if (mode == 33)
                    {
                        short coords2[8];
                        for (i = 0; i < 4; i++)
                        {
                            coords2[i] = coords[i + 4];
                            coords2[i + 4] = coords[i];
                        }
                        lib_vdi_spppp(&lib_vro_cpyfm, vwk, 3, coords2, 0, &mfdb, 0);
                        for (x = src_rect->x2 - src_rect->x1; x >= 0; x--)
                        {
                            if (v & mask)
                                *dst = ~0;
                            dst++;
                            mask = (mask >> 1) | (mask << 31);
                            if ((long)mask < 0)
                                v = *src++;
                        }
                    }
                    else
                    {
                        for (x = src_rect->x2 - src_rect->x1; x >= 0; x--)
                        {
                            if (v & mask)
                                *dst++ = ~0;
                            else
                                *dst++ = 0;
                            mask = (mask >> 1) | (mask << 31);
                            if ((long)mask < 0)
                                v = *src++;
                        }
                    }
                    lib_vdi_spppp(&lib_vro_cpyfm, vwk, 3, coords, &mfdb, 0, 0);
                    coords[5]++;
                    coords[7]++;
                }

                free_block(block);
#if 0
                mode = 0;  /* Just to skip error printout at the end */
#else
                error = "OK?";
#endif
            }
            else if (src_bm->px_format == 0x01020808)
            {
                /* PX_PREF8 */
                char *block;
                long *palette;
                unsigned char *src;
                long *dst;
                MFDB mfdb;
                short coords[8];
                int x, y, i;

                if (!src_bm->ctab)
                {
                    error = "Need a colour table for 8 bit->TC";
                    break;
                }
                if (src_bm->ctab->color_space != 1)
                {
                    error = "Need an RGB colour table for 8 bit->TC";
                    break;
                }

                if (!(block = (char *)allocate_block(0)))
                {
                    error = "Could not allocate memory block";
                    break;
                }

                palette = (long *)block;
                for (i = 0; i < src_bm->ctab->no_colors; i++)
                {
                    palette[i] = ((long)(src_bm->ctab->colors[i].rgb.red & 0xff) << 16) |
                            ((long)(src_bm->ctab->colors[i].rgb.green & 0xff) << 8) |
                            ((long)(src_bm->ctab->colors[i].rgb.blue & 0xff));
                }

                mfdb.address   = (short *)&block[src_bm->ctab->no_colors * sizeof(*palette)];
                mfdb.width     = src_rect->x2 - src_rect->x1 + 1;
                mfdb.height    = 1;
                mfdb.wdwidth   = (mfdb.width + 15) / 16;
                mfdb.standard  = 0;
                mfdb.bitplanes = 32;

                coords[0] = 0;
                coords[1] = 0;
                coords[2] = src_rect->x2 - src_rect->x1 + 1;
                coords[3] = 0;

                coords[4] = dst_rect->x1;
                coords[5] = dst_rect->y1;
                coords[6] = dst_rect->x2;
                coords[7] = dst_rect->y1;

                for (y = src_rect->y1; y <= src_rect->y2; y++)
                {
                    src = src_bm->addr + src_bm->width * y + src_rect->x1;
                    dst = (long *)&block[src_bm->ctab->no_colors * sizeof(*palette)];
                    for (x = src_rect->x2 - src_rect->x1; x >= 0; x--)
                        *dst++ = palette[*src++];
                    lib_vdi_spppp(&lib_vro_cpyfm, vwk, 3, coords, &mfdb, 0, 0);
                    coords[5]++;
                    coords[7]++;
                }

                free_block(block);
            }
            else if (src_bm->px_format == 0x03421820)
            {
                /* PX_PREF32 */
                MFDB mfdb;
                short coords[8];

                mfdb.address   = (short *)src_bm->addr;
                mfdb.width     = src_bm->xmax - src_bm->xmin;
                mfdb.height    = src_bm->ymax - src_bm->ymin;
                mfdb.wdwidth   = (mfdb.width + 15) / 16;
                mfdb.standard  = 0;
                mfdb.bitplanes = 32;

                coords[0] = src_rect->x1;
                coords[1] = src_rect->y1;
                coords[2] = src_rect->x2;
                coords[3] = src_rect->y2;

                coords[4] = dst_rect->x1;
                coords[5] = dst_rect->y1;
                coords[6] = dst_rect->x2;
                coords[7] = dst_rect->y2;

                lib_vdi_spppp(&lib_vro_cpyfm, vwk, 3, coords, &mfdb, 0, 0);
            }
            else
            {
                puts("Unsupported source pixel format ($");
                ltoa(buf, src_bm->px_format, 16);
                puts(buf);
                error = ") for !TC->TC";
                break;
            }
        }
    } while(0);

    if (error)
    {
        int i;

        puts(error);
        puts("\x0d\x0a");

        puts("vr_transfer_bits mode "),
                ltoa(buf, mode, 10);
        puts(buf);
        puts("\x0d\x0a");

        for(i = 0; i < 2; i++)
        {
            GCBITMAP *bm = (i == 0) ? src_bm : dst_bm;
            if (bm)
            {
                puts((i == 0) ? "SRC" : "DST");
                puts(" $");
                ltoa(buf, (long)bm->addr, 16);
                puts(buf);
                puts("  Width: ");
                ltoa(buf, bm->width, 10);
                puts(buf);
                puts("  Bits: ");
                ltoa(buf, bm->bits, 10);
                puts(buf);
                puts("  Format: $");
                ltoa(buf, bm->px_format, 16);
                puts(buf);
                if (bm->xmin || bm->ymin)
                {
                    puts("  Base: ");
                    ltoa(buf, bm->xmin, 10);
                    puts(buf);
                    puts(",");
                    ltoa(buf, bm->ymin, 10);
                    puts(buf);
                }
                puts("  Dim: ");
                ltoa(buf, bm->xmax - bm->xmin, 10);
                puts(buf);
                puts(",");
                ltoa(buf, bm->ymax - bm->ymin, 10);
                puts(buf);
                puts("\x0d\x0a");

                puts("    C/ITAB: $");
                ltoa(buf, (long)bm->ctab, 16);
                puts(buf);
                puts("/$");
                ltoa(buf, (long)bm->itab, 16);
                puts(buf);

                if (bm->ctab)
                {
                    puts("  ID: $");
                    ltoa(buf, bm->ctab->map_id, 16);
                    puts(buf);
                    puts("  Space: ");
                    ltoa(buf, bm->ctab->color_space, 10);
                    puts(buf);
                    puts("  Flags: $");
                    ltoa(buf, bm->ctab->flags, 16);
                    puts(buf);
                    puts("  Colours: ");
                    ltoa(buf, bm->ctab->no_colors, 10);
                    puts(buf);
                }

                puts("\x0d\x0a");
            }
        }

        puts("--------\x0d\x0a");
    }
    else if ((mode != 0) && (mode != 32))
    {
        ltoa(buf, mode, 10);
        puts("\x0d\x0a");
        puts("vr_transform_bits mode ");
        puts(buf);
        puts("\x0d\x0a\x0d\x0a");
    }
}


#if 0
dc.b	0,"default_line",0
* _default_line - Pixel by pixel line routine
* In:	a0	VDI struct (odd address marks table operation)
  *	d0	Colour
  *	d1	x1 or table address
  *	d2	y1 or table length (high) and type (0 - coordinate pairs, 1 - pairs+moves)
  *	d3	x2 or move point count
  *	d4	y2 or move index address
  *	d5.w	Pattern
  *	d6	Logic operation
  * Call:	a0	VDI struct, 0 (destination MFDB)
  *	d1-d2.w	Coordinates
  *	a3-a4	Set/get pixel
  _default_line:
  movem.l	d6-d7/a1/a3-a4,-(a7)

  move.w	a0,d7
               and.w	#1,d7
               sub.w	d7,a0

               move.l	vwk_real_address(a0),a1
                                             move.l	wk_r_get_colour(a1),a1	; Index to real colour
jsr	(a1)

clr.l	-(a7)			; No MFDB => draw on screen
        move.l	a0,-(a7)

move.w	d6,-(a7)
bsr	setup_plot		; Setup pixel plot functions (a1/a3/a4)
addq.l	#2,a7

tst.w	d7
bne	.multiline

bsr	clip_line
bvs	.skip_draw

move.l	a7,a0			; a0 no longer -> VDI struct!

        bsr	.draw
        .skip_draw:

        move.l	(a7),a0
addq.l	#8,a7

movem.l	(a7)+,d6-d7/a1/a3-a4
rts

.draw:
    move.l	#$00010001,d7		; d7 = y-step, x-step

sub.w	d1,d3			; d3 = dx
        bge	.ok1
        neg.w	d3
        neg.w	d7
        .ok1:
        sub.w	d2,d4			; d4 = dy
        bge	.ok2
        neg.w	d4
        swap	d7
        neg.w	d7
        swap	d7
        .ok2:
        and.l	#$ffff,d5
cmp.w	d3,d4
bls	.xmajor
or.l	#$80000000,d5
exg	d3,d4
.xmajor:
    add.w	d4,d4			; d4 = incrE = 2dy
        move.w	d4,d6
sub.w	d3,d6			; d6 = lines, d = 2dy - dx
        swap	d4
        move.w	d6,d4
sub.w	d3,d4			; d4 = incrE, incrNE = 2(dy - dx)

        rol.w	#1,d5
jsr	(a1)

swap	d1
move.w	d2,d1
swap	d1			; d1 = y, x
bra	.loop_end1

.loop1:
    tst.w	d6
    bgt	.both
    swap	d4
    add.w	d4,d6
    swap	d4
    tst.l	d5
    bmi	.ymajor
    add.w	d7,d1
    bra	.plot
    .ymajor:
    swap	d7
    swap	d1
    add.w	d7,d1
    swap	d7
    swap	d1
    bra	.plot
    .both:
    add.w	d4,d6
    ;	add.l	d7,d1
add.w	d7,d1
swap	d7
swap	d1
add.w	d7,d1
swap	d7
swap	d1
.plot:
    move.l	d1,d2
    swap	d2
    rol.w	#1,d5
    jsr	(a1)

  .loop_end1:
  dbra	d3,.loop1
            rts

            .multiline:				; Transform multiline to single ones
cmp.w	#1,d2
bhi	.line_done		; Only coordinate pairs and pairs+marks available so far
beq	.use_marks
moveq	#0,d3			; Move count
.use_marks:
    swap	d3
    move.w	#1,d3			; Current index in high word
swap	d3
movem.l	d0/d2/d3/d5/a0/a5-a6,-(a7)
move.l	d1,a5			; Table address
move.l	d4,a6			; Move index address
tst.w	d3			;  may not be set
beq	.no_start_move
add.w	d3,a6
add.w	d3,a6
subq.l	#2,a6
cmp.w	#-4,(a6)
bne	.no_start_movex
subq.l	#2,a6
sub.w	#1,d3
.no_start_movex:
    cmp.w	#-2,(a6)
  bne	.no_start_move
  subq.l	#2,a6
               sub.w	#1,d3
               .no_start_move:
               bra	.line_loop_end
               .line_loop:
               movem.w	(a5),d1-d4
                             move.l	7*4(a7),a0
                                            bsr	clip_line
                                            bvs	.no_draw
                                            move.l	0(a7),d6		; Colour
move.l	3*4(a7),d5		; Pattern
;	move.l	xxx(a7),d0		; Logic operation
lea	7*4(a7),a0
bsr	.draw
.no_draw:
    move.l	2*4(a7),d3
                    tst.w	d3
                    beq	.no_marks
                    swap	d3
                    addq.w	#1,d3
                    move.w	d3,d4
                    add.w	d4,d4
                    subq.w	#4,d4
                    cmp.w	(a6),d4
                                 bne	.no_move
                                 subq.l	#2,a6
                                 addq.w	#1,d3
                                 swap	d3
                                 subq.w	#1,d3
                                 swap	d3
                                 addq.l	#4,a5
                                 subq.w	#1,1*4(a7)
  .no_move:
  swap	d3
  move.l	d3,2*4(a7)
  .no_marks:
  addq.l	#4,a5
               .line_loop_end:
               subq.w	#1,1*4(a7)
  bgt	.line_loop
  movem.l	(a7)+,d0/d2/d3/d5/a0/a5-a6
                  .line_done:
                  move.l	(a7),a0
                                 addq.l	#8,a7

                                 movem.l	(a7)+,d6-d7/a1/a3-a4
                                                  rts

                                                  dc.b	0,"default_fill",0
                                                  * _default_fill - Line by line (or pixel by pixel) fill routine
  * In:	a0	VDI struct (odd address marks table operation)
  *	d0	Colours
  *	d1	x1 destination or table address
  *	d2	y1   - "" -     or table length (high) and type (0 - y/x1/x2 spans)
  *	d3-d4.w	x2,y2 destination
               *	d5	Pointer to pattern
               *	d6	Mode
               *	d7	Interior/style
               * Call:	a0	VDI struct, 0 (destination MFDB)
  *	d0	Colours
  *	d1-d2.w	Start coordinates
  *	d3-d4.w	End coordinates
  *	d5	Pattern
  _default_fill:
  movem.l	d1-d7/a0-a2,-(a7)

  move.w	a0,d7
               and.w	#1,d7
               sub.w	d7,a0

               move.l	d5,a2

               cmp.w	#1,d6			; Not replace?
        bne	.pattern

        move.l	a2,a1
moveq	#8-1,d5
move.l	d6,-(a7)
.check_pattern:
    move.l	(a1)+,d6		; All ones?
        addq.l	#1,d6
dbne	d5,.check_pattern
beq	.no_pattern
move.l	(a7)+,d6
bra	.pattern
.no_pattern:
    move.l	(a7)+,d6
                  moveq	#-1,d5

                  move.l	vwk_real_address(a0),a1
                                                 move.l	wk_r_line(a1),a1
                                                                      cmp.l	#_default_line,a1	; No real acceleration?
        beq	.pattern

        tst.w	d7
        bne	.table_lfill

        move.w	d4,d7
moveq	#0,d6
move.w	vwk_mode(a0),d6
.loopy_sl:
    move.w	d2,d4
    jsr	(a1)
  addq.w	#1,d2
               cmp.w	d7,d2
               ble	.loopy_sl

               .end_default_fill:
               movem.l	(a7)+,d1-d7/a0-a2
                              rts

                              .table_lfill:
                              tst.w	d2
                              bne	.end_default_fill	; Only y/x1/x2 spans available so far
move.l	d2,d6
swap	d6
subq.w	#1,d6
blt	.end_default_fill
move.l	d1,a2
.tlfill_loop:
    moveq	#0,d2
    move.w	(a2)+,d2
                  move.w	d2,d4
                  moveq	#0,d1
                  move.w	(a2)+,d1
                                  move.w	(a2)+,d3
                                                  jsr	(a1)
  dbra	d6,.tlfill_loop
            bra	.end_default_fill

            * Call:	a0	VDI struct, 0 (destination MFDB)
  *	d0	Colour values
  *	d1-d2.w	Coordinates
  *	d6	Mode
  *	a3-a4	Set/get pixel
  .pattern:
  movem.l	a3-a4,-(a7)

  move.l	vwk_real_address(a0),a1
                                 move.l	wk_r_get_colour(a1),a1	; Index to real colour
jsr	(a1)

move.w	d6,-(a7)
bsr	setup_plot
addq.l	#2,a7

;	move.l	d5,a2

clr.l	-(a7)			; No MFDB => draw on screen
        move.l	a0,-(a7)
move.l	a7,a0			; a0 no longer -> VDI struct!

        tst.w	d7
        bne	.table_pfill

        move.w	d1,d6
        .loopy_pp:
        move.w	d6,d1			; x
move.w	d2,d5
and.w	#$000f,d5
add.w	d5,d5
move.w	0(a2,d5.w),d5
rol.w	d1,d5
.loopx_pp:
    rol.w	#1,d5
    jsr	(a1)
  addq.w	#1,d1
               cmp.w	d3,d1
               ble	.loopx_pp
               addq.w	#1,d2
               cmp.w	d4,d2
               ble	.loopy_pp
               .end_pfill:
               move.l	(a7),a0
                             addq.l	#8,a7
                             movem.l	(a7)+,a3-a4
                                              bra	.end_default_fill

                                              .table_pfill:
                                              tst.w	d2
                                              bne	.end_pfill		; Only y/x1/x2 spans available so far
move.l	d2,d6
swap	d6
subq.w	#1,d6
blt	.end_pfill

move.l	a5,-(a7)
move.l	d1,a5
.tploopy_pp:
    moveq	#0,d2
    move.w	(a5)+,d2
                  moveq	#0,d1
                  move.w	(a5)+,d1
                                  move.w	(a5)+,d3

                                                  move.w	d2,d5
                                                  and.w	#$000f,d5
                                                  add.w	d5,d5
                                                  move.w	0(a2,d5.w),d5
                                                                       rol.w	d1,d5
                                                                       .tploopx_pp:
                                                                       rol.w	#1,d5
                                                                       jsr	(a1)
  addq.w	#1,d1
               cmp.w	d3,d1
               ble	.tploopx_pp
               dbra	d6,.tploopy_pp
               move.l	(a7)+,a5
                              bra	.end_pfill

                              #endif
