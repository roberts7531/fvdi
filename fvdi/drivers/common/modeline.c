#include "modeline.h"


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

/* define a round function to keep the source ANSI C 89 compliant */
static inline double round(double input)
{
    return ((double) ((int) (input + 0.5)));
}

static inline double floor(double input)
{
    return ((double) ((int) input));
}

static inline double ceil(double input)
{
    return ((double) ((int) (input + 1.0)));
}

static inline double sqrt(const double fg)
{
    double n = fg / 2.0;
    double lstX = 0.0;

    while (n != lstX)
    {
        lstX = n;
        n = (n + fg/n) / 2.0;
    }
    return n;
}

static struct display dsp =
{
    .CharacterCell = 8.0,
    .PClockStep = 0.0,
    .HSyncPercent = 8.0,
    .M = 600.0,
    .C = 40.0,
    .K = 128.0,
    .J = 20.0,
    .VFrontPorch = 1.0,
    .VBackPorchPlusSync = 550.0,
    .VSyncWidth = 3.0,
    .VBackPorch = 6.0,
    .Margin = 0.0,
    .HBlankingTicks = 160.0,
    .HSyncTicks = 32.0,
    .VBlankingTime = 460.0
};

void general_timing_formula(double HRes, double VRes, double Clock, double Flags, struct modeline *modeline)
{
    struct display *Display = &dsp;

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


    /* initialize a few modeline variables to their default values */
    modeline->flags.double_scan = 0;
    modeline->flags.interlace = 0;


    /* round character cell granularity to nearest integer*/
    Display->CharacterCell = round(Display->CharacterCell);

    if (Display->CharacterCell < 1)
    {
        //fprintf(stderr, "Error:  character cell less than 1 pixel.\n");
    }


    /* round number of lines in front porch to nearest integer */
    if (Flags < 0) // if doublescan mode
    {
        Display->VFrontPorch = round(Display->VFrontPorch / 2.0) * 2.0;
    }
    else
    {
        Display->VFrontPorch = floor(Display->VFrontPorch);
    }

    if (Display->VFrontPorch < 0)
    {
        // fprintf(stderr, "Error:  vertical sync start less than 0 lines.\n");
    }


    /* round number of lines in vsync width to nearest integer */
    if (Flags < 0) // if doublescan mode
    {
        Display->VSyncWidth = round(Display->VSyncWidth / 2.0) * 2.0;
    }
    else
    {
        Display->VSyncWidth = round(Display->VSyncWidth);
    }

    if (Display->VSyncWidth < 1)
    {
        // fprintf(stderr, "Error:  vertical sync width less than 1 line.\n");
    }


    /* round number of lines in back porch to nearest integer */
    if (Flags < 0) // if doublescan mode
    {
        Display->VBackPorch= round(Display->VBackPorch / 2.0) * 2.0;
    }
    else
    {
        Display->VBackPorch = round(Display->VBackPorch);
    }

    if (Display->VBackPorch < 1)
    {
        // fprintf(stderr, "Error:  vertical sync width less than 1 line.\n");
    }


    /* Calculate M, incorporating the scaling factor */
    if (Display->K == 0)
    {
        Display->K = 00.1;
    }
    Display->M = (Display->K / 256.0) * Display->M;


    /* Calculate C, incorporating the scaling factor */
    Display->C = ((Display->C - Display->J) * Display->K / 256.0) + Display->J;


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
    HRes = round(HRes / Display->CharacterCell) * Display->CharacterCell;


    /* calculate margins */
    if (Display->Margin > 0)
    {
        /* calculate top and bottom margins */
        TopMargin = BottomMargin = round((Display->Margin / 100.0) * VRes);


        /* calculate left and right margins */
        LeftMargin = RightMargin =
            round(HRes * (Display->Margin / 100.0) / Display->CharacterCell) *
            Display->CharacterCell;
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
            ((1.0 / RefreshRate) - Display->VBackPorchPlusSync / 1000000.0) /
            (VRes + TopMargin + BottomMargin + Display->VFrontPorch + Interlace) *
            1000000.0;


        /* calculate number of lines in vertical sync and back porch */
        VSyncPlusBackPorch =
            Display->VBackPorchPlusSync / HorizontalPeriodEstimate;

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
        if (VSyncPlusBackPorch < Display->VSyncWidth + Display->VBackPorch)
        {
            VSyncPlusBackPorch = Display->VSyncWidth + Display->VBackPorch;
        }


        /* calculate number of lines in back porch */
        VBackPorch = VSyncPlusBackPorch - Display->VSyncWidth;


        /* calculate total number of lines */
        VTotal =
            VRes + TopMargin + BottomMargin +
            Display->VFrontPorch + VSyncPlusBackPorch + Interlace;


        /* estimate field refresh rate*/
        RefreshRateEstimate =
            1.0 / HorizontalPeriodEstimate / VTotal * 1000000.0;


        /* calculate horizontal period */
        HorizontalPeriod =
            HorizontalPeriodEstimate / (RefreshRate / RefreshRateEstimate);


        /* calculate ideal duty cycle */
        IdealDutyCycle = Display->C - (Display->M * HorizontalPeriod / 1000.0);


        /* calculate horizontal blanking time in pixels */
        if (IdealDutyCycle < 20)
        {
            HorizontalBlankingPixels =
                floor((HActive * 20.0) / (100.0 - 20.0) /
                        (2.0 * Display->CharacterCell)) * 2.0 * Display->CharacterCell;
        }
        else
        {
            HorizontalBlankingPixels =
                round(HActive * IdealDutyCycle / (100.0 - IdealDutyCycle) /
                        (2.0 * Display->CharacterCell)) * 2.0 * Display->CharacterCell;
        }


        /* calculate total number of pixels per line */
        HTotal = HActive + HorizontalBlankingPixels;


        /* calculate pixel clock */
        PClock = HTotal / HorizontalPeriod;
        if (Display->PClockStep > 0)
        {
            PClock = floor(PClock / Display->PClockStep) * Display->PClockStep;
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
            Display->VBackPorchPlusSync * HClock / 1000.0;

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
        if (VSyncPlusBackPorch < Display->VSyncWidth + Display->VBackPorch)
        {
            VSyncPlusBackPorch = Display->VSyncWidth + Display->VBackPorch;
        }


        /* calculate number of lines in back porch */
        VBackPorch = VSyncPlusBackPorch - Display->VSyncWidth;


        /* calculate total number of lines */
        VTotal =
            VRes + TopMargin + BottomMargin +
            Display->VFrontPorch + VSyncPlusBackPorch + Interlace;


        /* calculate ideal duty cycle */
        IdealDutyCycle = Display->C - (Display->M / HClock);


        /* calculate horizontal blanking time in pixels */
        if (IdealDutyCycle < 20)
        {
            HorizontalBlankingPixels =
                floor((HActive * 20.0) / (100.0 - 20.0) /
                        (2.0 * Display->CharacterCell)) * 2.0 * Display->CharacterCell;
        }
        else
        {
            HorizontalBlankingPixels =
                round(HActive * IdealDutyCycle / (100.0 - IdealDutyCycle) /
                        (2.0 * Display->CharacterCell)) * 2.0 * Display->CharacterCell;
        }


        /* calculate total number of pixels per line */
        HTotal = HActive + HorizontalBlankingPixels;


        /* calculate pixel clock */
        PClock = HTotal * HClock / 1000.0;
        if (Display->PClockStep > 0)
        {
            PClock = floor(PClock / Display->PClockStep) * Display->PClockStep;
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
        if (Display->PClockStep > 0)
        {
            PClock = floor(PClock / Display->PClockStep) * Display->PClockStep;
        }


        /* calculate ideal horizontal period */
        IdealHorizontalPeriod =
            ((Display->C - 100) + sqrt(((100 - Display->C) * (100 - Display->C)) +
                (0.4 * Display->M * (HActive + LeftMargin + RightMargin) / PClock))) /
            2.0 / Display->M * 1000.0;


        /* calculate ideal duty cycle */
        IdealDutyCycle = Display->C - (Display->M * IdealHorizontalPeriod / 1000);


        /* calculate horizontal blanking time in pixels */
        if (IdealDutyCycle < 20)
        {
            HorizontalBlankingPixels =
                floor((HActive * 20.0) / (100.0 - 20.0) /
                        (2.0 * Display->CharacterCell)) * 2.0 * Display->CharacterCell;
        }
        else
        {
            HorizontalBlankingPixels =
                round(HActive * IdealDutyCycle / (100.0 - IdealDutyCycle) /
                        (2.0 * Display->CharacterCell)) * 2.0 * Display->CharacterCell;
        }


        /* calculate total number of pixels per line */
        HTotal = HActive + HorizontalBlankingPixels;


        /* calculate horizontal clock */
        HClock = PClock / HTotal * 1000.0;


        /* calculate number of lines in vertical sync and back porch */
        VSyncPlusBackPorch =
            Display->VBackPorchPlusSync * HClock / 1000.0;

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
        if (VSyncPlusBackPorch < Display->VSyncWidth + Display->VBackPorch)
        {
            VSyncPlusBackPorch = Display->VSyncWidth + Display->VBackPorch;
        }


        /* calculate number of lines in back porch */
        VBackPorch = VSyncPlusBackPorch - Display->VSyncWidth;


        /* calculate total number of lines */
        VTotal =
            VRes + TopMargin + BottomMargin +
            Display->VFrontPorch + VSyncPlusBackPorch + Interlace;
    }


    /* calculate horizontal sync width in pixels */
    HSyncWidth =
        round(Display->HSyncPercent / 100.0 * HTotal / Display->CharacterCell) *
        Display->CharacterCell;


    /* calculate horizontal back porch in pixels */
    HBackPorch = HorizontalBlankingPixels / 2.0;


    /* output modeline calculations */
    if (Flags > 0)  // if interlace mode
    {
        modeline->flags.interlace = 1;
        modeline->pixel_clock = (unsigned short) PClock;
        modeline->h_display = (unsigned short) HRes;
        modeline->h_sync_start = (unsigned short) (HTotal - HBackPorch - HSyncWidth);
        modeline->h_sync_end = (unsigned short) (HTotal - HBackPorch);
        modeline->h_total = (unsigned short) HTotal;
        modeline->v_display = (unsigned short) (VRes * 2.0);
        modeline->v_sync_start = (unsigned short) ((VTotal - VSyncPlusBackPorch) * 2.0);
        modeline->v_sync_end = (unsigned short) ((VTotal - VBackPorch) * 2.0);
        modeline->v_total = (unsigned short) (VTotal * 2.0);
    }
    else if (Flags < 0)  // if doublescan mode
    {
        modeline->flags.double_scan = 1;
        modeline->pixel_clock = (unsigned short) PClock;
        modeline->h_display = (unsigned short) HRes;
        modeline->h_sync_start = (unsigned short) (HTotal - HBackPorch - HSyncWidth);
        modeline->h_sync_end = (unsigned short) (HTotal - HBackPorch);
        modeline->h_total = (unsigned short) HTotal;
        modeline->v_display = (unsigned short) (VRes / 2.0);
        modeline->v_sync_start = (unsigned short) ((VTotal - VSyncPlusBackPorch) / 2.0);
        modeline->v_sync_end = (unsigned short) ((VTotal - VBackPorch) / 2.0);
        modeline->v_total = (unsigned short) (VTotal / 2.0);
    }
    else
    {
        modeline->pixel_clock = (unsigned short)PClock;
        modeline->h_display = (unsigned short) HRes;
        modeline->h_sync_start = (unsigned short) (HTotal - HBackPorch - HSyncWidth);
        modeline->h_sync_end = (unsigned short) (HTotal - HBackPorch);
        modeline->h_total = (unsigned short) HTotal;
        modeline->v_display = (unsigned short) VRes;
        modeline->v_sync_start = (unsigned short) (VTotal - VSyncPlusBackPorch);
        modeline->v_sync_end = (unsigned short) (VTotal - VBackPorch);
        modeline->v_total = (unsigned short) VTotal;
    }

    modeline->flags.hsync_polarity = 0;
    modeline->flags.vsync_polarity = 1;

    return (modeline);
} //general_timing_formula()
