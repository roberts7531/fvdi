
/* from ft2_ftsystem.c */
void ft_keep_open(void);

void ft_keep_closed(void);

/* from engine/text.s */
void CDECL bitmap_outline(void *src, void *dst, long pitch, long wdwidth, long lines);

#ifdef FT_DEBUG_MEMORY

FT_Int ft_mem_debug_init(FT_Memory memory);

void ft_mem_debug_done(FT_Memory memory);

#endif
