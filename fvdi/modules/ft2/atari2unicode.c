#include "fvdi.h"
#include "modules/ft2.h"
short const Atari2Unicode[256] = {
        /* ISO/IEC 10646-1:2000 (UNICODE) Character Name */
/* 00 */    0x0000, /* (Nothing) */
            0x21E7, /* UPWARDS WHITE ARROW */
            0x21E9, /* DOWNWARDS WHITE ARROW */
            0x21E8, /* RIGHTWARDS WHITE ARROW */
            0x21E6, /* LEFTWARDS WHITE ARROW */
            0x2610, /* BALLOT BOX */
            0x2611, /* BALLOT BOX WITH CHECK */
            0x2612, /* BALLOT BOX WITH X */
            0x2713, /* CHECK MARK */
            0x25F7, /* WHITE CIRCLE WITH UPPER RIGHT QUADRANT */
            0x237E, /* BELL SYMBOL */
            0x266A, /* EIGHTH NOTE */
            0x240C, /* SYMBOL FOR FORM FEED */
            0x240D, /* SYMBOL FOR CARRIAGE RETURN */
            0x255D, /* BOX DRAWINGS DOUBLE UP AND LEFT (Fuji left) */
            0x255A, /* BOX DRAWINGS DOUBLE UP AND RIGHT (Fuji right) */
/* 10 */    0x24EA, /* CIRCLED DIGIT ZERO */
            0x2460, /* CIRCLED DIGIT ONE */
            0x2461, /* CIRCLED DIGIT TWO */
            0x2462, /* CIRCLED DIGIT THREE */
            0x2463, /* CIRCLED DIGIT FOUR */
            0x2464, /* CIRCLED DIGIT FIVE */
            0x2465, /* CIRCLED DIGIT SIX */
            0x2466, /* CIRCLED DIGIT SEVEN */
            0x2467, /* CIRCLED DIGIT EIGHT */
            0x2468, /* CIRCLED DIGIT NINE */
            0x20AC, /* EURO SIGN (Upside down small E) */
            0x241B, /* SYMBOL FOR ESCAPE */
            0x256D, /* BOX DRAWINGS LIGHT ARC DOWN AND RIGHT (Face upper left) */
            0x256E, /* BOX DRAWINGS LIGHT ARC DOWN AND LEFT (Face upper right) */
            0x256F, /* BOX DRAWINGS LIGHT ARC UP AND LEFT (Face lower left) */
            0x2570, /* BOX DRAWINGS LIGHT ARC UP AND RIGHT (Face lower right) */
/* 20 */    0x0020, /* SPACE */
            0x0021, /* EXCLAMATION MARK */
            0x0022, /* QUOTATION MARK */
            0x0023, /* NUMBER SIGN */
            0x0024, /* DOLLAR SIGN */
            0x0025, /* PERCENT SIGN */
            0x0026, /* AMPERSAND */
            0x0027, /* APOSTROPHE */
            0x0028, /* LEFT PARENTHESIS */
            0x0029, /* RIGHT PARENTHESIS */
            0x002A, /* ASTERISK */
            0x002B, /* PLUS SIGN */
            0x002C, /* COMMA */
            0x002D, /* HYPHEN-MINUS */
            0x002E, /* FULL STOP */
            0x002F, /* SOLIDUS */
/* 30 */    0x0030, /* DIGIT ZERO */
            0x0031, /* DIGIT ONE */
            0x0032, /* DIGIT TWO */
            0x0033, /* DIGIT THREE */
            0x0034, /* DIGIT FOUR */
            0x0035, /* DIGIT FIVE */
            0x0036, /* DIGIT SIX */
            0x0037, /* DIGIT SEVEN */
            0x0038, /* DIGIT EIGHT */
            0x0039, /* DIGIT NINE */
            0x003A, /* COLON */
            0x003B, /* SEMICOLON */
            0x003C, /* LESS-THAN SIGN */
            0x003D, /* EQUALS SIGN */
            0x003E, /* GREATER-THAN SIGN */
            0x003F, /* QUESTION MARK */
/* 40 */    0x0040, /* COMMERCIAL AT */
            0x0041, /* LATIN CAPITAL LETTER A */
            0x0042, /* LATIN CAPITAL LETTER B */
            0x0043, /* LATIN CAPITAL LETTER C */
            0x0044, /* LATIN CAPITAL LETTER D */
            0x0045, /* LATIN CAPITAL LETTER E */
            0x0046, /* LATIN CAPITAL LETTER F */
            0x0047, /* LATIN CAPITAL LETTER G */
            0x0048, /* LATIN CAPITAL LETTER H */
            0x0049, /* LATIN CAPITAL LETTER I */
            0x004A, /* LATIN CAPITAL LETTER J */
            0x004B, /* LATIN CAPITAL LETTER K */
            0x004C, /* LATIN CAPITAL LETTER L */
            0x004D, /* LATIN CAPITAL LETTER M */
            0x004E, /* LATIN CAPITAL LETTER N */
            0x004F, /* LATIN CAPITAL LETTER O */
/* 50 */    0x0050, /* LATIN CAPITAL LETTER P */
            0x0051, /* LATIN CAPITAL LETTER Q */
            0x0052, /* LATIN CAPITAL LETTER R */
            0x0053, /* LATIN CAPITAL LETTER S */
            0x0054, /* LATIN CAPITAL LETTER T */
            0x0055, /* LATIN CAPITAL LETTER U */
            0x0056, /* LATIN CAPITAL LETTER V */
            0x0057, /* LATIN CAPITAL LETTER W */
            0x0058, /* LATIN CAPITAL LETTER X */
            0x0059, /* LATIN CAPITAL LETTER Y */
            0x005A, /* LATIN CAPITAL LETTER Z */
            0x005B, /* LEFT SQUARE BRACKET */
            0x005C, /* REVERSE SOLIDUS */
            0x005D, /* RIGHT SQUARE BRACKET */
            0x005E, /* CIRCUMFLEX ACCENT */
            0x005F, /* LOW LINE */
/* 60 */    0x0060, /* GRAVE ACCENT */
            0x0061, /* LATIN SMALL LETTER A */
            0x0062, /* LATIN SMALL LETTER B */
            0x0063, /* LATIN SMALL LETTER C */
            0x0064, /* LATIN SMALL LETTER D */
            0x0065, /* LATIN SMALL LETTER E */
            0x0066, /* LATIN SMALL LETTER F */
            0x0067, /* LATIN SMALL LETTER G */
            0x0068, /* LATIN SMALL LETTER H */
            0x0069, /* LATIN SMALL LETTER I */
            0x006A, /* LATIN SMALL LETTER J */
            0x006B, /* LATIN SMALL LETTER K */
            0x006C, /* LATIN SMALL LETTER L */
            0x006D, /* LATIN SMALL LETTER M */
            0x006E, /* LATIN SMALL LETTER N */
            0x006F, /* LATIN SMALL LETTER O */
/* 70 */    0x0070, /* LATIN SMALL LETTER P */
            0x0071, /* LATIN SMALL LETTER Q */
            0x0072, /* LATIN SMALL LETTER R */
            0x0073, /* LATIN SMALL LETTER S */
            0x0074, /* LATIN SMALL LETTER T */
            0x0075, /* LATIN SMALL LETTER U */
            0x0076, /* LATIN SMALL LETTER V */
            0x0077, /* LATIN SMALL LETTER W */
            0x0078, /* LATIN SMALL LETTER X */
            0x0079, /* LATIN SMALL LETTER Y */
            0x007A, /* LATIN SMALL LETTER Z */
            0x007B, /* LEFT CURLY BRACKET */
            0x007C, /* VERTICAL LINE */
            0x007D, /* RIGHT CURLY BRACKET */
            0x007E, /* TILDE */
            0x0394, /* GREEK CAPITAL LETTER DELTA */        /* alt: 0x25b3, WHITE UP-POINTING TRIANGLE, 0x2206, INCREMENT */
/* 80 */    0x00C7, /* LATIN CAPITAL LETTER C WITH CEDILLA */
            0x00FC, /* LATIN SMALL LETTER U WITH DIAERESIS */
            0x00E9, /* LATIN SMALL LETTER E WITH ACUTE */
            0x00E2, /* LATIN SMALL LETTER A WITH CIRCUMFLEX */
            0x00E4, /* LATIN SMALL LETTER A WITH DIAERESIS */
            0x00E0, /* LATIN SMALL LETTER A WITH GRAVE */
            0x00E5, /* LATIN SMALL LETTER A WITH RING ABOVE */
            0x00E7, /* LATIN SMALL LETTER C WITH CEDILLA */
            0x00EA, /* LATIN SMALL LETTER E WITH CIRCUMFLEX */
            0x00EB, /* LATIN SMALL LETTER E WITH DIAERESIS */
            0x00E8, /* LATIN SMALL LETTER E WITH GRAVE */
            0x00EF, /* LATIN SMALL LETTER I WITH DIAERESIS */
            0x00EE, /* LATIN SMALL LETTER I WITH CIRCUMFLEX */
            0x00EC, /* LATIN SMALL LETTER I WITH GRAVE */
            0x00C4, /* LATIN CAPITAL LETTER A WITH DIAERESIS */
            0x00C5, /* LATIN CAPITAL LETTER A WITH RING ABOVE */
/* 90 */    0x00C9, /* LATIN CAPITAL LETTER E WITH ACUTE */
            0x00E6, /* LATIN SMALL LETTER AE */
            0x00C6, /* LATIN CAPITAL LETTER AE */
            0x00F4, /* LATIN SMALL LETTER O WITH CIRCUMFLEX */
            0x00F6, /* LATIN SMALL LETTER O WITH DIAERESIS */
            0x00F2, /* LATIN SMALL LETTER O WITH GRAVE */
            0x00FB, /* LATIN SMALL LETTER U WITH CIRCUMFLEX */
            0x00F9, /* LATIN SMALL LETTER U WITH GRAVE */
            0x00FF, /* LATIN SMALL LETTER Y WITH DIAERESIS */
            0x00D6, /* LATIN CAPITAL LETTER O WITH DIAERESIS */
            0x00DC, /* LATIN CAPITAL LETTER U WITH DIAERESIS */
            0x00A2, /* CENT SIGN */
            0x00A3, /* POUND SIGN */
            0x00A5, /* YEN SIGN */
            0x00DF, /* LATIN SMALL LETTER SHARP S */
            0x0192, /* LATIN SMALL LETTER F WITH HOOK */
/* a0 */    0x00E1, /* LATIN SMALL LETTER A WITH ACUTE */
            0x00ED, /* LATIN SMALL LETTER I WITH ACUTE */
            0x00F3, /* LATIN SMALL LETTER O WITH ACUTE */
            0x00FA, /* LATIN SMALL LETTER U WITH ACUTE */
            0x00F1, /* LATIN SMALL LETTER N WITH TILDE */
            0x00D1, /* LATIN CAPITAL LETTER N WITH TILDE */
            0x00AA, /* FEMININE ORDINAL INDICATOR */
            0x00BA, /* MASCULINE ORDINAL INDICATOR */
            0x00BF, /* INVERTED QUESTION MARK */
            0x2310, /* REVERSED NOT SIGN */
            0x00AC, /* NOT SIGN */
            0x00BD, /* VULGAR FRACTION ONE HALF */
            0x00BC, /* VULGAR FRACTION ONE QUARTER */
            0x00A1, /* INVERTED EXCLAMATION MARK */
            0x00AB, /* LEFT-POINTING DOUBLE ANGLE QUOTATION MARK */
            0x00BB, /* RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK */
/* b0 */    0x00E3, /* LATIN SMALL LETTER A WITH TILDE */
            0x00F5, /* LATIN SMALL LETTER O WITH TILDE */
            0x00D8, /* LATIN CAPITAL LETTER O WITH STROKE */
            0x00F8, /* LATIN SMALL LETTER O WITH STROKE */
            0x0153, /* LATIN SMALL LIGATURE OE */
            0x0152, /* LATIN CAPITAL LIGATURE OE */
            0x00C0, /* LATIN CAPITAL LETTER A WITH GRAVE */
            0x00C3, /* LATIN CAPITAL LETTER A WITH TILDE */
            0x00D5, /* LATIN CAPITAL LETTER O WITH TILDE */
            0x00A8, /* DIAERESIS */
            0x00B4, /* ACUTE ACCENT */
            0x2020, /* DAGGER */
            0x00B6, /* PILCROW SIGN */
            0x00A9, /* COPYRIGHT SIGN */
            0x00AE, /* REGISTERED SIGN */
            0x2122, /* TRADE MARK SIGN */
/* c0 */    0x0133, /* LATIN SMALL LIGATURE IJ */
            0x0132, /* LATIN CAPITAL LIGATURE IJ */
            0x05D0, /* HEBREW LETTER ALEF */
            0x05D1, /* HEBREW LETTER BET */
            0x05D2, /* HEBREW LETTER GIMEL */
            0x05D3, /* HEBREW LETTER DALET */
            0x05D4, /* HEBREW LETTER HE */
            0x05D5, /* HEBREW LETTER VAV */
            0x05D6, /* HEBREW LETTER ZAYIN */
            0x05D7, /* HEBREW LETTER HET */
            0x05D8, /* HEBREW LETTER TET */
            0x05D9, /* HEBREW LETTER YOD */
            0x05DB, /* HEBREW LETTER KAF */
            0x05DC, /* HEBREW LETTER LAMED */
            0x05DE, /* HEBREW LETTER MEM */
            0x05E0, /* HEBREW LETTER NUN */
/* d0 */    0x05E1, /* HEBREW LETTER SAMEKH */
            0x05E2, /* HEBREW LETTER AYIN */
            0x05E4, /* HEBREW LETTER PE */
            0x05E6, /* HEBREW LETTER TSADI */
            0x05E7, /* HEBREW LETTER QOF */
            0x05E8, /* HEBREW LETTER RESH */
            0x05E9, /* HEBREW LETTER SHIN */
            0x05EA, /* HEBREW LETTER TAV */
            0x05DF, /* HEBREW LETTER FINAL NUN */
            0x05DA, /* HEBREW LETTER FINAL KAF */
            0x05DD, /* HEBREW LETTER FINAL MEM */
            0x05E3, /* HEBREW LETTER FINAL PE */
            0x05E5, /* HEBREW LETTER FINAL TSADI */
            0x00A7, /* SECTION SIGN */
            0x2227, /* LOGICAL AND */
            0x221E, /* INFINITY */
/* e0 */    0x03B1, /* GREEK SMALL LETTER ALPHA */
            0x03B2, /* GREEK SMALL LETTER BETA */
            0x0393, /* GREEK CAPITAL LETTER GAMMA */
            0x03C0, /* GREEK SMALL LETTER PI */
            0x03A3, /* GREEK CAPITAL LETTER SIGMA */
            0x03C3, /* GREEK SMALL LETTER SIGMA */
            0x00B5, /* MICRO SIGN */
            0x03C4, /* GREEK SMALL LETTER TAU */
            0x03A6, /* GREEK CAPITAL LETTER PHI */
            0x0398, /* GREEK CAPITAL LETTER THETA */
            0x03A9, /* GREEK CAPITAL LETTER OMEGA */
            0x03B4, /* GREEK SMALL LETTER DELTA */
            0x222E, /* CONTOUR INTEGRAL */
            0x03C6, /* GREEK SMALL LETTER PHI */
            0x03B5, /* GREEK SMALL LETTER EPSILON */
            0x2229, /* INTERSECTION */
/* f0 */    0x2261, /* IDENTICAL TO */
            0x00B1, /* PLUS-MINUS SIGN */
            0x2265, /* GREATER-THAN OR EQUAL TO */
            0x2264, /* LESS-THAN OR EQUAL TO */
            0x2320, /* TOP HALF INTEGRAL */
            0x2321, /* BOTTOM HALF INTEGRAL */
            0x00F7, /* DIVISION SIGN */
            0x2248, /* ALMOST EQUAL TO */
            0x00B0, /* DEGREE SIGN */
            0x2219, /* BULLET OPERATOR */
            0x00B7, /* MIDDLE DOT */
            0x221A, /* SQUARE ROOT */
            0x207F, /* SUPERSCRIPT LATIN SMALL LETTER N */
            0x00B2, /* SUPERSCRIPT TWO */
            0x00B3, /* SUPERSCRIPT THREE */
            0x00af  /* MACRON */
};
