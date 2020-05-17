/*
 * fb_video.h - FireBee (& Falcon, partly) video defines
 * This is part of the FireBee driver for fVDI
 *
 * Copyright (C) 2020 Markus Fröschle
 */

#ifndef FB_VIDEO_H
#define FB_VIDEO_H


/*
 * Falcon (and FireBee) Videl register set.
 *
 * Falcon registers are mostly 8 or 9 bit while FireBee
 * registers are extended to 10 or 11 bits.
 */

struct videl_registers
{
    union
    {
        short vbasx;            /* only on the Firebee, the high byte is extended 0xffff8200 */
        struct
        {
            char filler;
            char vbash;         /* video base address hi  - 0xffff8201 */
        };

    };
    unsigned char rsv2;         /* 0xffff8202 */
    unsigned char vbasm;        /* video base address mid - 0xffff8203 */
    unsigned char rsv3;         /* 0xffff8204 */
    unsigned char vcnth;        /* video count register hi- 0xffff8205 */
    unsigned char rsv4;         /* 0xffff8206 */
    unsigned char vcntm;        /* video count register mid 0xffff8207 */
    unsigned char rsv5;         /* 0xffff8208 */
    unsigned char vcntl;        /* video count register lo- 0xffff8209 */
    unsigned char st_syncmode;  /* ST sync mode register  - 0xffff820a */
    unsigned char rsv6;
    unsigned char rsv7;
    unsigned char vbasl;        /* video base address lo  - 0xffff820d */
    unsigned short nextl;       /* 9 bits line width register - 0xffff820e */
    unsigned short vwrap;       /* 10 bits number of words in line - 0xffff8210 */
    unsigned short rsv8[23];    /* 0xffff8212 - 0xffff823e unused */
    unsigned short ste_col[16]; /* STE 4 plane mode clut 0xffff8240 - 0xffff825f */
    unsigned char stsft;        /* ST shift mode register - 0xffff8260 */
    unsigned char ttsft;        /* TT shifter resolution - 0xffff8262 */
    unsigned char rsv9;         /* 0xffff8263 */
    unsigned short rsv10;       /* 0xffff8264 - 0xffff8265 */
    unsigned short spshift;     /* SPSHIFT 0xffff8266 */
    unsigned short rsv11[12];   /* 0xffff8268 - 0xffff827e */
    unsigned short hhc;         /* horizontal half line counter - 0xffff8280 */
    unsigned short hht;         /* horizontal half line total - 0xffff8282 */
    unsigned short hbb;         /* horizontal blank begin - 0xffff8284 */
    unsigned short hbe;         /* horizontal blank end - 0xffff8286 */
    unsigned short hdb;         /* horizontal display begin - 0xffff8288 */
    unsigned short hde;         /* horizontal display end - 0xffff828a */
    unsigned short hss;         /* horizontal sync start - 0xffff828c */
    unsigned short hfs;         /* horizontal field sync - 0xffff828e */
    unsigned short hee;         /* horizontal equalization end - 0xffff8290 */
    /* documented in "FALCON.TXT", but obviously not implemented */
    unsigned short vbt;         /* video burst time - 0xffff8292 - not implemented? */
    unsigned short numreq;      /* video data transfers - 0xffff8294 - not implemented? */
    unsigned short hwc;         /* horizontal word count - 0xffff8296 - not implemented? */
    unsigned short rsv12[4];    /* 0xffff8298 - 0xffff829e */
    unsigned short vfc;         /* vertical field counter - 0xffff82a0 */
    unsigned short vft;         /* vertical field total - 0xffff82a2 */
    unsigned short vbb;         /* vertical blank begin - 0xffff82a4 */
    unsigned short vbe;         /* vertical blank end - 0xffff82a6 */
    unsigned short vdb;         /* vertical display begin - 0xffff82a8 */
    unsigned short vde;         /* vertical display end - 0xffff82aa */
    unsigned short vss;         /* vertical sync start - 0xffff82ac */
    unsigned short rsv13[8];    /* 0xffff82ae - 0xffff82be */
    unsigned short vclk;        /* video clock - 0xffff82c0 */
    unsigned short vco;         /* video control  - 0xffff82c2 */
};

/*
 * bit constants for the fb_vd_cntrl FireBee hardware register
 *
 * Need to be defined as long constants since we are compiling
 * -mshort (enum would otherwise end up as 16 bit constants)
 */
enum fb_vd_vcntrl_fields
{
    ATARI_SYNC = (1L << 26),
    BORDER_ON = (1L << 25),
    FIFO_ON = (1L << 24),
    CONFIG_ON = (1L << 19),
    REFRESH_ON = (1L << 18),
    VCS = (1L << 17),
    VCKE = (1L << 16),
    NEG_SYNC_ALLOWED = (1 << 15),
    CLK_CTRL = (1 << 8),                /* 2 bits ! */
    FALCON_SHIFT_MODE = (1 << 7),
    ST_SHIFT_MODE = (1 << 6),
    COLOR1 = (1 << 5),
    COLOR8 = (1 << 4),
    COLOR16 = (1 << 3),
    COLOR24 = (1 << 2),
    VIDEO_DAC_ON = (1 << 1),
    FB_VIDEO_ON = (1 << 0),
};

enum fb_clockmode
{
    FB_CLOCK_25 = 0,        /* 25 MHz */
    FB_CLOCK_33 = 1,        /* 33 MHz */
    FB_CLOCK_PLL = 2,       /* clock from PLL (fb_vd_frq) */
    FB_CLOCK_MASK = 3
};

/*
 * FireBee hardware registers. Defined (as linker symbol definitions) from
 * Makefile on linker command line
 */
extern unsigned long FPGA_CS2_BASE;

extern volatile unsigned long fb_vd_clut[256];
extern volatile unsigned long fb_vd_cntrl;
extern volatile unsigned long fb_vd_border;

extern volatile unsigned short fb_vd_pll_config[255];
extern volatile short fb_vd_pll_reconfig;
extern volatile unsigned short fb_vd_frq;
extern volatile struct videl_registers videl_regs;
extern volatile unsigned char VRAM[];

#endif /* FB_VIDEO_H */

