/*
 * firebee.c - FireBee specific functions
 * This is part of the FireBee driver for fVDI
 *
 * https://github.com/ezrec/saga-drivers/tree/master/saga.card
 * Glued by Vincent Riviere
 * Reused for the FireBee by Markus Fröschle
 */

#include "fvdi.h"
#include "driver.h"
#include "firebee.h"
#include "fb_video.h"


static void fbee_set_clockmode(enum fb_clockmode mode)
{
    if (!(mode <= FB_CLOCK_PLL))
    {
        access->funcs.puts("error: illegal clock mode\r\n");
        for (;;);
    }

    fb_vd_cntrl = (fb_vd_cntrl & ~(FB_CLOCK_MASK)) | mode << 8;
    fb_vd_cntrl = (fb_vd_cntrl & ~(FB_CLOCK_MASK)) | mode << 8;
}

/*
 * wait for the Firebee clock generator to be idle
 */
static void wait_pll(void)
{
    do {} while (fb_vd_pll_reconfig < 0);   /* wait until video PLL not busy */
}

/*
 * set the Firebee video (pixel) clock to <clock> MHz.
 */
void fbee_set_clock(unsigned short clock)
{
    fbee_set_clockmode(FB_CLOCK_PLL);

    wait_pll();
    fb_vd_frq = clock - 1;
    wait_pll();
    fb_vd_pll_reconfig = 0;
}

void fbee_set_screen(volatile struct videl_registers *regs, void *adr)
{
    regs->vbasx = ((unsigned long) adr >> 16) & 0x3ff;
    regs->vbasm = ((unsigned long) adr >> 8) & 0xff;
    regs->vbasl = ((unsigned long) adr);
}

void fbee_set_video(short *screen_address)
{
    fbee_set_screen(&videl_regs, screen_address);

    fb_vd_cntrl &= ~(FALCON_SHIFT_MODE | ST_SHIFT_MODE | FB_VIDEO_ON | VIDEO_DAC_ON);

    /* it appears we can only enable FireBee video if we write 0 to ST shift mode
     * and Falcon shift mode in exactly this sequence
     */
    videl_regs.stsft = 0;
    videl_regs.spshift = 0;

    set_videl_regs_from_modeline(&modeline, &videl_regs);

    fb_vd_cntrl |= COLOR16 | NEG_SYNC_ALLOWED;
    fb_vd_cntrl &= ~(FALCON_SHIFT_MODE | ST_SHIFT_MODE | COLOR24 | COLOR8 | COLOR1);
    fb_vd_cntrl |= FB_VIDEO_ON | VIDEO_DAC_ON;
}

void set_videl_regs_from_modeline(struct modeline *ml, volatile struct videl_registers *vr)
{
    unsigned short left_margin = (ml->h_total - ml->h_display) / 2;
    unsigned short upper_margin = (ml->v_total - ml->v_display) / 2;

    fbee_set_clockmode(FB_CLOCK_PLL);
    fbee_set_clock(ml->pixel_clock);

    vr->hht = ml->h_total;
    vr->hde = left_margin - 1 + ml->h_display;
    vr->hbe = left_margin - 1;
    vr->hdb = left_margin;
    vr->hbb = left_margin + ml->h_display;
    vr->hss = ml->h_total - (ml->h_sync_end - ml->h_sync_start);

    vr->vft = ml->v_total;
    vr->vde = upper_margin + ml->v_display - 1;
    vr->vbe = upper_margin - 1;
    vr->vdb = upper_margin;
    vr->vbb = upper_margin + ml->v_display;

    vr->vss = ml->v_total - (ml->v_sync_end - ml->v_sync_start);
}

