/*
 * fVDI colour handling
 *
 * Copyright 2005, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

#include "fvdi.h"
#include "stdio.h"
#include "function.h"
#include "relocate.h"
#include "utility.h"

#define neg_pal_n  9


/* Necessary intermediate function, for now. */
extern set_palette(Virtual *vwk, DrvPalette *palette_pars);


static Colour *get_clut(Virtual *vwk)
{
    Colour *palette;
    Workstation *wk;
    char *addr;

    wk = vwk->real_address;
    palette = vwk->palette;
    if (wk->driver->device->clut == 1)          /* Hardware CLUT? (used to test look_up_table) */
        palette = wk->screen.palette.colours;     /* Actually a common global */
    else if (!palette || ((long)palette & 1)) { /* No or only negative palette allocated? */
        addr = malloc((wk->screen.palette.size + neg_pal_n) * sizeof(Colour));
        if (!addr) {                              /* If no memory for local palette, */
            palette = wk->screen.palette.colours;   /*  modify in global (BAD!) */
            PUTS("Could not allocate space for palette!\n");
        } else
        {
            if (!palette)
            {
                /* No palette allocated? */
                palette = vwk->palette = (Colour *)(addr + neg_pal_n * sizeof(Colour));   /* Point to index 0 */
            } else
            {                           /* Only negative palette allocated so far? */
                palette = (Colour *)((long)palette & ~1);  /* Copy the negative side first and free it */
                vwk->palette = (Colour *)(addr + neg_pal_n * sizeof(Colour));
                copymem_aligned(palette - neg_pal_n, addr, neg_pal_n * sizeof(Colour));
                free(palette);
                palette = vwk->palette;
            }
            copymem_aligned(wk->screen.palette.colours, palette, wk->screen.palette.size * sizeof(Colour));
        }
    }

    return palette;
}


void lib_vs_color(Virtual *vwk, long pen, RGB *values)
{
    Workstation *wk = vwk->real_address;
    Colour *palette;
    DrvPalette palette_pars;

    if (pen >= wk->screen.palette.size)
        return;

    palette = get_clut(vwk);

    palette_pars.first_pen = pen;
    palette_pars.count = 1;             /* One colour to set up */
    palette_pars.requested = (short *)values;
    palette_pars.palette = palette;

    set_palette(vwk, &palette_pars);
}


static int idx2vdi(Workstation *wk, int index)
{
    static signed char vdi_colours[] = { 0, 2, 3, 6, 4, 7, 5, 8, 9, 10, 11, 14, 12, 15, 13, -1 };
    int ret;

    if ((unsigned int) index >= (unsigned int) wk->screen.palette.size)
        return -1;

    /* No VDI->TOS conversion for true colour */
    if (wk->driver->device->clut != 1)  /* Hardware CLUT? (used to test look_up_table) */
        ret = index;
    else if (index == wk->screen.palette.size - 1)
        ret = 1;
    else if ((index >= 16) && (index != 255))
        ret = index;
    else
    {
        ret = vdi_colours[index];
        if (ret < 0)
            ret = wk->screen.palette.size - 1;
    }

    return ret;
}


static int vdi2idx(Workstation *wk, int vdi_pen)
{
    static signed char tos_colours[] = { 0, -1, 1, 2, 4, 6, 3, 5, 7, 8, 9, 10, 12, 14, 11, 13 };
    int ret;

    if ((unsigned int)vdi_pen >= (unsigned int)wk->screen.palette.size)
        return -1;

    if (wk->driver->device->clut != 1)
        ret = vdi_pen;
    else if (vdi_pen == 255)
        ret = 15;
    else if (vdi_pen >= 16)
        ret = vdi_pen;
    else
    {
        ret = tos_colours[vdi_pen];
        if (ret < 0)
            ret = wk->screen.palette.size - 1;
    }

    return ret;
}


