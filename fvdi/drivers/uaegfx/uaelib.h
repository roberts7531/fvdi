/*
 * uaelib.h - Bindings to UAE Library Picasso96 functions
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

#ifndef UAELIB_H
#define UAELIB_H

#include "p96.h"

BOOL uaelib_init(void);
uae_u32 uaelib_picasso_FindCard(struct BoardInfo* bi);
uae_u32 uaelib_picasso_InitCard(struct BoardInfo* bi);
uae_u32 uaelib_picasso_SetGC(struct BoardInfo* bi, struct ModeInfo* mi, uae_u32 Border);
uae_u32 uaelib_picasso_SetSwitch(struct BoardInfo* bi, uae_u16 state);
uae_u32 uaelib_picasso_SetPanning(struct BoardInfo* bi, UBYTE *Memory, uae_u16 Width, WORD XOffset, WORD YOffset, RGBFTYPE RGBFormat);
uae_u32 uaelib_picasso_FillRect(struct BoardInfo* boardinfo, struct RenderInfo* renderinfo, WORD X, WORD Y, WORD Width, WORD Height, uae_u32 Pen, UWORD Mask, RGBFTYPE RGBFormat);
uae_u32 uaelib_picasso_BlitPattern(struct BoardInfo *bi, struct RenderInfo *ri, struct Pattern *pattern, WORD X, WORD Y, WORD Width, WORD Height, WORD Mask, RGBFTYPE RGBFormat);
uae_u32 uaelib_picasso_BlitRectNoMaskComplete(struct BoardInfo *bi, struct RenderInfo *srcri, struct RenderInfo *dstri, WORD SrcX, WORD SrcY, WORD DstX, WORD DstY, WORD Width, WORD Height, UWORD OpCode, RGBFTYPE RGBFormat);
uae_u32 uaelib_picasso_BlitTemplate(struct BoardInfo *bi, struct RenderInfo *ri, struct Template *template, WORD X, WORD Y, WORD Width, WORD Height, WORD Mask, RGBFTYPE RGBFormat);

/* Correspondence from VDI to Picasso96 codes */
extern const uae_u8 uae_blit_opcode[];
extern const uae_u8 uae_drawmode[];

/* Global variables defined somewhere */
extern struct BoardInfo my_BoardInfo;
extern RGBFTYPE my_RGBFormat;

#endif /* UAELIB_H */
