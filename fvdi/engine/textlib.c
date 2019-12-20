/* No debugging right now! */
#undef DEB
/*
 * fVDI text handling
 *
 * $Id: textlib.c,v 1.16 2006-12-06 20:36:12 standa Exp $
 *
 * Copyright 2005, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

#include <stdlib.h>
#include "fvdi.h"
#include "function.h"
#include "globals.h"

#define BLACK 1


void lib_vqt_extent(Virtual *vwk, long length, short *string, short *points);

static inline void set_current_font(Virtual *vwk, Fontheader *font)
{
    /* Adjust the font structure utilization counters */
    if (vwk->text.current_font != font) {
        if (vwk->text.current_font)
            vwk->text.current_font->extra.ref_count--;
        font->extra.ref_count++;
    }

    vwk->text.current_font = font;
}

                                                              // colour_set = lib_vst_color(colour)
                                                              long lib_vst_color(Virtual *vwk, unsigned long colour)
{
                                                              if (colour >= vwk->real_address->screen.palette.size)
                                                              colour = BLACK;
                                                              vwk->text.colour.foreground = colour;

                                                              return colour;
                                                              }

                                                              // effects_set = lib_vst_effects(effects)
                                                              long lib_vst_effects(Virtual *vwk, long effects)
{
                                                              effects &= vwk->real_address->writing.effects;
                                                              vwk->text.effects = effects;

                                                              return effects;
                                                              }

                                                              // lib_vst_alignment(halign, valign, &hresult, &vresult)
                                                              void lib_vst_alignment(Virtual *vwk, unsigned long halign, unsigned long valign,
                                                                                     short *hresult, short *vresult)
{
                                                              if (halign > 2)    /* Not from wk struct? */
                                                              halign = 0;    /* Left */
                                                              if (valign > 5)    /* Not from wk struct? */
                                                              valign = 0;    /* Baseline */

                                                              *hresult = vwk->text.alignment.horizontal = halign;
                                                              *vresult = vwk->text.alignment.vertical   = valign;
                                                              }

                                                              #if 0
                                                              // Todo: Check if any angle is allowed.
                                                              // angle_set = lib_vst_rotation(angle)
                                                              lib_vst_rotation:
                                                              move.w	(a1),d0
move.l	vwk_real_address(a0),a2
tst.w	wk_writing_rotation_possible(a2)
lbeq	.none,1
cmp.w	#1,wk_writing_rotation_type(a2)
lblo	.none,1
lbhi	.any,3
add.w	#450,d0
divu	#900,d0			; Only allow right angles
cmp.w	#3,d0			; Should probably check font
lbls	.ok,2
label .none,1
moveq	#0,d0
label .ok,2
mulu	#900,d0
label .any,3
move.w	d0,vwk_text_rotation(a0)
rts
#endif

// Todo: Also look for correct size?
// font_set = lib_vst_font(fontID)
int lib_vst_font(Virtual *vwk, long fontID)
{
    Fontheader *font;
    char buf[10];
    short dummy, size;

    if (!fontID)
        fontID = 1;

#if DEB
    puts("lib_vst_font ");
    ltoa(buf, fontID, 10);
    puts(buf);
    puts("\x0d\x0a");
#endif

    font = vwk->real_address->writing.first_font;
    if (vwk->text.current_font) {
        if (vwk->text.font == fontID)
            return fontID;
        if (vwk->text.font > fontID)
            font = font->extra.first_size;
    }


    do {
#if DEB
        puts("  loop\x0d\x0a");
        ltoa(buf, font->id, 10);
        puts(buf);
        puts("\x0d\x0a");
#endif
        if (fontID <= font->id)
            break;
        font = font->next;
    } while (font);

    if (!font || (font->id != fontID)) {
#if DEB
        puts("  set first\x0d\x0a");
#endif
        fontID = 1;
        font = vwk->real_address->writing.first_font;
    }

    if (vwk->text.current_font)
        size = vwk->text.current_font->size;
    else
        size = 10;


    vwk->text.font = fontID;
    set_current_font(vwk, font);

#if 0
    vwk->text.character.width  = font->widest.character;
    vwk->text.character.height = font->distance.top;
    vwk->text.cell.width       = font->widest.cell;
    vwk->text.cell.height      = font->height;
#else
    /* Choose the right size */
    lib_vst_point(vwk, size, &dummy, &dummy, &dummy, &dummy);
#endif

    return fontID;
}


