/*
 * uaelib.c - Bindings to UAE Library Picasso96 functions
 * This is part of the WinUAE RTG driver for fVDI
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

/*#define ENABLE_KDEBUG*/

#include "fvdi.h"
#include "driver.h"
#include "uaegfx.h"
#include "uaelib.h"

/*
 * The uaelib glue has been borrowed from EmuTOS.
 * https://sourceforge.net/p/emutos/code/ci/master/tree/bios/amiga.c
 */

/* Location of the UAE Boot ROM, a.k.a. RTAREA */
#define RTAREA_DEFAULT 0x00f00000
#define RTAREA_BACKUP  0x00ef0000

/* uaelib_demux() is the entry point */
#define UAELIB_DEMUX_OFFSET 0xFF60
typedef ULONG uaelib_demux_t(ULONG fnum, ...);
static uaelib_demux_t* uaelib_demux = NULL;

#define IS_TRAP(x)(((*(ULONG*)(x)) & 0xf000ffff) == 0xa0004e75)

/* Initialize the UAE Library */
BOOL uaelib_init(void)
{
	if (IS_TRAP(RTAREA_DEFAULT + UAELIB_DEMUX_OFFSET))
		uaelib_demux = (uaelib_demux_t*)(RTAREA_DEFAULT + UAELIB_DEMUX_OFFSET);
	else if (IS_TRAP(RTAREA_BACKUP + UAELIB_DEMUX_OFFSET))
		uaelib_demux = (uaelib_demux_t*)(RTAREA_BACKUP + UAELIB_DEMUX_OFFSET);

	KDEBUG(("uaelib_demux = %p\n", uaelib_demux));

	return uaelib_demux != NULL;
}

/*
 * The following functions are bindings to call WinUAE Picasso96 functions.
 * They put the parameters into appropriate registers and call uaelib_demux().
 * The actual implementation of those functions can be found in WinUAE sources:
 * https://github.com/tonioni/WinUAE/blob/master/od-win32/picasso96_win.cpp
 * The entry point there is picasso_demux().
 * Then the implementation functions have the same names as the bindings here.
 * See the above WinUAE source for documentation of these functions.
 */

uae_u32 uaelib_picasso_FindCard(struct BoardInfo* bi)
{
	register long a0 __asm__("a0") = (long)bi;
	register long retvalue __asm__("d0");

	__asm__ volatile
	(
		"move.l  #16,-(sp)\n\t"
		"jsr     (%1)\n\t"
		"addq.l  #4,sp"
	: "=r"(retvalue)
	: "a"(uaelib_demux), "r"(a0)
	: "memory", "cc"
	);

	return retvalue;
}

uae_u32 uaelib_picasso_InitCard(struct BoardInfo* bi)
{
	register long a0 __asm__("a0") = (long)bi;
	register long retvalue __asm__("d0");

	__asm__ volatile
	(
		"move.l  #29,-(sp)\n\t"
		"jsr     (%1)\n\t"
		"addq.l  #4,sp"
	: "=r"(retvalue)
	: "a"(uaelib_demux), "r"(a0)
	: "memory", "cc"
	);

	return retvalue;
}

uae_u32 uaelib_picasso_SetGC(struct BoardInfo* bi, struct ModeInfo* mi, uae_u32 Border)
{
	register long a0 __asm__("a0") = (long)bi;
	register long a1 __asm__("a1") = (long)mi;
	register long d0 __asm__("d0") = (long)Border;
	register long retvalue __asm__("d0");

	__asm__ volatile
	(
		"move.l  #21,-(sp)\n\t"
		"jsr     (%1)\n\t"
		"addq.l  #4,sp"
	: "=r"(retvalue)
	: "a"(uaelib_demux), "r"(a0), "r"(a1), "r"(d0)
	: "memory", "cc"
	);

	return retvalue;
}

uae_u32 uaelib_picasso_SetSwitch(struct BoardInfo* bi, uae_u16 state)
{
	register long a0 __asm__("a0") = (long)bi;
	register short d0 __asm__("d0") = (short)state;
	register long retvalue __asm__("d0");

	__asm__ volatile
	(
		"move.l  #18,-(sp)\n\t"
		"jsr     (%1)\n\t"
		"addq.l  #4,sp"
	: "=r"(retvalue)
	: "a"(uaelib_demux), "r"(a0), "r"(d0)
	: "memory", "cc"
	);

	return retvalue;
}

uae_u32 uaelib_picasso_SetPanning(struct BoardInfo* bi, UBYTE *Memory, uae_u16 Width, WORD XOffset, WORD YOffset, RGBFTYPE RGBFormat)
{
	register long a0 __asm__("a0") = (long)bi;
	register long a1 __asm__("a1") = (long)Memory;
	register short d0 __asm__("d0") = (short)Width;
	register short d1 __asm__("d1") = (short)XOffset;
	register short d2 __asm__("d2") = (short)YOffset;
	register long d7 __asm__("d7") = (long)RGBFormat;
	register long retvalue __asm__("d0");

	__asm__ volatile
	(
		"move.l  #22,-(sp)\n\t"
		"jsr     (%1)\n\t"
		"addq.l  #4,sp"
	: "=r"(retvalue)
	: "a"(uaelib_demux), "r"(a0), "r"(a1), "r"(d0), "r"(d1), "r"(d2), "r"(d7)
	: "memory", "cc"
	);

	return retvalue;
}

