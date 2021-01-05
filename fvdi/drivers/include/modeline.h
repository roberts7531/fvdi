#ifndef MODELINE_H
#define MODELINE_H

/* define an input variable for display timings */
struct display
{
    /* pixel clock parameters */
    double CharacterCell;         //horizontal character cell granularity
    double PClockStep;            //pixel clock stepping

    /* horizontal blanking time parameters */
    double HSyncPercent;          //horizontal sync width as % of line period

    double M;                     //M gradient (%/kHz)
    double C;                     //C offset (%)
    double K;                     //K blanking time scaling factor
    double J;                     //J scaling factor weighting

    /* vertical blanking time parameters */
    double VFrontPorch;           //minimum number of lines for front porch
    double VBackPorchPlusSync;    //minimum time for vertical sync+back porch
    double VSyncWidth;            //minimum number of lines for vertical sync
    double VBackPorch;            //minimum number of lines for back porch

    /* configuration parameters */
    double Margin;                //top/bottom MARGIN size as % of height

    /* reduced blanking parameters */
    double HBlankingTicks;        //number of clock ticks for horizontal blanking
    double HSyncTicks;            //number of clock ticks for horizontal sync
    double VBlankingTime;         //minimum vertical blanking time
};


/*
 * this is the programmatic representation of a standard X-Windows modeline
 * used within fVDI to calculate and set video resolutions
 */

struct modeline_flags
{
    short interlace : 1;
    short double_scan : 1;
    short hsync_polarity : 1;
    short vsync_polarity : 1;
};

struct modeline
{
    unsigned short pixel_clock;
    unsigned short h_display;
    unsigned short h_sync_start;
    unsigned short h_sync_end;
    unsigned short h_total;
    unsigned short v_display;
    unsigned short v_sync_start;
    unsigned short v_sync_end;
    unsigned short v_total;
    struct modeline_flags flags;
};

struct modeline *general_timing_formula(double HRes, double VRes, double Clock, double Flags);

#endif // MODELINE_H