// Apparently extended since NVDI 3.00 (add 33rd word, bitmap/vector flag)
// Perhaps a version that returns the name as 32 bytes rather than 32 words?
// id = lib_vqt_name(number, name)
long lib_vqt_name(Virtual *vwk, long number, short *name)
{
    int i;
    Fontheader *font;
    unsigned char *font_name;

    if (!number || ((unsigned long) number > vwk->real_address->writing.fonts))
        number = 1;

    font = vwk->real_address->writing.first_font;
    for (number -= 2; number >= 0; number--)
        font = font->next;

    font_name = font->name;
    for (i = 31; i >= 0; i--)
        *name++ = *font_name++;

    if (font->flags & 0x4000)
        *name = 1;   /* Vector font! */
    else
        *name = 0;

    return font->id;
}



// lib_vqt_font_info(&minchar, &maxchar, distance, &maxwidth, effects)
void lib_vqt_font_info(Virtual *vwk, short *minchar, short *maxchar,
                       short *distance, short *maxwidth, short *effects)
{
    *minchar    = vwk->text.current_font->code.low;
    *maxchar    = vwk->text.current_font->code.high;
    distance[0] = vwk->text.current_font->distance.bottom;
    distance[1] = vwk->text.current_font->distance.descent;
    distance[2] = vwk->text.current_font->distance.half;
    distance[3] = vwk->text.current_font->distance.ascent;
    distance[4] = vwk->text.current_font->distance.top;
    *maxwidth   = vwk->text.current_font->widest.cell;
    effects[0]  = 0;   /* Temporary current spec. eff. change of width! */
    effects[1]  = 0;   /* Temporary current spec. eff. change to left! */
    effects[2]  = 0;   /* Temporary current spec. eff. change to right! */
}


void lib_vqt_xfntinfo(Virtual *vwk, long flags, long id, long index,
                      XFNT_INFO *info)
{
    int i;
    Fontheader *font;

    font = vwk->real_address->writing.first_font;

    if (index) {
        for(i = index - 1; i >= 0; i--) {
            if (!(font = font->next))
                break;
        }
        if (font) {
            id = font->id;
        }
    } else if (id) {
        index = 1;
    } else {
        if (!vwk->text.current_font) {
            set_current_font(vwk, font);
        }
        id = vwk->text.current_font->id;
        index = 1;
    }

    while (font && (id > font->id)) {
        font = font->next;
        index++;
    }
    if (font && (id != font->id))
        font = 0;

    if (!font) {
        info->format = 0;
        info->id     = 0;
        info->index  = 0;
        return;
    }

    info->id     = id;
    info->index  = index;

    if ((font->flags & 0x8000) && external_xfntinfo) {
        set_stack_call_lvplp(vdi_stack_top, vdi_stack_size,
                             external_xfntinfo,
                             vwk, font, flags, info);
        return;
    }

    info->format = 1;

    if (flags & 0x01) {
        for(i = 0; i < 32; i++) {
            info->font_name[i] = font->name[i];
        }
        info->font_name[i] = 0;
    }

    /* Dummy text */
    if (flags & 0x02) {
        strncpy(info->family_name, "Century 725 Italic BT",
                sizeof(info->family_name) - 1);
        info->family_name[sizeof(info->family_name) - 1] = 0;
    }

    /* Dummy text */
    if (flags & 0x04) {
        strncpy(info->style_name, "Italic",
                sizeof(info->style_name) - 1);
        info->style_name[sizeof(info->style_name) - 1] = 0;
    }

    if (flags & 0x08) {
        info->file_name1[0] = 0;
    }

    if (flags & 0x10) {
        info->file_name2[0] = 0;
    }

    if (flags & 0x20) {
        info->file_name3[0] = 0;
    }

    /* 0x100 is without enlargement, 0x200 with */
    if (flags & 0x300) {
        i = 0;
        font = font->extra.first_size;
        while (font) {
            info->pt_sizes[i] = font->size;
            i++;
            font = font->extra.next_size;
        }
        info->pt_cnt = i;
    }
}


