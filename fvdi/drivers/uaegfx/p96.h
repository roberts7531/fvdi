/*
 * p96.h - Definitions for Picasso96 drivers
 *
 * Most of the definitions here come from WinUAE sources.
 * https://github.com/tonioni/WinUAE/blob/master/od-win32/picasso96_win.h
 * Copyright 1997 Brian King <Brian_King@Mitel.com, Brian_King@Cloanto.com>
 *
 * Glued here by Vincent Riviere, 2017.
 */

#ifndef P96_H
#define P96_H

#include "aos.h"

/*
 * Picasso96 resolution modes (pixel formats).
 * Constants have been prefixed to avoid name clashes with fVDI.
 */
enum {
	P96_PLANAR,
	P96_CHUNKY,
	P96_HICOLOR,
	P96_TRUECOLOR,
	P96_TRUEALPHA,
	MAXMODES
};

struct BitMapExtra {
	UBYTE filler[40];
	UWORD Width;
	UWORD Height;
};

/*
 * The struct BoardInfo is an essential part of the Picasso96 driver API.
 * Unfortunately, its definition seems to be part of the Picasso96 device
 * driver documentation, which is not public.
 * Fortunately, the offsets of that structure members can be found in WinUAE
 * sources, so we can recreate it.
 * Update: Definition of struct BoardInfo can also be found in SAGA driver:
 * https://github.com/ezrec/saga-drivers/blob/master/saga.card/include/picasso96/card.h
 */

#define PSSO_BoardInfo_RegisterBase		    0
#define PSSO_BoardInfo_MemoryBase		    PSSO_BoardInfo_RegisterBase + 4
#define PSSO_BoardInfo_MemoryIOBase		    PSSO_BoardInfo_MemoryBase + 4
#define PSSO_BoardInfo_MemorySize		    PSSO_BoardInfo_MemoryIOBase + 4
#define PSSO_BoardInfo_BoardName		    PSSO_BoardInfo_MemorySize + 4
#define PSSO_BoardInfo_VBIName			    PSSO_BoardInfo_BoardName + 4
#define PSSO_BoardInfo_CardBase			    PSSO_BoardInfo_VBIName + 32
#define PSSO_BoardInfo_ChipBase			    PSSO_BoardInfo_CardBase + 4
#define PSSO_BoardInfo_ExecBase			    PSSO_BoardInfo_ChipBase + 4
#define PSSO_BoardInfo_UtilBase			    PSSO_BoardInfo_ExecBase + 4
#define PSSO_BoardInfo_HardInterrupt		    PSSO_BoardInfo_UtilBase + 4
#define PSSO_BoardInfo_SoftInterrupt		    PSSO_BoardInfo_HardInterrupt + 22 /* The HardInterrupt is 22-bytes */
#define PSSO_BoardInfo_BoardLock		    PSSO_BoardInfo_SoftInterrupt + 22 /* The SoftInterrupt is 22-bytes */
#define PSSO_BoardInfo_ResolutionsList		    PSSO_BoardInfo_BoardLock + 46 /* On the BoardLock, we were having some fun... */
#define PSSO_BoardInfo_BoardType		    PSSO_BoardInfo_ResolutionsList + 12 /* The ResolutionsList is 12-bytes */
#define PSSO_BoardInfo_PaletteChipType		    PSSO_BoardInfo_BoardType + 4
#define PSSO_BoardInfo_GraphicsControllerType	    PSSO_BoardInfo_PaletteChipType + 4
#define PSSO_BoardInfo_MoniSwitch		    PSSO_BoardInfo_GraphicsControllerType + 4
#define PSSO_BoardInfo_BitsPerCannon		    PSSO_BoardInfo_MoniSwitch + 2
#define PSSO_BoardInfo_Flags			    PSSO_BoardInfo_BitsPerCannon + 2
#define PSSO_BoardInfo_SoftSpriteFlags		    PSSO_BoardInfo_Flags + 4
#define PSSO_BoardInfo_ChipFlags		    PSSO_BoardInfo_SoftSpriteFlags + 2
#define PSSO_BoardInfo_CardFlags		    PSSO_BoardInfo_ChipFlags + 2
#define PSSO_BoardInfo_BoardNum			    PSSO_BoardInfo_CardFlags + 4
#define PSSO_BoardInfo_RGBFormats		    PSSO_BoardInfo_BoardNum + 2
#define PSSO_BoardInfo_MaxHorValue		    PSSO_BoardInfo_RGBFormats + 2
#define PSSO_BoardInfo_MaxVerValue		    PSSO_BoardInfo_MaxHorValue + MAXMODES * 2
#define PSSO_BoardInfo_MaxHorResolution		    PSSO_BoardInfo_MaxVerValue + MAXMODES * 2
#define PSSO_BoardInfo_MaxVerResolution		    PSSO_BoardInfo_MaxHorResolution + MAXMODES * 2
#define PSSO_BoardInfo_MaxMemorySize		    PSSO_BoardInfo_MaxVerResolution + MAXMODES * 2
#define PSSO_BoardInfo_MaxChunkSize		    PSSO_BoardInfo_MaxMemorySize + 4
#define PSSO_BoardInfo_MemoryClock		    PSSO_BoardInfo_MaxChunkSize + 4
#define PSSO_BoardInfo_PixelClockCount		    PSSO_BoardInfo_MemoryClock + 4

