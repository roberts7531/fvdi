/*
 * fb_video.h - FireBee (& Falcon, partly) video defines
 * This is part of the FireBee driver for fVDI
 *
 * Copyright (C) 2020 Markus Fröschle, mfro@mubf.de
 */

#ifndef FB_VIDEO_H
#define FB_VIDEO_H

#include <stdint.h>


/*
 * Falcon Bus control register at 0xffff8007
 */
struct falcon_busctrl
{
    uint8_t unused          : 2;
    uint8_t bus32           : 1;
    uint8_t unused2         : 2;
    uint8_t blitter_freq    : 1;
    uint8_t unused3         : 1;
    uint8_t processor_freq  : 1;
};

struct blitter_status
{
    uint8_t busy    : 1;
    uint8_t hog     : 1;
    uint8_t smudge  : 1;
    uint8_t unused  : 1;
    uint8_t lineno  : 4;
};

struct blitter_skew
{
    uint8_t fxsr    : 1;
    uint8_t nfsr    : 1;
    uint8_t unused  : 2;
    uint8_t skew    : 4;
};

/*
 * Blitter registers at 0xffff8a00
 */
struct blitter_registers
{
    uint16_t halftone[16];
    int16_t src_x_incr;
    int16_t src_y_incr;
    volatile uint16_t *src_addr;
    uint16_t endmask_1;
    uint16_t endmask_2;
    uint16_t endmask_3;
    int16_t dst_x_incr;
    int16_t dst_y_incr;
    volatile uint16_t *dst_addr;
    volatile uint16_t x_count;
    volatile uint16_t y_count;
    uint8_t hop;
    uint8_t op;
    volatile struct blitter_status status;
    struct blitter_skew skew;
};

enum hop_values
{
    HOP_ALL_ONES            = 0,
    HOP_HALFTONE_ONLY       = 1,
    HOP_SOURCE_ONLY         = 2,
    HOP_SOURCE_AND_HALFTONE = 3,
};

struct videl_registers
{
    union {
        int16_t vbasx;              /* on the Firebee, the high byte is extended 0xffff8200 */
        struct
        {
            int8_t filler;
            int8_t vbash;           /* video base address hi  - 0xffff8201 */
        };

    };
    uint8_t rsv2;         /* 0xffff8202 */
    uint8_t vbasm;        /* video base address mid - 0xffff8203 */
    uint8_t rsv3;         /* 0xffff8204 */
    uint8_t vcnth;        /* video count register hi- 0xffff8205 */
    uint8_t rsv4;         /* 0xffff8206 */
    uint8_t vcntm;        /* video count register mid 0xffff8207 */
    uint8_t rsv5;         /* 0xffff8208 */
    uint8_t vcntl;        /* video count register lo- 0xffff8209 */
    uint8_t st_syncmode;  /* ST sync mode register  - 0xffff820a */
    uint8_t rsv6;
    uint8_t rsv7;
    uint8_t vbasl;        /* video base address lo  - 0xffff820d */
    uint16_t nextl;       /* 9 bits line width register - 0xffff820e */
    uint16_t vwrap;       /* 10 bits number of words in line - 0xffff8210 */
    uint16_t rsv8[23];    /* 0xffff8212 - 0xffff823e unused */
    uint16_t ste_col[16]; /* STE 4 plane mode clut 0xffff8240 - 0xffff825f */
    uint8_t stsft;        /* ST shift mode register - 0xffff8260 */
    uint8_t ttsft;        /* TT shifter resolution - 0xffff8262 */
    uint8_t rsv9;         /* 0xffff8263 */
    uint16_t rsv10;       /* 0xffff8264 - 0xffff8265 */
    uint16_t spshift;     /* SPSHIFT 0xffff8266 */
    uint16_t rsv11[12];   /* 0xffff8268 - 0xffff827e */
    uint16_t hhc;         /* horizontal half line counter - 0xffff8280 */
    uint16_t hht;         /* horizontal half line total - 0xffff8282 */
    uint16_t hbb;         /* horizontal blank begin - 0xffff8284 */
    uint16_t hbe;         /* horizontal blank end - 0xffff8286 */
    uint16_t hdb;         /* horizontal display begin - 0xffff8288 */
    uint16_t hde;         /* horizontal display end - 0xffff828a */
    uint16_t hss;         /* horizontal sync start - 0xffff828c */
    uint16_t hfs;         /* horizontal field sync - 0xffff828e */
    uint16_t hee;         /* horizontal equalization end - 0xffff8290 */
    /* documented in "FALCON.TXT", but obviously not implemented: */
    uint16_t vbt;         /* video burst time - 0xffff8292 - not implemented? */
    uint16_t numreq;      /* video data transfers - 0xffff8294 - not implemented? */
    uint16_t hwc;         /* horizontal word count - 0xffff8296 - not implemented? */
    uint16_t rsv12[4];    /* 0xffff8298 - 0xffff829e */
    uint16_t vfc;         /* vertical field counter - 0xffff82a0 */
    uint16_t vft;         /* vertical field total - 0xffff82a2 */
    uint16_t vbb;         /* vertical blank begin - 0xffff82a4 */
    uint16_t vbe;         /* vertical blank end - 0xffff82a6 */
    uint16_t vdb;         /* vertical display begin - 0xffff82a8 */
    uint16_t vde;         /* vertical display end - 0xffff82aa */
    uint16_t vss;         /* vertical sync start - 0xffff82ac */
    uint16_t rsv13[8];    /* 0xffff82ae - 0xffff82be */
    uint16_t vclk;        /* video clock - 0xffff82c0 */
    uint16_t vco;         /* video control  - 0xffff82c2 */
};