uae_u32 uaelib_picasso_FillRect(struct BoardInfo* bi, struct RenderInfo* ri, WORD X, WORD Y, WORD Width, WORD Height, uae_u32 Pen, UWORD Mask, RGBFTYPE RGBFormat)
{
	register long a0 __asm__("a0") = (long)bi;
	register long a1 __asm__("a1") = (long)ri;
	register short d0 __asm__("d0") = (short)X;
	register short d1 __asm__("d1") = (short)Y;
	register short d2 __asm__("d2") = (short)Width;
	register short d3 __asm__("d3") = (short)Height;
	register long d4 __asm__("d4") = (long)Pen;
	register short d5 __asm__("d5") = (short)Mask;
	register long d7 __asm__("d7") = (long)RGBFormat;
	register long retvalue __asm__("d0");

	__asm__ volatile
	(
		"move.l  #17,-(sp)\n\t"
		"jsr     (%1)\n\t"
		"addq.l  #4,sp"
	: "=r"(retvalue)
	: "a"(uaelib_demux), "r"(a0), "r"(a1), "r"(d0), "r"(d1), "r"(d2), "r"(d3), "r"(d4), "r"(d5), "r"(d7)
	: "memory", "cc"
	);

	return retvalue;
}

uae_u32 uaelib_picasso_BlitPattern(struct BoardInfo *bi, struct RenderInfo *ri, struct Pattern *pattern, WORD X, WORD Y, WORD Width, WORD Height, WORD Mask, RGBFTYPE RGBFormat)
{
	register long a0 __asm__("a0") = (long)bi;
	register long a1 __asm__("a1") = (long)ri;
	register long a2 __asm__("a2") = (long)pattern;
	register short d0 __asm__("d0") = (short)X;
	register short d1 __asm__("d1") = (short)Y;
	register short d2 __asm__("d2") = (short)Width;
	register short d3 __asm__("d3") = (short)Height;
	register short d4 __asm__("d4") = (short)Mask;
	register long d7 __asm__("d7") = (long)RGBFormat;
	register long retvalue __asm__("d0");

	__asm__ volatile
	(
		"move.l  #30,-(sp)\n\t"
		"jsr     (%1)\n\t"
		"addq.l  #4,sp"
	: "=r"(retvalue)
	: "a"(uaelib_demux), "r"(a0), "r"(a1), "r"(a2), "r"(d0), "r"(d1), "r"(d2), "r"(d3), "r"(d4), "r"(d7)
	: "memory", "cc"
	);

	return retvalue;
}

uae_u32 uaelib_picasso_BlitRectNoMaskComplete(struct BoardInfo *bi, struct RenderInfo *srcri, struct RenderInfo *dstri, WORD SrcX, WORD SrcY, WORD DstX, WORD DstY, WORD Width, WORD Height, UWORD OpCode, RGBFTYPE RGBFormat)
{
	register long a0 __asm__("a0") = (long)bi;
	register long a1 __asm__("a1") = (long)srcri;
	register long a2 __asm__("a2") = (long)dstri;
	register short d0 __asm__("d0") = (short)SrcX;
	register short d1 __asm__("d1") = (short)SrcY;
	register short d2 __asm__("d2") = (short)DstX;
	register short d3 __asm__("d3") = (short)DstY;
	register short d4 __asm__("d4") = (short)Width;
	register short d5 __asm__("d5") = (short)Height;
	register short d6 __asm__("d6") = (short)OpCode;
	register long d7 __asm__("d7") = (long)RGBFormat;
	register long retvalue __asm__("d0");

	__asm__ volatile
	(
		"move.l  #28,-(sp)\n\t"
		"jsr     (%1)\n\t"
		"addq.l  #4,sp"
	: "=r"(retvalue)
	: "a"(uaelib_demux), "r"(a0), "r"(a1), "r"(a2), "r"(d0), "r"(d1), "r"(d2), "r"(d3), "r"(d4), "r"(d5), "r"(d6), "r"(d7)
	: "memory", "cc"
	);

	return retvalue;
}

uae_u32 uaelib_picasso_BlitTemplate(struct BoardInfo *bi, struct RenderInfo *ri, struct Template *template, WORD X, WORD Y, WORD Width, WORD Height, WORD Mask, RGBFTYPE RGBFormat)
{
	register long a0 __asm__("a0") = (long)bi;
	register long a1 __asm__("a1") = (long)ri;
	register long a2 __asm__("a2") = (long)template;
	register short d0 __asm__("d0") = (short)X;
	register short d1 __asm__("d1") = (short)Y;
	register short d2 __asm__("d2") = (short)Width;
	register short d3 __asm__("d3") = (short)Height;
	register short d4 __asm__("d4") = (short)Mask;
	register long d7 __asm__("d7") = (long)RGBFormat;
	register long retvalue __asm__("d0");

	__asm__ volatile
	(
		"move.l  #27,-(sp)\n\t"
		"jsr     (%1)\n\t"
		"addq.l  #4,sp"
	: "=r"(retvalue)
	: "a"(uaelib_demux), "r"(a0), "r"(a1), "r"(a2), "r"(d0), "r"(d1), "r"(d2), "r"(d3), "r"(d4), "r"(d7)
	: "memory", "cc"
	);

	return retvalue;
}

/* Correspondence from VDI blit mode to Picasso96 blit mode */
const uae_u8 uae_blit_opcode[] =
{
	BLIT_FALSE,
	BLIT_AND,
	BLIT_ONLYSRC,
	BLIT_SRC,
	BLIT_ONLYDST,
	BLIT_DST,
	BLIT_EOR,
	BLIT_OR,
	BLIT_NOR,
	BLIT_NEOR,
	BLIT_NOTDST,
	BLIT_NOTONLYDST,
	BLIT_NOTSRC,
	BLIT_NOTONLYSRC,
	BLIT_NAND,
	BLIT_TRUE,
};

/* Correspondence from VDI draw mode to Picasso96 draw mode */
const uae_u8 uae_drawmode[] =
{
	0,
	JAM2,
	JAM1,
	COMP,
	JAM1 | INVERS,
};
