#if 0
#define FAST		/* Write in FastRAM buffer */
#define BOTH		/* Write in both FastRAM and on screen */
#else
#undef FAST
#undef BOTH
#endif

#include "../16_bit/16b_exp.c"
#include "../16_bit/16b_blit.c"
#include "../16_bit/16b_line.c"
#include "../16_bit/16b_fill.c"
#include "../16_bit/16b_scr.c"
#include "../16_bit/16b_pal.c"