#define PSSO_BoardInfo_AllocCardMem		    PSSO_BoardInfo_PixelClockCount + MAXMODES * 4
#define PSSO_BoardInfo_FreeCardMem		    PSSO_BoardInfo_AllocCardMem + 4

#define PSSO_BoardInfo_SetSwitch		    PSSO_BoardInfo_FreeCardMem + 4

#define PSSO_BoardInfo_SetColorArray		    PSSO_BoardInfo_SetSwitch + 4

#define PSSO_BoardInfo_SetDAC			    PSSO_BoardInfo_SetColorArray + 4
#define PSSO_BoardInfo_SetGC			    PSSO_BoardInfo_SetDAC + 4
#define PSSO_BoardInfo_SetPanning		    PSSO_BoardInfo_SetGC + 4
#define PSSO_BoardInfo_CalculateBytesPerRow	    PSSO_BoardInfo_SetPanning + 4
#define PSSO_BoardInfo_CalculateMemory		    PSSO_BoardInfo_CalculateBytesPerRow + 4
#define PSSO_BoardInfo_GetCompatibleFormats	    PSSO_BoardInfo_CalculateMemory + 4
#define PSSO_BoardInfo_SetDisplay		    PSSO_BoardInfo_GetCompatibleFormats + 4

#define PSSO_BoardInfo_ResolvePixelClock	    PSSO_BoardInfo_SetDisplay + 4
#define PSSO_BoardInfo_GetPixelClock		    PSSO_BoardInfo_ResolvePixelClock + 4
#define PSSO_BoardInfo_SetClock			    PSSO_BoardInfo_GetPixelClock + 4

#define PSSO_BoardInfo_SetMemoryMode		    PSSO_BoardInfo_SetClock + 4
#define PSSO_BoardInfo_SetWriteMask		    PSSO_BoardInfo_SetMemoryMode + 4
#define PSSO_BoardInfo_SetClearMask		    PSSO_BoardInfo_SetWriteMask + 4
#define PSSO_BoardInfo_SetReadPlane		    PSSO_BoardInfo_SetClearMask + 4

#define PSSO_BoardInfo_WaitVerticalSync		    PSSO_BoardInfo_SetReadPlane + 4
#define PSSO_BoardInfo_SetInterrupt		    PSSO_BoardInfo_WaitVerticalSync + 4

#define PSSO_BoardInfo_WaitBlitter		    PSSO_BoardInfo_SetInterrupt + 4

#define PSSO_BoardInfo_ScrollPlanar		    PSSO_BoardInfo_WaitBlitter + 4
#define PSSO_BoardInfo_ScrollPlanarDefault	    PSSO_BoardInfo_ScrollPlanar + 4
#define PSSO_BoardInfo_UpdatePlanar		    PSSO_BoardInfo_ScrollPlanarDefault + 4
#define PSSO_BoardInfo_UpdatePlanarDefault	    PSSO_BoardInfo_UpdatePlanar + 4
#define PSSO_BoardInfo_BlitPlanar2Chunky	    PSSO_BoardInfo_UpdatePlanarDefault + 4
#define PSSO_BoardInfo_BlitPlanar2ChunkyDefault	    PSSO_BoardInfo_BlitPlanar2Chunky + 4

