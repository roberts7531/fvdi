long CDECL c_get_colour_8(Virtual *vwk, long colour);
void CDECL c_get_colours_8(Virtual *vwk, long colour, unsigned long *foreground, unsigned long *background);
void CDECL c_set_colours_8(Virtual *vwk, long start, long entries, unsigned short *requested, Colour palette[]);

long CDECL c_get_colour_16(Virtual *vwk, long colour);
void CDECL c_get_colours_16(Virtual *vwk, long colour, unsigned long *foreground, unsigned long *background);
void CDECL c_set_colours_16(Virtual *vwk, long start, long entries, unsigned short *requested, Colour palette[]);

long CDECL c_get_colour_32(Virtual *vwk, long colour);
void CDECL c_get_colours_32(Virtual *vwk, long colour, unsigned long *foreground, unsigned long *background);
void CDECL c_set_colours_32(Virtual *vwk, long start, long entries, unsigned short *requested, Colour palette[]);

long CDECL c_write_pixel(Virtual *vwk, MFDB *mfdb, long x, long y, long colour);
long CDECL c_read_pixel(Virtual *vwk, MFDB *mfdb, long x, long y);
long CDECL c_line_draw(Virtual *vwk, long x1, long y1, long x2, long y2, long pattern, long colour, long mode);
long CDECL c_expand_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour);
long CDECL c_fill_area(Virtual *vwk, long x, long y, long w, long h, short *pattern, long colour, long mode, long interior_style);
long CDECL c_fill_polygon(Virtual *vwk, short points[], long n, short index[], long moves, short *pattern, long colour, long mode, long interior_style);
long CDECL c_blit_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation);
long CDECL c_text_area(Virtual *vwk, short *text, long length, long dst_x, long dst_y, short *offsets);
long CDECL c_mouse_draw(Workstation *wk, long x, long y, Mouse *mouse);

int nf_initialize(void);

/* Mouse event handling */
extern void (*next_handler) (void);

void event_trampoline(void);
long CDECL event_query(void);
long CDECL event_init(void);
void CDECL event_handler(void);

long CDECL c_get_videoramaddress(void);
long CDECL c_set_resolution(long width, long height, long depth, long freq);
long CDECL c_get_width(void);
long CDECL c_get_height(void);
long CDECL c_openwk(Virtual *vwk);
long CDECL c_closewk(Virtual *vwk);
long CDECL c_get_bpp(void);

void CDECL c_get_component(long component, long *mask, long *shift, long *loss);

long CDECL c_get_hw_colour(short index, long red, long green, long blue, unsigned long *hw_value);
long CDECL c_set_colour(Virtual *vwk, long index, long red, long green, long blue);