/***************************************************************************
 *   Copyright (C) 2005 by Desmond Colin Jones                             *
 *   http://umc.sourceforge.net/                                           *
 *                                                                         *
 *   gcc umc.c -o umc -lm -Wall                                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

UMC_DISPLAY UMC_GTF = {
    8.0,    //horizontal character cell granularity
    0.0,    //pixel clock stepping
    8.0,    //horizontal sync width% of line period
    600.0,  //M gradient (%/kHz)
    40.0,   //C offset (%)
    128.0,  //K blanking time scaling factor
    20.0,   //J scaling factor weighting
    1.0,    //number of lines for front porch
    550.0,  //time for vertical sync+back porch
    3.0,    //number of lines for vertical sync
    6.0,    //number of lines for back porch
    0.0,    //top/bottom MARGIN size as % of height
    160.0,  //clock ticks for horizontal blanking
    32.0,   //clock ticks for horizontal sync
    460.0,  //minimum vertical blanking time
};

/* define a round function to keep the source ANSI C 89 compliant */
double round(double input)
{
    return ((double) ((int) (input + 0.5)));
}

double floor(double input)
{
    return ((double) ((int) input));
}

double ceil(double input)
{
    return ((double) ((int) (input + 1.0)));
}


UMC_MODELINE Modeline[1];