int lib_vq_color(Virtual *vwk, long pen, long flag, RGB *colour)
{
    int index;
    Colour *palette;

    index = vdi2idx(vwk->real_address, pen);
    if (index < 0)
        return -1;

    palette = vwk->palette;
    /* Negative indices are always in local palette, but this can't be one of those */
    if (!palette || ((long)palette & 1))
        palette = vwk->real_address->screen.palette.colours;

    if (flag == 0)
    {
        colour->red = palette[index].vdi.red;
        colour->green = palette[index].vdi.green;
        colour->blue = palette[index].vdi.blue;
    } else
    {
        colour->red = palette[index].hw.red;
        colour->green = palette[index].hw.green;
        colour->blue = palette[index].hw.blue;
    }

    return pen;
}


static int fg_bg_index(Virtual *vwk, int subfunction, short **fg, short **bg)
{
    switch (subfunction)
    {
    case 0:
        *fg = &vwk->text.colour.foreground;
        *bg = &vwk->text.colour.background;
        break;

    case 1:
        *fg = &vwk->fill.colour.foreground;
        *bg = &vwk->fill.colour.background;
        break;

    case 2:
        *fg = &vwk->line.colour.foreground;
        *bg = &vwk->line.colour.background;
        break;

    case 3:
        *fg = &vwk->marker.colour.foreground;
        *bg = &vwk->marker.colour.background;
        break;

    case 4:
        /* This will be for bitmaps */
        return 0;
        break;

    default:
        return 0;
    }

    return 1;
}


int lib_vs_fg_color(Virtual *vwk, long subfunction, long colour_space, COLOR_ENTRY *values)
{
    short *fg, *bg, index;
    Colour *palette;
    void *addr;
    DrvPalette palette_pars;

    if ((unsigned int)colour_space > 1)    /* Only 0 or 1 allowed for now (current or RGB) */
        return 0;

    if (!fg_bg_index(vwk, subfunction, &fg, &bg))
        return -1;
    index = *fg = -subfunction * 2 - 2;      /* Index -2/-4... */

    palette = vwk->palette;
    if (!palette)
    {
        addr = malloc(neg_pal_n * sizeof(Colour));
        if (!addr)
        {
            PUTS("Could not allocate space for negative palette!\n");
            return -1;
        }
        palette = vwk->palette = (Colour *)(((long)addr + neg_pal_n * sizeof(Colour)) | 1);   /* Point to index 0 */
    }
    palette = (Colour *)((long)palette & ~1);

    palette_pars.first_pen = index;
    palette_pars.count = 1;             /* One colour to set up */
    palette_pars.requested = (short *)((long)values | 1);    /* Odd for new style entries */
    palette_pars.palette = palette;

    set_palette(vwk, &palette_pars);

    return 1;
}


int lib_vs_bg_color(Virtual *vwk, long subfunction, long colour_space, COLOR_ENTRY *values)
{
    short *fg, *bg, index;
    Colour *palette;
    void *addr;
    DrvPalette palette_pars;

    if ((unsigned int)colour_space > 1)    /* Only 0 or 1 allowed for now (current or RGB) */
        return 0;

    if (!fg_bg_index(vwk, subfunction, &fg, &bg))
        return -1;
    index = *bg = -subfunction * 2 - 3;      /* Index -3/-5... */

    palette = vwk->palette;
    if (!palette)
    {
        addr = malloc(neg_pal_n * sizeof(Colour));
        if (!addr)
        {
            PUTS("Could not allocate space for negative palette!\n");
            return -1;
        }
        palette = vwk->palette = (Colour *)(((long)addr + neg_pal_n * sizeof(Colour)) | 1);   /* Point to index 0 */
    }
    palette = (Colour *)((long)palette & ~1);

    palette_pars.first_pen = index;
    palette_pars.count = 1;             /* One colour to set up */
    palette_pars.requested = (short *)((long)values | 1);    /* Odd for new style entries */
    palette_pars.palette = palette;

    set_palette(vwk, &palette_pars);

    return 1;
}