/* Which one of these is really correct? */
#if 0
void lib_vqt_fontheader(Virtual *vwk, VQT_FHDR *fhdr, const char *filename)
#else
void lib_vqt_fontheader(Virtual *vwk, VQT_FHDR *fhdr)
#endif
{
    int i;
    Fontheader *font;

    /* Strings should not have NUL termination if max size. */
    /* Normally 1000 ORUs per Em square (width of 'M'), but header says. */
    /* 6 byte transformation parameters contain:
   *  short y offset (ORUs)
   *  short x scaling (units of 1/4096)
   *  short y scaling (units of 1/4096)
   */

    /* Is this correct? */
    font = vwk->text.current_font;

    if ((font->flags & 0x8000) && external_fontheader) {
        set_stack_call_lvppl(vdi_stack_top, vdi_stack_size,
                             external_fontheader,
                             vwk, font, fhdr, 0);
        return;
    }

    memcpy(fhdr->fh_fmver, "D1.0\x0d\x0a\0\0", 8);  /* Format identifier */
    fhdr->fh_fntsz = 0;     /* Font file size */
    fhdr->fh_fbfsz = 0;     /* Minimum font buffer size (non-image data) */
    fhdr->fh_cbfsz = 0;     /* Minimum character buffer size (largest char) */
    fhdr->fh_hedsz = sizeof(VQT_FHDR);  /* Header size */
    fhdr->fh_fntid = 0;     /* Font ID (Bitstream) */
    fhdr->fh_sfvnr = 0;     /* Font version number */
    for(i = 0; i < 32; i++) {   /* Font full name (vqt_name) */
        fhdr->fh_fntnm[i] = font->name[i];
    }
    fhdr->fh_fntnm[i] = 0;
    fhdr->fh_mdate[0] = 0;  /* Manufacturing date (DD Mon YY) */
    fhdr->fh_laynm[0] = 0;  /* Character set name, vendor ID, character set ID */
    /* Last two is char set, usually the second two characters in font filename
   *   Bitstream International Character Set = '00'
   * Two before that is manufacturer, usually first two chars in font filename
   *   Bitstream fonts use 'BX'
   */
    fhdr->fh_cpyrt[0] = 0;  /* Copyright notice */
    fhdr->fh_nchrl = 0;     /* Number of character indices in character set */
    fhdr->fh_nchrf = 0;     /* Total number of character indices in font */
    fhdr->fh_fchrf = 0;     /* Index of first character */
    fhdr->fh_nktks = 0;     /* Number of kerning tracks */
    fhdr->fh_nkprs = 0;     /* Number of kerning pairs */
    fhdr->fh_flags = 0;     /* Font flags, bit 0 - extended mode */
    /* Extended mode is for fonts that require higher quality of rendering,
   * such as chess pieces. Otherwise compact, the default.
   */
    fhdr->fh_cflgs = 1;     /* Classification flags */
    /* bit 0 - Italic
   * bit 1 - Monospace
   * bit 2 - Serif
   * bit 3 - Display
   */
    fhdr->fh_famcl = 0;     /* Family classification */
    /* 0 - Don't care
   * 1 - Serif
   * 2 - Sans serif
   * 3 - Monospace
   * 4 - Script
   * 5 - Decorative
   */
    fhdr->fh_frmcl = 0x68;  /* Font form classification */
    /* 0x_4 - Condensed
   * 0x_5 - (Reserved for 3/4 condensed)
   * 0x_6 - Semi-condensed
   * 0x_7 - (Reserved for 1/4 condensed)
   * 0x_8 - Normal
   * 0x_9 - (Reserved for 3/4 expanded)
   * 0x_a - Semi-expanded
   * 0x_b - (Reserved for 1/4 expanded)
   * 0x_c - Expanded
   * 0x1_ - Thin
   * 0x2_ - Ultralight
   * 0x3_ - Extralight
   * 0x4_ - Light
   * 0x5_ - Book
   * 0x6_ - Normal
   * 0x7_ - Medium
   * 0x8_ - Semibold
   * 0x9_ - Demibold
   * 0xa_ - Bold
   * 0xb_ - Extrabold
   * 0xc_ - Ultrabold
   * 0xd_ - Heavy
   * 0xe_ - Black
   */
    /* Dummy text */
    strncpy(fhdr->fh_sfntn, "Century725BT-Italic",  /* Short font name */
            sizeof(fhdr->fh_sfntn));
    /* Abbreviation of Postscript equivalent font name */
    strncpy(fhdr->fh_sfacn, "Century 725 BT",  /* Short face name */
            sizeof(fhdr->fh_sfacn));
    /* Abbreviation of the typeface family name */
    strncpy(fhdr->fh_fntfm, "Italic",   /* Font form (as above), style */
            sizeof(fhdr->fh_fntfm));
    fhdr->fh_itang = 0;     /* Italic angle */
    /* Skew in 1/256 of degrees clockwise, if italic font */
    fhdr->fh_orupm = 2048;  /* ORUs per Em */
    /* Outline Resolution Units */

    /* There's actually a bunch of more values, but they are not
   * in the struct definition, so skip them
   */
}