#define PSSO_BoardInfo_FillRect			    PSSO_BoardInfo_BlitPlanar2ChunkyDefault + 4
#define PSSO_BoardInfo_FillRectDefault		    PSSO_BoardInfo_FillRect + 4
#define PSSO_BoardInfo_InvertRect		    PSSO_BoardInfo_FillRectDefault + 4
#define PSSO_BoardInfo_InvertRectDefault	    PSSO_BoardInfo_InvertRect + 4
#define PSSO_BoardInfo_BlitRect			    PSSO_BoardInfo_InvertRectDefault + 4
#define PSSO_BoardInfo_BlitRectDefault		    PSSO_BoardInfo_BlitRect + 4
#define PSSO_BoardInfo_BlitTemplate		    PSSO_BoardInfo_BlitRectDefault + 4
#define PSSO_BoardInfo_BlitTemplateDefault	    PSSO_BoardInfo_BlitTemplate + 4
#define PSSO_BoardInfo_BlitPattern		    PSSO_BoardInfo_BlitTemplateDefault + 4
#define PSSO_BoardInfo_BlitPatternDefault	    PSSO_BoardInfo_BlitPattern + 4
#define PSSO_BoardInfo_DrawLine			    PSSO_BoardInfo_BlitPatternDefault + 4
#define PSSO_BoardInfo_DrawLineDefault		    PSSO_BoardInfo_DrawLine + 4
#define PSSO_BoardInfo_BlitRectNoMaskComplete	    PSSO_BoardInfo_DrawLineDefault + 4
#define PSSO_BoardInfo_BlitRectNoMaskCompleteDefault PSSO_BoardInfo_BlitRectNoMaskComplete + 4
#define PSSO_BoardInfo_BlitPlanar2Direct	    PSSO_BoardInfo_BlitRectNoMaskCompleteDefault + 4
#define PSSO_BoardInfo_BlitPlanar2DirectDefault	    PSSO_BoardInfo_BlitPlanar2Direct + 4

#define PSSO_BoardInfo_Reserved0		    PSSO_BoardInfo_BlitPlanar2DirectDefault + 4
#define PSSO_BoardInfo_Reserved0Default		    PSSO_BoardInfo_Reserved0 + 4
#define PSSO_BoardInfo_Reserved1		    PSSO_BoardInfo_Reserved0Default + 4
#define PSSO_BoardInfo_Reserved1Default		    PSSO_BoardInfo_Reserved1 + 4
#define PSSO_BoardInfo_Reserved2		    PSSO_BoardInfo_Reserved1Default + 4
#define PSSO_BoardInfo_Reserved2Default		    PSSO_BoardInfo_Reserved2 + 4
#define PSSO_BoardInfo_Reserved3		    PSSO_BoardInfo_Reserved2Default + 4
#define PSSO_BoardInfo_Reserved3Default		    PSSO_BoardInfo_Reserved3 + 4
#define PSSO_BoardInfo_Reserved4		    PSSO_BoardInfo_Reserved3Default + 4
#define PSSO_BoardInfo_Reserved4Default		    PSSO_BoardInfo_Reserved4 + 4
#define PSSO_BoardInfo_Reserved5		    PSSO_BoardInfo_Reserved4Default + 4
#define PSSO_BoardInfo_Reserved5Default		    PSSO_BoardInfo_Reserved5 + 4

#define PSSO_BoardInfo_SetDPMSLevel		    PSSO_BoardInfo_Reserved5Default + 4
#define PSSO_BoardInfo_ResetChip		    PSSO_BoardInfo_SetDPMSLevel + 4

#define PSSO_BoardInfo_GetFeatureAttrs		    PSSO_BoardInfo_ResetChip + 4

#define PSSO_BoardInfo_AllocBitMap		    PSSO_BoardInfo_GetFeatureAttrs + 4
#define PSSO_BoardInfo_FreeBitMap		    PSSO_BoardInfo_AllocBitMap + 4
#define PSSO_BoardInfo_GetBitMapAttr		    PSSO_BoardInfo_FreeBitMap + 4

#define PSSO_BoardInfo_SetSprite		    PSSO_BoardInfo_GetBitMapAttr + 4
#define PSSO_BoardInfo_SetSpritePosition	    PSSO_BoardInfo_SetSprite + 4
#define PSSO_BoardInfo_SetSpriteImage		    PSSO_BoardInfo_SetSpritePosition + 4
#define PSSO_BoardInfo_SetSpriteColor		    PSSO_BoardInfo_SetSpriteImage + 4