long lib_vq_fg_color(Virtual *vwk, long subfunction, COLOR_ENTRY *colour)
{
    short *fg, *bg, index;
    Colour *palette;

    if (!fg_bg_index(vwk, subfunction, &fg, &bg))
        return -1;
    index = *fg;

    palette = vwk->palette;
    if (!palette || (((long)palette & 1) && (index >= 0)))   /* No or only part local? */
        palette = vwk->real_address->screen.palette.colours;
    palette = (Colour *)((long)palette & ~1);

    colour->rgb.reserved = 0;
    colour->rgb.red = palette[index].vdi.red;
    colour->rgb.green = palette[index].vdi.green;
    colour->rgb.blue = palette[index].vdi.blue;

    return 1;    /* RGB_SPACE */
}


long lib_vq_bg_color(Virtual *vwk, long subfunction, COLOR_ENTRY *colour)
{
    short *fg, *bg, index;
    Colour *palette;

    if (!fg_bg_index(vwk, subfunction, &fg, &bg))
        return -1;
    index = *bg;

    palette = vwk->palette;
    if (!palette || (((long)palette & 1) && (index >= 0)))   /* No or only part local? */
        palette = vwk->real_address->screen.palette.colours;
    palette = (Colour *)((long)palette & ~1);

    colour->rgb.reserved = 0;
    colour->rgb.red = palette[index].vdi.red;
    colour->rgb.green = palette[index].vdi.green;
    colour->rgb.blue = palette[index].vdi.blue;

    return 1;    /* RGB_SPACE */
}


int colour_entry(Virtual *vwk, long subfunction, short *intin, short *intout)
{
    (void) vwk;
    (void) intin;
    switch (subfunction)
    {
    case 0:     /* v_color2value */
        PUTS("v_color2value not yet supported\n");
        return 2;

    case 1:     /* v_value2color */
        PUTS("v_value2color not yet supported\n");
        return 6;

    case 2:     /* v_color2nearest */
        PUTS("v_color2nearest not yet supported\n");
        return 6;

    case 3:     /* vq_px_format */
        PUTS("vq_px_format not yet supported\n");
        *(long *)&intout[0] = 1;
        *(long *)&intout[2] = 0x03421820;
        return 4;

    default:
        PUTS("Unknown colour entry operation\n");
        return 0;
    }
}


static int set_col_table(Virtual *vwk, long count, long start, COLOR_ENTRY *values)
{
    Workstation *wk = vwk->real_address;
    Colour *palette;
    DrvPalette palette_pars;

    if (start + count > wk->screen.palette.size)
        count = wk->screen.palette.size - start;

    palette = get_clut(vwk);

    palette_pars.first_pen = start;
    palette_pars.count = count;
    palette_pars.requested = (short *)((long)values | 1);
    palette_pars.palette = palette;

    set_palette(vwk, &palette_pars);

    return count;
}


int set_colour_table(Virtual *vwk, long subfunction, short *intin)
{
    COLOR_TAB *ctab;

    switch (subfunction)
    {
    case 0:     /* vs_ctab */
        ctab = (COLOR_TAB *)intin;
        return set_col_table(vwk, ctab->no_colors, 0, ctab->colors);

    case 1:     /* vs_ctab_entry */
        PUTS("vs_ctab_entry not yet supported\n");
        return 1;      /* Seems to be the only possible value for non-failure */

    case 2:     /* vs_dflt_ctab */
        PUTS("vs_dflt_ctab not yet supported\n");
        return 256;    /* Not really correct */

    default:
        PUTS("Unknown set colour table operation\n");
        return 0;
    }
}