// lib_vqt_extent(length, &string, points)
//_get_extent:
//	move.l	4(a7),a0		; vwk as parameter
//	lea	8(a7),a1
void lib_vqt_extent(Virtual *vwk, long length, short *string, short *points)
{
    short ch, width;
    unsigned short low, high;
    short *char_tab;

    /* Some other method should be used for this! */
    if (vwk->text.current_font->flags < 0) {
        /* Handle differently? This is not really allowed at all! */
        if (!external_vqt_extent)
            return;
        width = set_stack_call_lpppll(vdi_stack_top, vdi_stack_size,
                                      external_vqt_extent,
                                      vwk, vwk->text.current_font, string, length, 0);
    } else {

        char_tab = vwk->text.current_font->table.character;
        low      = vwk->text.current_font->code.low;
        high     = vwk->text.current_font->code.high;
        width   = 0;

        for(length-- ;length >= 0; length--) {
            ch = *string++ - low;
            /* Negative numbers are very high as unsigned */
            if ((unsigned short)ch <= high)
                width += char_tab[ch + 1] - char_tab[ch];
        }

        if (vwk->text.effects & 0x01)
            width += vwk->text.current_font->thickening;

        if (vwk->text.effects & 0x10)   /* Outline */
            width += 2;

        if (!(vwk->text.effects & 0x04)) {
            unsigned short skewing = vwk->text.current_font->skewing;
            short height = vwk->text.current_font->height;
            for(height--; height >= 0; height--) {
                skewing = (skewing << 1) | (skewing >> 15);
                width += skewing & 1;
            }
        }
    }

#ifndef SUB1
    points[0] = 0;
    points[1] = 0;
    points[2] = width;
    points[3] = 0;
    points[4] = width;
    points[5] = vwk->text.current_font->height;
    points[6] = 0;
    points[7] = vwk->text.current_font->height;
#else
    points[0] = 0;
    points[1] = 0;
    points[2] = width - 1;
    points[3] = 0;
    points[4] = width - 1;
    points[5] = vwk->text.current_font->height - 1;
    points[6] = 0;
    points[7] = vwk->text.current_font->height - 1;
#endif
}