#define PSSO_BoardInfo_CreateFeature		    PSSO_BoardInfo_SetSpriteColor + 4
#define PSSO_BoardInfo_SetFeatureAttrs		    PSSO_BoardInfo_CreateFeature + 4
#define PSSO_BoardInfo_DeleteFeature		    PSSO_BoardInfo_SetFeatureAttrs + 4
#define PSSO_BoardInfo_SpecialFeatures		    PSSO_BoardInfo_DeleteFeature + 4

#define PSSO_BoardInfo_ModeInfo			    PSSO_BoardInfo_SpecialFeatures + 12 /* SpecialFeatures is 12-bytes */
#define PSSO_BoardInfo_RGBFormat		    PSSO_BoardInfo_ModeInfo + 4
#define PSSO_BoardInfo_XOffset			    PSSO_BoardInfo_RGBFormat + 4
#define PSSO_BoardInfo_YOffset			    PSSO_BoardInfo_XOffset + 2
#define PSSO_BoardInfo_Depth			    PSSO_BoardInfo_YOffset + 2
#define PSSO_BoardInfo_ClearMask		    PSSO_BoardInfo_Depth + 1
#define PSSO_BoardInfo_Border			    PSSO_BoardInfo_ClearMask + 1
#define PSSO_BoardInfo_Mask			    PSSO_BoardInfo_Border + 2 /* BOOL type is only 2-bytes! */
#define PSSO_BoardInfo_CLUT			    PSSO_BoardInfo_Mask + 4
#define PSSO_BoardInfo_ViewPort			    PSSO_BoardInfo_CLUT + 3*256
#define PSSO_BoardInfo_VisibleBitMap		    PSSO_BoardInfo_ViewPort + 4
#define PSSO_BoardInfo_BitMapExtra		    PSSO_BoardInfo_VisibleBitMap + 4
#define PSSO_BoardInfo_BitMapList		    PSSO_BoardInfo_BitMapExtra + 4
#define PSSO_BoardInfo_MemList			    PSSO_BoardInfo_BitMapList + 12 /* BitMapList is 12-bytes */
#define PSSO_BoardInfo_MouseX			    PSSO_BoardInfo_MemList + 12 /* MemList is 12-bytes */
#define PSSO_BoardInfo_MouseY			    PSSO_BoardInfo_MouseX + 2
#define PSSO_BoardInfo_MouseWidth		    PSSO_BoardInfo_MouseY + 2
#define PSSO_BoardInfo_MouseHeight		    PSSO_BoardInfo_MouseWidth + 1
#define PSSO_BoardInfo_MouseXOffset		    PSSO_BoardInfo_MouseHeight + 1
#define PSSO_BoardInfo_MouseYOffset		    PSSO_BoardInfo_MouseXOffset + 1
#define PSSO_BoardInfo_MouseImage		    PSSO_BoardInfo_MouseYOffset + 1
#define PSSO_BoardInfo_MousePens		    PSSO_BoardInfo_MouseImage + 4
#define PSSO_BoardInfo_MouseRect		    PSSO_BoardInfo_MousePens + 4
#define PSSO_BoardInfo_MouseChunky		    PSSO_BoardInfo_MouseRect + 8 /* MouseRect is 8-bytes */
#define PSSO_BoardInfo_MouseRendered		    PSSO_BoardInfo_MouseChunky + 4
#define PSSO_BoardInfo_MouseSaveBuffer		    PSSO_BoardInfo_MouseRendered + 4

#define PSSO_BoardInfo_ChipData			    PSSO_BoardInfo_MouseSaveBuffer + 4
#define PSSO_BoardInfo_CardData			    PSSO_BoardInfo_ChipData + 16 * 4
#define PSSO_BoardInfo_MemorySpaceBase		    PSSO_BoardInfo_CardData + 16 * 4
#define PSSO_BoardInfo_MemorySpaceSize		    PSSO_BoardInfo_MemorySpaceBase + 4
#define PSSO_BoardInfo_DoubleBufferList		    PSSO_BoardInfo_MemorySpaceSize + 4
#define PSSO_BoardInfo_SyncTime			    PSSO_BoardInfo_DoubleBufferList + 4
#define PSSO_BoardInfo_SyncPeriod		    PSSO_BoardInfo_SyncTime + 4
#define PSSO_BoardInfo_SoftVBlankPort		    PSSO_BoardInfo_SyncPeriod + 8
#define PSSO_BoardInfo_SizeOf			    PSSO_BoardInfo_SoftVBlankPort + 34

