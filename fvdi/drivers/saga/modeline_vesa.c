/*
 * modeline_vesa.c - SAGA supported modelines
 * This is part of the SAGA driver for fVDI
 * This file comes from the SAGA Picasso96 driver:
 * https://github.com/ezrec/saga-drivers/blob/master/saga.card/modeline_vesa.c
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

#include "saga.h"
#define MODELINE_VESA 0

/* Originally adapted from AROS uaegfx.hidd timing data
 */
struct ModeInfo modeline_vesa_entry[] = {
#if MODELINE_VESA
    { 
        .Node = { .ln_Name = "VESA: 320 x 240 60Hz" },
        .Width = 320, .Height = 240,
        .Flags = GMF_HPOLARITY | GMF_VPOLARITY | GMF_DOUBLESCAN | GMF_DOUBLEVERTICAL,
        .HorTotal = 408,        .VerTotal = 262,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 32,     .VerSyncStart = 5,
        .HorSyncSize  = 24,     .VerSyncSize  = 1,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        .Node = { .ln_Name = "VESA: 640 x 480 60Hz" },
        .Width = 640, .Height = 480,
        .Flags = GMF_HPOLARITY | GMF_VPOLARITY,
        .HorTotal = 800,        .VerTotal = 525,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 40,     .VerSyncStart = 1,
        .HorSyncSize  = 88,     .VerSyncSize  = 2,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        .Node = { .ln_Name = "VESA: 800 x 600 60Hz" },
        .Width = 800, .Height = 600,
        .Flags = 0,
        .HorTotal = 1024,       .VerTotal = 625,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 56,     .VerSyncStart = 1,
        .HorSyncSize  = 40,     .VerSyncSize  = 2,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        .Node = { .ln_Name = "VESA:1024 x 768 60Hz" },
        .Width = 1024, .Height = 768,
        .Flags = 0,
        .HorTotal = 1336,       .VerTotal = 800,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 48,     .VerSyncStart = 4,
        .HorSyncSize  = 112,    .VerSyncSize  = 5,
        .Numerator = 60,        /* Refresh rate */
    },
#endif
    /* 4:3 modes */
    {
        /* umc 320 240 60 -d */ /* Pixel Clock = 12.00 MHz */
        .Node = { .ln_Name = "SAGA: 320 x 240 60Hz" },
        .Width = 320, .Height = 240,
        .Flags = GMF_VPOLARITY | GMF_DOUBLESCAN | GMF_DOUBLEVERTICAL,
        .HorTotal = 400,        .VerTotal = 250,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 8,      .VerSyncStart = 2,
        .HorSyncSize  = 32,     .VerSyncSize  = 2,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        /* umc 640 480 60 */ /* Pixel Clock = 24.05 MHz */
        .Node = { .ln_Name = "SAGA: 640 x 480 60Hz" },
        .Width = 640, .Height = 480,
        .Flags = GMF_VPOLARITY,
        .HorTotal = 800,        .VerTotal = 501,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 16,     .VerSyncStart = 4,
        .HorSyncSize  = 64,     .VerSyncSize  = 4,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        /* umc 800 600 60 */ /* Pixel Clock = 38.40 MHz */
        .Node = { .ln_Name = "SAGA: 800 x 600 60Hz" },
        .Width = 800, .Height = 600,
        .Flags = GMF_VPOLARITY,
        .HorTotal = 1024,       .VerTotal = 625,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 32,     .VerSyncStart = 4,
        .HorSyncSize  = 80,     .VerSyncSize  = 4,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        /* umc 1024 768 60 */ /* Pixel Clock = 64.35 MHz */
        .Node = { .ln_Name = "SAGA:1024 x 768 60Hz" },
        .Width = 1024, .Height = 768,
        .Flags = GMF_VPOLARITY,
        .HorTotal = 1344,       .VerTotal = 798,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 56,     .VerSyncStart = 4,
        .HorSyncSize  = 104,    .VerSyncSize  = 4,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        /* umc 1280 960 60 */ /* Pixel Clock = 102.41 MHz */
        .Node = { .ln_Name = "SAGA:1280 x 960 60Hz" },
        .Width = 1280, .Height = 960,
        .Flags = GMF_VPOLARITY,
        .HorTotal = 1712,       .VerTotal = 997,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 80,     .VerSyncStart = 4,
        .HorSyncSize  = 136,    .VerSyncSize  = 4,
        .Numerator = 60,        /* Refresh rate */
    },
    /* 16:10 modes */
    {
        /* umc 840 525 60 --rbt */ /* Pixel Clock = 32.25 MHz */
        .Node = { .ln_Name = "LG:840 x 525 60" },
        .Width = 840, .Height = 525,
        .Flags = GMF_HPOLARITY,
        .HorTotal = 1000,       .VerTotal = 540,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 48,     .VerSyncStart = 3,
        .HorSyncSize  = 32,     .VerSyncSize  = 4,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        /* umc 1440 900 30 --rbt */ /* Pixel Clock = 43.75 MHz */
        .Node = { .ln_Name = "LG:1440 x 900 30Hz" },
        .Width = 1440, .Height = 900,
        .Flags = GMF_HPOLARITY,
        .HorTotal = 1600,       .VerTotal = 913,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 48,     .VerSyncStart = 3,
        .HorSyncSize  = 32,     .VerSyncSize  = 4,
        .Numerator = 30,        /* Refresh rate */
    },
    {
        /* umc 1680 1050 30 --rbt */ /* Pixel Clock = 58.75 MHz */
        .Node = { .ln_Name = "LG:1680 x 1050 30Hz" },
        .Width = 1680, .Height = 1050,
        .Flags = GMF_HPOLARITY,
        .HorTotal = 1840,       .VerTotal = 1065,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 48,     .VerSyncStart = 3,
        .HorSyncSize  = 32,     .VerSyncSize  = 4,
        .Numerator = 30,        /* Refresh rate */
    },
    /* 16:9 modes */
    {
        /* umc 640 360 60 --rbt */ /* Pixel Clock = 17.75 MHz */
        .Node = { .ln_Name = "HD:640 x 360 60" },
        .Width = 640, .Height = 360,
        .Flags = GMF_HPOLARITY,
        .HorTotal = 800,        .VerTotal = 373,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 48,     .VerSyncStart = 3,
        .HorSyncSize  = 32,     .VerSyncSize  = 4,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        /* umc 960 540 60 --rbt */ /* Pixel Clock = 37.25 MHz */
        .Node = { .ln_Name = "HD:960 x 540 60" },
        .Width = 960, .Height = 540,
        .Flags = GMF_HPOLARITY,
        .HorTotal = 1120,       .VerTotal = 556,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 48,     .VerSyncStart = 3,
        .HorSyncSize  = 32,     .VerSyncSize  = 4,
        .Numerator = 60,        /* Refresh rate */
    },
    {
        /* umc 1280 720 24 --rbt */ /* Pixel Clock = 25.25 MHz */
        .Node = { .ln_Name = "HD:1280 x 720 24" },
        .Width = 1280, .Height = 720,
        .Flags = GMF_HPOLARITY,
        .HorTotal = 1440,       .VerTotal = 733,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 48,     .VerSyncStart = 3,
        .HorSyncSize  = 32,     .VerSyncSize  = 4,
        .Numerator = 24,        /* Refresh rate */
    },
    {
        /* umc 1408 792 24 --rbt */ /* Pixel Clock = 30.25 MHz */
        .Node = { .ln_Name = "HD:1408 x 792 24" },
        .Width = 1408, .Height = 792,
        .Flags = GMF_HPOLARITY,
        .HorTotal = 1568,       .VerTotal = 805,
        .HorBlankSize = 0,      .VerBlankSize = 0,
        .HorSyncStart = 48,     .VerSyncStart = 3,
        .HorSyncSize  = 32,     .VerSyncSize  = 4,
        .Numerator = 24,        /* Refresh rate */
    },
};

const int modeline_vesa_entries = sizeof(modeline_vesa_entry)/sizeof(modeline_vesa_entry[0]);