int colour_table(Virtual *vwk, long subfunction, short *intin, short *intout)
{
    switch (subfunction)
    {
    case 0:     /* vq_ctab */
        {
            COLOR_TAB *ctab = (COLOR_TAB *)&intout[0];
            int i;
            Workstation *wk = vwk->real_address;
            long ctab_length = *(long *) &intin[0];
            long length = 48 /* sizeof(COLOR_TAB) */ + wk->screen.palette.size * sizeof(COLOR_ENTRY);
            Colour *palette = vwk->palette;

            /* Negative indices are always in local palette, but this can't be one of those */
            if (!palette || ((long)palette & 1))
                palette = wk->screen.palette.colours;

            PUTS("vq_ctab not yet really supported\n");
            if (length > ctab_length)
            {
                PRINTF(("Too little space available for ctab (%ld when ctab needs %ld)!\n", *(long *) &intin[0], length));
                return 0;
            }
            ctab->magic = 0x63746162; /* 'ctab' */
            ctab->length = length;
            ctab->format = 0;
            ctab->reserved = 0;
            ctab->map_id = 0xbadc0de1;
            ctab->color_space = 1;
            ctab->flags = 0;
            ctab->no_colors = 256;
            ctab->reserved1 = 0;
            ctab->reserved2 = 0;
            ctab->reserved3 = 0;
            ctab->reserved4 = 0;

            for (i = 0; i < wk->screen.palette.size; i++)
            {
                ctab->colors[i].rgb.red = (palette[i].vdi.red * 255L) / 1000;
                ctab->colors[i].rgb.green = (palette[i].vdi.green * 255L) / 1000;
                ctab->colors[i].rgb.blue = (palette[i].vdi.blue * 255L) / 1000;
                ctab->colors[i].rgb.red |= ctab->colors[i].rgb.red << 8;
                ctab->colors[i].rgb.green |= ctab->colors[i].rgb.green << 8;
                ctab->colors[i].rgb.blue |= ctab->colors[i].rgb.blue << 8;
                PRINTF(("[%d] = %04x,%04x,%04x  %04x,%04x,%04x  %04x,%04x,%04x\n", i,
                    ctab->colors[i].rgb.red & 0xffff,
                    ctab->colors[i].rgb.green & 0xffff,
                    ctab->colors[i].rgb.blue & 0xffff,
                    palette[i].vdi.red & 0xffff,
                    palette[i].vdi.green & 0xffff,
                    palette[i].vdi.blue & 0xffff,
                    palette[i].hw.red & 0xffff,
                    palette[i].hw.green & 0xffff,
                    palette[i].hw.blue & 0xffff));
            }
            return length / 2;
        }

    case 1:     /* vq_ctab_entry */
        PUTS("vq_ctab_entry not yet supported\n");
        return 6;

    case 2:     /* vq_ctab_id */
        PUTS("vq_ctab_id not yet supported\n");
        *(long *)&intout[0] = 0xbadc0de1;   /* Not really correct */
        return 2;

    case 3:     /* v_ctab_idx2vdi */
        intout[0] = idx2vdi(vwk->real_address, intin[0]);
        return 1;

    case 4:     /* v_ctab_vdi2idx */
        intout[0] = vdi2idx(vwk->real_address, intin[0]);
        return 1;

    case 5:     /* v_ctab_idx2value */
        PUTS("v_ctab_idx2value not yet supported\n");
        return 2;

    case 6:     /* v_get_ctab_id */
        PUTS("v_get_ctab_id not yet supported\n");
        *(long *)&intout[0] = 0xbadc0de1;   /* Should always be different */
        return 2;

    case 7:     /* vq_dflt_ctab */
        PUTS("vq_dflt_ctan not yet supported\n");
        return 256;    /* Depending on palette size */

    case 8:     /* v_create_ctab */
        PUTS("v_create_ctab not yet supported\n");
        return 2;

    case 9:     /* v_delete_ctab */
        PUTS("v_delete_ctab not yet supported\n");
        intout[0] = 1;   /* OK */
        return 1;

    default:
        PUTS("Unknown colour table operation\n");
        return 0;
    }
}


int inverse_table(Virtual *vwk, long subfunction, short *intin, short *intout)
{
    (void) vwk;
    (void) intin;
    switch (subfunction)
    {
    case 0:     /* v_create_itab */
        {
            /* COLOR_TAB *ctab = (COLOR_TAB *) *(long *)&intin[0]; */

            PUTS("v_create_itab not yet supported\n");
            *(long *)&intout[0] = 0xbadc0de1;
            return 2;
        }

    case 1:     /* v_delete_itab */
        PUTS("v_delete_itab not yet supported\n");
        intout[0] = 1;   /* OK */
        return 1;

    default:
        PUTS("Unknown inverse colour table operation\n");
        return 0;
    }
}