UMC_MODELINE *general_timing_formula(double HRes, double VRes, double Clock,
        UMC_DISPLAY Display, double Flags)
{
    /* define Clock variables */
    double RefreshRate = 0;
    double RefreshRateEstimate = 0;
    double HClock = 0;
    double PClock = 0;

    /* horizontal calculation variables */
    double HActive = 0;
    double HSyncWidth = 0;
    double HBackPorch = 0;
    double HTotal = 0;
    double IdealDutyCycle = 0;
    double HorizontalPeriodEstimate = 0;
    double IdealHorizontalPeriod = 0;
    double HorizontalPeriod = 0;
    double HorizontalBlankingPixels = 0;

    /* vertical calculation variables */
    double Interlace = 0;
    double VSyncPlusBackPorch = 0;
    double VBackPorch = 0;
    double VTotal = 0;


    /* Margin variables */
    double TopMargin = 0;
    double BottomMargin = 0;
    double RightMargin = 0;
    double LeftMargin = 0;


    /* initialize a few Modeline variables to their default values */
    Modeline->Doublescan = 0;
    Modeline->Interlace = 0;
    Modeline->next = NULL;
    Modeline->previous = NULL;


    /* round character cell granularity to nearest integer*/
    Display.CharacterCell = round(Display.CharacterCell);

    if (Display.CharacterCell < 1)
    {
        //fprintf(stderr, "Error:  character cell less than 1 pixel.\n");
    }


    /* round number of lines in front porch to nearest integer */
    if (Flags < 0) // if doublescan mode
    {
        Display.VFrontPorch = round(Display.VFrontPorch / 2.0) * 2.0;
    }
    else
    {
        Display.VFrontPorch = floor(Display.VFrontPorch);
    }

    if (Display.VFrontPorch < 0)
    {
        // fprintf(stderr, "Error:  vertical sync start less than 0 lines.\n");
    }


    /* round number of lines in vsync width to nearest integer */
    if (Flags < 0) // if doublescan mode
    {
        Display.VSyncWidth = round(Display.VSyncWidth / 2.0) * 2.0;
    }
    else
    {
        Display.VSyncWidth = round(Display.VSyncWidth);
    }

    if (Display.VSyncWidth < 1)
    {
        // fprintf(stderr, "Error:  vertical sync width less than 1 line.\n");
    }


    /* round number of lines in back porch to nearest integer */
    if (Flags < 0) // if doublescan mode
    {
        Display.VBackPorch= round(Display.VBackPorch / 2.0) * 2.0;
    }
    else
    {
        Display.VBackPorch = round(Display.VBackPorch);
    }

    if (Display.VBackPorch < 1)
    {
        // fprintf(stderr, "Error:  vertical sync width less than 1 line.\n");
    }


    /* Calculate M, incorporating the scaling factor */
    if (Display.K == 0)
    {
        Display.K = 00.1;
    }
    Display.M = (Display.K / 256.0) * Display.M;


    /* Calculate C, incorporating the scaling factor */
    Display.C = ((Display.C - Display.J) * Display.K / 256.0) + Display.J;


    /* number of lines per field rounded to nearest integer */
    if (Flags > 0)  // if interlace mode
    {
        VRes = round(VRes / 2.0);
    }
    else if (Flags < 0)  // if doublescan mode
    {
        VRes = round(VRes) * 2;
    }
    else
    {
        VRes = round(VRes);
    }


    /* number of pixels per line rouned to nearest character cell */
    HRes = round(HRes / Display.CharacterCell) * Display.CharacterCell;


    /* calculate margins */
    if (Display.Margin > 0)
    {
        /* calculate top and bottom margins */
        TopMargin = BottomMargin = round((Display.Margin / 100.0) * VRes);


        /* calculate left and right margins */
        LeftMargin = RightMargin =
            round(HRes * (Display.Margin / 100.0) / Display.CharacterCell) *
            Display.CharacterCell;
    }


    /* calculate total number of active pixels per line */
    HActive = HRes + LeftMargin + RightMargin;


    /* set interlace variable if interlace mode */
    if (Flags > 0)  // if interlace mode
    {
        Interlace = 0.5;
    }
    else
    {
        Interlace = 0;
    }


    /***************************************************************************
     *                                                                         *
     *   Refresh rate driven calculations.                                     *
     *                                                                         *
     ***************************************************************************/
    if (Clock < 1000)
    {
        RefreshRate = Clock;


        /* calculate field refresh rate */
        if (Flags > 0)  // if interlace mode
        {
            RefreshRate = RefreshRate * 2.0;
        }


        /* estimate horizontal period */
        HorizontalPeriodEstimate =
            ((1.0 / RefreshRate) - Display.VBackPorchPlusSync / 1000000.0) /
            (VRes + TopMargin + BottomMargin + Display.VFrontPorch + Interlace) *
            1000000.0;


        /* calculate number of lines in vertical sync and back porch */
        VSyncPlusBackPorch =
            Display.VBackPorchPlusSync / HorizontalPeriodEstimate;

        if (Flags < 0) // if doublescan mode
        {
            VSyncPlusBackPorch = round(VSyncPlusBackPorch / 2.0) * 2.0;
        }
        else // default
        {
            VSyncPlusBackPorch = round(VSyncPlusBackPorch);
        }

        /***************************************************************************
         *                                                                          *
         *   The following reality check was not a part of the gtf worksheet but    *
         *   it should have been, which is why I included it.                       *
         *                                                                          *
         ****************************************************************************/
        if (VSyncPlusBackPorch < Display.VSyncWidth + Display.VBackPorch)
        {
            VSyncPlusBackPorch = Display.VSyncWidth + Display.VBackPorch;
        }


        /* calculate number of lines in back porch */
        VBackPorch = VSyncPlusBackPorch - Display.VSyncWidth;


        /* calculate total number of lines */
        VTotal =
            VRes + TopMargin + BottomMargin +
            Display.VFrontPorch + VSyncPlusBackPorch + Interlace;


        /* estimate field refresh rate*/
        RefreshRateEstimate =
            1.0 / HorizontalPeriodEstimate / VTotal * 1000000.0;


        /* calculate horizontal period */
        HorizontalPeriod =
            HorizontalPeriodEstimate / (RefreshRate / RefreshRateEstimate);


        /* calculate ideal duty cycle */
        IdealDutyCycle = Display.C - (Display.M * HorizontalPeriod / 1000.0);


        /* calculate horizontal blanking time in pixels */
        if (IdealDutyCycle < 20)
        {
            HorizontalBlankingPixels =
                floor((HActive * 20.0) / (100.0 - 20.0) /
                        (2.0 * Display.CharacterCell)) * 2.0 * Display.CharacterCell;
        }
        else
        {
            HorizontalBlankingPixels =
                round(HActive * IdealDutyCycle / (100.0 - IdealDutyCycle) /
                        (2.0 * Display.CharacterCell)) * 2.0 * Display.CharacterCell;
        }


        /* calculate total number of pixels per line */
        HTotal = HActive + HorizontalBlankingPixels;


        /* calculate pixel clock */
        PClock = HTotal / HorizontalPeriod;
        if (Display.PClockStep > 0)
        {
            PClock = floor(PClock / Display.PClockStep) * Display.PClockStep;
        }
    }


    /***************************************************************************
     *                                                                         *
     *   Horizontal clock driven calculations.                                 *
     *                                                                         *
     ***************************************************************************/
    else if (Clock < 100000)
    {
        HClock = Clock / 1000.0;

        /* calculate number of lines in vertical sync and back porch */
        VSyncPlusBackPorch =
            Display.VBackPorchPlusSync * HClock / 1000.0;

        if (Flags < 0) // if doublescan mode
        {
            VSyncPlusBackPorch = round(VSyncPlusBackPorch / 2.0) * 2.0;
        }
        else // default
        {
            VSyncPlusBackPorch = round(VSyncPlusBackPorch);
        }

        /****************************************************************************
         *                                                                          *
         *   The following reality check was not a part of the gtf worksheet but    *
         *   it should have been, which is why I included it.                       *
         *                                                                          *
         ****************************************************************************/
        if (VSyncPlusBackPorch < Display.VSyncWidth + Display.VBackPorch)
        {
            VSyncPlusBackPorch = Display.VSyncWidth + Display.VBackPorch;
        }


        /* calculate number of lines in back porch */
        VBackPorch = VSyncPlusBackPorch - Display.VSyncWidth;


        /* calculate total number of lines */
        VTotal =
            VRes + TopMargin + BottomMargin +
            Display.VFrontPorch + VSyncPlusBackPorch + Interlace;


        /* calculate ideal duty cycle */
        IdealDutyCycle = Display.C - (Display.M / HClock);


        /* calculate horizontal blanking time in pixels */
        if (IdealDutyCycle < 20)
        {
            HorizontalBlankingPixels =
                floor((HActive * 20.0) / (100.0 - 20.0) /
                        (2.0 * Display.CharacterCell)) * 2.0 * Display.CharacterCell;
        }
        else
        {
            HorizontalBlankingPixels =
                round(HActive * IdealDutyCycle / (100.0 - IdealDutyCycle) /
                        (2.0 * Display.CharacterCell)) * 2.0 * Display.CharacterCell;
        }


        /* calculate total number of pixels per line */
        HTotal = HActive + HorizontalBlankingPixels;


        /* calculate pixel clock */
        PClock = HTotal * HClock / 1000.0;
        if (Display.PClockStep > 0)
        {
            PClock = floor(PClock / Display.PClockStep) * Display.PClockStep;
        }

    }


    /***************************************************************************
     *                                                                         *
     *   Pixel clock driven calculations.                                      *
     *                                                                         *
     ***************************************************************************/
    else
    {
        PClock = Clock / 1000000.0;
        if (Display.PClockStep > 0)
        {
            PClock = floor(PClock / Display.PClockStep) * Display.PClockStep;
        }


        /* calculate ideal horizontal period */
        IdealHorizontalPeriod =
            ((Display.C - 100) + sqrt(((100 - Display.C) * (100 - Display.C)) +
                (0.4 * Display.M * (HActive + LeftMargin + RightMargin) / PClock))) /
            2.0 / Display.M * 1000.0;


        /* calculate ideal duty cycle */
        IdealDutyCycle = Display.C - (Display.M * IdealHorizontalPeriod / 1000);


        /* calculate horizontal blanking time in pixels */
        if (IdealDutyCycle < 20)
        {
            HorizontalBlankingPixels =
                floor((HActive * 20.0) / (100.0 - 20.0) /
                        (2.0 * Display.CharacterCell)) * 2.0 * Display.CharacterCell;
        }
        else
        {
            HorizontalBlankingPixels =
                round(HActive * IdealDutyCycle / (100.0 - IdealDutyCycle) /
                        (2.0 * Display.CharacterCell)) * 2.0 * Display.CharacterCell;
        }


        /* calculate total number of pixels per line */
        HTotal = HActive + HorizontalBlankingPixels;


        /* calculate horizontal clock */
        HClock = PClock / HTotal * 1000.0;


        /* calculate number of lines in vertical sync and back porch */
        VSyncPlusBackPorch =
            Display.VBackPorchPlusSync * HClock / 1000.0;

        if (Flags < 0) // if doublescan mode
        {
            VSyncPlusBackPorch = round(VSyncPlusBackPorch / 2.0) * 2.0;
        }
        else // default
        {
            VSyncPlusBackPorch = round(VSyncPlusBackPorch);
        }

        /****************************************************************************
         *                                                                          *
         *   The following reality check was not a part of the gtf worksheet but    *
         *   it should have been, which is why I included it.                       *
         *                                                                          *
         ****************************************************************************/
        if (VSyncPlusBackPorch < Display.VSyncWidth + Display.VBackPorch)
        {
            VSyncPlusBackPorch = Display.VSyncWidth + Display.VBackPorch;
        }


        /* calculate number of lines in back porch */
        VBackPorch = VSyncPlusBackPorch - Display.VSyncWidth;


        /* calculate total number of lines */
        VTotal =
            VRes + TopMargin + BottomMargin +
            Display.VFrontPorch + VSyncPlusBackPorch + Interlace;
    }


    /* calculate horizontal sync width in pixels */
    HSyncWidth =
        round(Display.HSyncPercent / 100.0 * HTotal / Display.CharacterCell) *
        Display.CharacterCell;


    /* calculate horizontal back porch in pixels */
    HBackPorch = HorizontalBlankingPixels / 2.0;


    /* output modeline calculations */
    if (Flags > 0)  // if interlace mode
    {
        Modeline->Interlace = 1;
        Modeline->PClock = PClock;
        Modeline->HRes = (int) HRes;
        Modeline->HSyncStart = (int) (HTotal - HBackPorch - HSyncWidth);
        Modeline->HSyncEnd = (int) (HTotal - HBackPorch);
        Modeline->HTotal = (int) HTotal;
        Modeline->VRes = (int) (VRes * 2.0);
        Modeline->VSyncStart = (int) ((VTotal - VSyncPlusBackPorch) * 2.0);
        Modeline->VSyncEnd = (int) ((VTotal - VBackPorch) * 2.0);
        Modeline->VTotal = (int) (VTotal * 2.0);
    }
    else if (Flags < 0)  // if doublescan mode
    {
        Modeline->Doublescan = 1;
        Modeline->PClock = PClock;
        Modeline->HRes = (int) HRes;
        Modeline->HSyncStart = (int) (HTotal - HBackPorch - HSyncWidth);
        Modeline->HSyncEnd = (int) (HTotal - HBackPorch);
        Modeline->HTotal = (int) HTotal;
        Modeline->VRes = (int) (VRes / 2.0);
        Modeline->VSyncStart = (int) ((VTotal - VSyncPlusBackPorch) / 2.0);
        Modeline->VSyncEnd = (int) ((VTotal - VBackPorch) / 2.0);
        Modeline->VTotal = (int) (VTotal / 2.0);
    }
    else
    {
        Modeline->PClock = PClock;
        Modeline->HRes = (int) HRes;
        Modeline->HSyncStart = (int) (HTotal - HBackPorch - HSyncWidth);
        Modeline->HSyncEnd = (int) (HTotal - HBackPorch);
        Modeline->HTotal = (int) HTotal;
        Modeline->VRes = (int) VRes;
        Modeline->VSyncStart = (int) (VTotal - VSyncPlusBackPorch);
        Modeline->VSyncEnd = (int) (VTotal - VBackPorch);
        Modeline->VTotal = (int) VTotal;
    }

    Modeline->HSyncPolarity = 0;
    Modeline->VSyncPolarity = 1;

    return (Modeline);
} //general_timing_formula()