/* Fake BoardInfo structure. Here, only size matters. */
struct BoardInfo {
	UBYTE private_data[PSSO_BoardInfo_SizeOf];
};

/* This macro provides easy access to BoardInfo members */
#define BOARDINFO_MEMBER(type,name) (*(type*)((UBYTE*)&my_BoardInfo + PSSO_BoardInfo_##name))

/* ModeInfo structure reconstructed from WinUAE PSSO_ModeInfo_* offsets */
struct ModeInfo {
	struct Node Node;
	UWORD OpenCount;
	BOOL Active;
	UWORD Width;
	UWORD Height;
	UBYTE Depth;
	UBYTE Flags;
	UWORD HorTotal;
	UWORD HorBlankSize;
	UWORD HorSyncStart;
	UWORD HorSyncSize;
	UBYTE HorSyncSkew;
	UBYTE HorEnableSkew;
	UWORD VerTotal;
	UWORD VerBlankSize;
	UWORD VerSyncStart;
	UWORD VerSyncSize;
	UBYTE Numerator;
	UBYTE Denomerator;
	ULONG PixelClock;
};

/* LibResolution structure reconstructed from WinUAE SSO_LibResolution_* offsets */
struct LibResolution
{
	struct Node Node;
	char P96ID[6];
	char Name[22];
	ULONG DisplayID;
	UWORD Width;
	UWORD Height;
	UWORD Flags;
	struct ModeInfo* Modes[MAXMODES];
	struct BoardInfo* BoardInfo;
};

/*
 * Types for RGBFormat. Taken from:
 * https://github.com/tonioni/WinUAE/blob/master/include/rtgmodes.h
 */
typedef enum {
    RGBFB_NONE,		/* no valid RGB format (should not happen) */
    RGBFB_CLUT,		/* palette mode, set colors when opening screen using
			   tags or use SetRGB32/LoadRGB32(...) */
    RGBFB_R8G8B8,	/* TrueColor RGB (8 bit each) */
    RGBFB_B8G8R8,	/* TrueColor BGR (8 bit each) */
    RGBFB_R5G6B5PC,	/* HiColor16 (5 bit R, 6 bit G, 5 bit B),
			   format: gggbbbbbrrrrrggg */
    RGBFB_R5G5B5PC,	/* HiColor15 (5 bit each), format: gggbbbbb0rrrrrgg */
    RGBFB_A8R8G8B8,	/* 4 Byte TrueColor ARGB (A unused alpha channel) */
    RGBFB_A8B8G8R8,	/* 4 Byte TrueColor ABGR (A unused alpha channel) */
    RGBFB_R8G8B8A8,	/* 4 Byte TrueColor RGBA (A unused alpha channel) */
    RGBFB_B8G8R8A8,	/* 4 Byte TrueColor BGRA (A unused alpha channel) */
    RGBFB_R5G6B5,	/* HiColor16 (5 bit R, 6 bit G, 5 bit B),
			   format: rrrrrggggggbbbbb */
    RGBFB_R5G5B5,	/* HiColor15 (5 bit each), format: 0rrrrrgggggbbbbb */
    RGBFB_B5G6R5PC,	/* HiColor16 (5 bit R, 6 bit G, 5 bit B),
			   format: gggrrrrrbbbbbggg */
    RGBFB_B5G5R5PC,	/* HiColor15 (5 bit each), format: gggrrrrr0bbbbbbgg */

    /* By now, the following formats are for use with a hardware window only
       (bitmap operations may be implemented incompletely) */

    RGBFB_Y4U2V2,	/* 2 Byte TrueColor YUV (CCIR recommendation CCIR601).
			   Each two-pixel unit is stored as one longword
			   containing luminance (Y) for each of the two pixels,
			   and chrominance (U,V) for alternate pixels.
			   The missing chrominance values are generated by
			   interpolation. (Y1-U0-Y0-V0) */
    RGBFB_Y4U1V1,	/* 1 Byte TrueColor ACCUPAK. Four adjacent pixels form
			   a packet of 5 bits Y (luminance) each pixel and 6 bits
			   U and V (chrominance) shared by the four pixels */

    RGBFB_MaxFormats
} RGBFTYPE_short; /* Was RGBFTYPE. See comment below. */