enum fb_vd_vcntrl_fields
{
    ATARI_SYNC = (1UL << 26),
    BORDER_ON = (1UL << 25),
    FIFO_ON = (1UL << 24),
    CONFIG_ON = (1UL << 19),
    REFRESH_ON = (1UL << 18),
    VCS = (1UL << 17),
    VCKE = (1UL << 16),
    NEG_SYNC_ALLOWED = (1UL << 15),
    CLK_CTRL = (1UL << 8),                /* 2 bits ! */
    FALCON_SHIFT_MODE = (1UL << 7),
    ST_SHIFT_MODE = (1UL << 6),
    COLOR1 = (1UL << 5),
    COLOR8 = (1UL << 4),
    COLOR16 = (1UL << 3),
    COLOR24 = (1UL << 2),
    VIDEO_DAC_ON = (1UL << 1),
    FB_VIDEO_ON = (1UL << 0)
};

enum fb_clockmode
{
    FB_CLOCK_25 = 0L,       /* 25 MHz */
    FB_CLOCK_33 = 1L,       /* 33 MHz */
    FB_CLOCK_PLL = 2L,      /* clock from PLL (fb_vd_frq) */
    FB_CLOCK_MASK = 3L
};

static const int16_t VCO_MONO = 0;
static const int16_t VCO_RGB = 1;
static const int16_t VCO_VGA = 2;
static const int16_t VCO_TV = 3;

enum falcon_vco_bits
{
    VCO_MON = (1 << 0),             /* Monitor type, see above. 2 bits! */
    VCO_BASECLOCK = (1 << 2),       /* Video base clock: 0 = 32 MHz, 1 = 25.175 MHz */
    VCO_HALFLINE_HSYNC = (1 << 3),  /* Halfline-Hsyncs from start of bottom border: 0 = off, 1 = on */
    VCO_UNUSED = (1 << 4),
    VCO_VSYNC_POL = (1 << 5),       /* Vsync polarity: 0 = vsync negative, 1 = vsync positive */
    VCO_HSYNC_POL = (1 << 6),       /* Hsync polarity: 0 = hsync negative, 1 = hsync positive */
    VCO_BUSWIDTH = (1 << 7),        /* Video bus width: 0 = 16 bits, 1 = 32 bits (Falcon) */
    VCO_HZ_OFFS = (1 << 8),         /* Hz base offset: 0 = 128 cycles (ST res), 1 = 64 cycles (all others) */
};


extern volatile uint32_t fb_vd_clut[256];
extern volatile uint32_t fb_vd_cntrl;
extern volatile uint32_t fb_vd_border;

extern volatile uint16_t fb_vd_pll_config[255];
extern volatile int16_t fb_vd_pll_reconfig;
extern volatile uint16_t fb_vd_frq;
extern volatile struct videl_registers videl_regs;

#define FB_VRAM_PHYS_OFFSET       0x40000000            /* FireBee video ram (MMU-mapped to ST RAM) has a phys offset into FPGA RAM */

extern struct modeline modeline;
extern void *screen_address;

extern struct blitter_registers blitter;
extern struct falcon_busctrl busctrl;

#endif /* FB_VIDEO_H */

