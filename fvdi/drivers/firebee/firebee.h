/*
 * saga.h - SAGA specific functions
 * This is part of the SAGA driver for fVDI
 * Most of the code here come from the SAGA Picasso96 driver.
 * https://github.com/ezrec/saga-drivers/tree/master/saga.card
 * Glued by Vincent Riviere
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

#ifndef SAGA_H
#define SAGA_H

typedef unsigned char UBYTE;
typedef unsigned short UWORD;
typedef unsigned long ULONG;
typedef int BOOL;
typedef void VOID;
typedef ULONG IPTR;

#define NULL ((void*)0)
#define TRUE 1
#define FALSE 0

#define FIREBEE_VRAM_PHYS_OFFSET    0x40000000      /* physical offset FPGA RAM to ST RAM start (0x0) */

struct Node
{
    char *ln_Name;
};

/*
 * Inspired from:
 * https://github.com/ezrec/saga-drivers/blob/master/saga.card/include/picasso96/card.h
 */
struct ModeInfo
{
    struct Node Node;
    UWORD       Width;
    UWORD       Height;
    UBYTE       Depth;
    UBYTE       Flags;  /* See GM* flags below */
    UWORD       HorTotal;
    UWORD       HorBlankSize;
    UWORD       HorSyncStart;
    UWORD       HorSyncSize;
    UBYTE       HorSyncSkew;
    UBYTE       HorEnableSkew;
    UWORD       VerTotal;
    UWORD       VerBlankSize;
    UWORD       VerSyncStart;
    UWORD       VerSyncSize;
    UBYTE       Numerator;
    UBYTE       Denomerator;
    ULONG       PixelClock;
};

#define GMB_DOUBLECLOCK         0       /* Clock is doubled after selection */
#define GMB_INTERLACE           1       /* Mode is interlaced */
#define GMB_DOUBLESCAN          2       /* Scanlines are doubled */
#define GMB_HPOLARITY           3       /* Horizontal Polarity */
#define GMB_VPOLARITY           4       /* Vertical Polarity */
#define GMB_COMPATVIDEO         5       /* Compatible Video */
#define GMB_DOUBLEVERTICAL      6       /* Doubled Vertical */

#define GMF_DOUBLECLOCK         (1UL << GMB_DOUBLECLOCK)
#define GMF_INTERLACE           (1UL << GMB_INTERLACE)
#define GMF_DOUBLESCAN          (1UL << GMB_DOUBLESCAN)
#define GMF_HPOLARITY           (1UL << GMB_HPOLARITY)
#define GMF_VPOLARITY           (1UL << GMB_VPOLARITY)
#define GMF_COMPATVIDEO         (1UL << GMB_COMPATVIDEO)
#define GMF_DOUBLEVERTICAL      (1UL << GMB_DOUBLEVERTICAL)

/*
 * Inspired from:
 * https://github.com/ezrec/saga-drivers/blob/master/saga.card/saga_intern.h
 */

static inline VOID Write32(IPTR addr, ULONG value)
{
    *(volatile ULONG *) addr = value;
}

static inline VOID Write16(IPTR addr, UWORD value)
{
    *(volatile UWORD *) addr = value;
}

/* Test if width or height requires doublescan
 */
#define IS_DOUBLEX(w)   ((w) <= 500)
#define IS_DOUBLEY(h)   ((h) <= 300)

/* saga_pll.c */
int fbee_pll_clock_count(void);
int fbee_pll_clock_freq(int id, BOOL is_ntsc, ULONG *freq);
int fbee_pll_clock_lookup(BOOL is_ntsc, ULONG *freqp);
int fbee_pll_clock_program(int clock);

/* from modeline_vesa.c */
extern struct ModeInfo modeline_vesa_entry[];
extern const int modeline_vesa_entries;

/* from saga.c */
void fbee_fix_mode(struct ModeInfo *mi);
void fbee_set_clock(const struct ModeInfo *mi);
void fbee_set_modeline(const struct ModeInfo *mi, UBYTE Format);
void fbee_set_panning(UBYTE *mem);

#endif /* SAGA_H */