/* Warning: RGBFTYPE is supposed to be 32-bit.
 * As we have short ints, we can't just use the enum name. */
typedef uae_u32 RGBFTYPE;

#define RGBFF_NONE	(1<<RGBFB_NONE)
#define RGBFF_CLUT	(1<<RGBFB_CLUT)
#define RGBFF_R8G8B8	(1<<RGBFB_R8G8B8)
#define RGBFF_B8G8R8	(1<<RGBFB_B8G8R8)
#define RGBFF_R5G6B5PC	(1<<RGBFB_R5G6B5PC)
#define RGBFF_R5G5B5PC	(1<<RGBFB_R5G5B5PC)
#define RGBFF_A8R8G8B8	(1<<RGBFB_A8R8G8B8)
#define RGBFF_A8B8G8R8	(1<<RGBFB_A8B8G8R8)
#define RGBFF_R8G8B8A8	(1<<RGBFB_R8G8B8A8)
#define RGBFF_B8G8R8A8	(1<<RGBFB_B8G8R8A8)
#define RGBFF_R5G6B5	(1<<RGBFB_R5G6B5)
#define RGBFF_R5G5B5	(1<<RGBFB_R5G5B5)
#define RGBFF_B5G6R5PC	(1<<RGBFB_B5G6R5PC)
#define RGBFF_B5G5R5PC	(1<<RGBFB_B5G5R5PC)
#define RGBFF_Y4U2V2	(1<<RGBFB_Y4U2V2)
#define RGBFF_Y4U1V1	(1<<RGBFB_Y4U1V1)

#define RGBMASK_8BIT RGBFF_CLUT
#define RGBMASK_16BIT (RGBFF_R5G6B5PC | RGBFF_B5G6R5PC | RGBFF_R5G6B5)
#define RGBMASK_15BIT (RGBFF_R5G5B5PC | RGBFF_B5G5R5PC | RGBFF_R5G5B5)
#define RGBMASK_24BIT (RGBFF_R8G8B8 | RGBFF_B8G8R8)
#define RGBMASK_32BIT (RGBFF_A8R8G8B8 | RGBFF_A8B8G8R8 | RGBFF_R8G8B8A8 | RGBFF_B8G8R8A8)

#define	RGBFF_PLANAR	RGBFF_NONE
#define	RGBFF_CHUNKY	RGBFF_CLUT

#define	RGBFB_PLANAR	RGBFB_NONE
#define	RGBFB_CHUNKY	RGBFB_CLUT

/*
 * Other structures from:
 * https://github.com/tonioni/WinUAE/blob/master/od-win32/picasso96_win.h
 */

struct RenderInfo {
	uae_u8 *Memory;
	uae_s16 BytesPerRow;
	uae_s16 pad;
	RGBFTYPE RGBFormat;
};

struct Pattern {
	uae_u8 *Memory;
	uae_u16 XOffset, YOffset;
	uae_u32 FgPen, BgPen;
	uae_u8 Size; /* Width: 16, Height: (1<<pat_Size) */
	uae_u8 DrawMode;
};

/*
 * Blitter opcodes from:
 * https://github.com/tonioni/WinUAE/blob/master/od-win32/picasso96_win.cpp
 */
typedef enum {
	BLIT_FALSE,
	BLIT_NOR,
	BLIT_ONLYDST,
	BLIT_NOTSRC,
	BLIT_ONLYSRC,
	BLIT_NOTDST,
	BLIT_EOR,
	BLIT_NAND,
	BLIT_AND,
	BLIT_NEOR,
	BLIT_DST,
	BLIT_NOTONLYSRC,
	BLIT_SRC,
	BLIT_NOTONLYDST,
	BLIT_OR,
	BLIT_TRUE,
	BLIT_SWAP = 30
} BLIT_OPCODE;

/*
 * DrawMode bits from:
 * https://github.com/tonioni/WinUAE/blob/master/od-win32/picasso96_win.h
 */
#define JAM1 0
#define JAM2 1
#define COMP 2
#define INVERS 4

struct Template {
	uae_u8 *Memory;
	uae_s16 BytesPerRow;
	uae_u8 XOffset;
	uae_u8 DrawMode;
	uae_u32 FgPen;
	uae_u32 BgPen;
};

#endif /* P96_H */