int lib_vst_point(Virtual *vwk, long height, short *charw, short *charh,
                  short *cellw, short *cellh)
{
    Fontheader *font;

    /* Some other method should be used for this! */
#if DEB
    puts("lib_vst_point\x0d\x0a");
#endif
    if (vwk->text.current_font->flags < 0) {
#if DEB
        puts("  vector\x0d\x0a");
#endif
        /* Handle differently? This is not really allowed at all! */
        if (!external_vst_point)
            return 0;
#if DEB
        puts("  vector ok\x0d\x0a");
#endif
        font = set_stack_call_pvlpl(vdi_stack_top, vdi_stack_size,
                                    external_vst_point,
                                    vwk, height, sizes, 0);

        /* fall back to the built-in bitmap font
     * in case something went wrong */
        if ( !font ) {
            puts("vst_point: external_vst_point returned NULL\x0d\x0a");
            font = vwk->real_address->writing.first_font;
        }
#if DEB
        puts("  vector found\x0d\x0a");
#endif
    } else {
#if DEB
        char buf[10];
        ltoa(buf, height, 10);
        puts(buf);
        puts(" ");
#endif

        font = vwk->text.current_font->extra.first_size;

        while (font->extra.next_size && (font->extra.next_size->size <= height)) {
            font = font->extra.next_size;
        }
#if DEB
        ltoa(buf, font->height, 10);
        puts(buf);
        puts("\x0d\x0a");
#endif
    }

    set_current_font(vwk, font);

    *charw = vwk->text.character.width  = font->widest.character;
    *charh = vwk->text.character.height = font->distance.top;
    *cellw = vwk->text.cell.width       = font->widest.cell;
    *cellh = vwk->text.cell.height      = font->height;

    return font->size;
}


int lib_vst_arbpt(Virtual *vwk, long height, short *charw, short *charh,
                  short *cellw, short *cellh)
{
    Fontheader *font;

    /* Some other method should be used for this! */
#if DEB
    puts("lib_vst_arbpt\x0d\x0a");
#endif
    if (vwk->text.current_font->flags < 0) {
#if DEB
        puts("  vector\x0d\x0a");
#endif
        /* Handle differently? This is not really allowed at all! */
        if (!external_vst_point)
            return 0;
#if DEB
        puts("  vector ok\x0d\x0a");
#endif
        font = set_stack_call_pvlpl(vdi_stack_top, vdi_stack_size,
                                    external_vst_point,
                                    vwk, height, 0, 0);

        /* fall back to the built-in bitmap font
     * in case something went wrong */
        if ( !font ) {
            puts("vst_arbpt: external_vst_point returned NULL\x0d\x0a");
            font = vwk->real_address->writing.first_font;
        }
#if DEB
        puts("  vector found\x0d\x0a");
#endif
    } else {
#if DEB
        char buf[10];
        ltoa(buf, height, 10);
        puts(buf);
        puts(" ");
#endif

        font = vwk->text.current_font->extra.first_size;

        while (font->extra.next_size && (font->extra.next_size->size <= height)) {
            font = font->extra.next_size;
        }
#if DEB
        ltoa(buf, font->height, 10);
        puts(buf);
        puts("\x0d\x0a");
#endif
    }

    set_current_font(vwk, font);

    *charw = vwk->text.character.width  = font->widest.character;
    *charh = vwk->text.character.height = font->distance.top;
    *cellw = vwk->text.cell.width       = font->widest.cell;
    *cellh = vwk->text.cell.height      = font->height;

    return font->size;
}


// lib_vqt_attributes(settings)
void lib_vqt_attributes(Virtual *vwk, short *settings)
{
    settings[0] = vwk->text.font;
    settings[1] = vwk->text.colour.foreground;
    settings[2] = vwk->text.rotation;
    settings[3] = vwk->text.alignment.horizontal;
    settings[4] = vwk->text.alignment.vertical;
    settings[5] = vwk->mode;
    settings[6] = vwk->text.character.width;
    settings[7] = vwk->text.character.height;
    settings[8] = vwk->text.cell.width;
    settings[9] = vwk->text.cell.height;
}

// fonts_loaded = lib_vst_load_fonts(select)
long lib_vst_load_fonts(Virtual *vwk, long select)
{
    return vwk->real_address->writing.fonts - 1;
}

// lib_vst_unload_fonts(select)
void lib_vst_unload_fonts(Virtual *vwk, long select)
{
}
